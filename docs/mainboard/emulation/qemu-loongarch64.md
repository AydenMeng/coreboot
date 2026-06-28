# QEMU LoongArch64 virt — Coreboot Port

## Architecture overview

LoongArch is a 64-bit RISC ISA maintained by the Loongson Technology
Corporation.  The QEMU `virt` machine emulates a LoongArch 3A5000
(LA464) CPU with:

- NS16550 UART at MMIO `0x1fe001e0`
- CFI parallel flash (pflash) at `0x1c000000`, 4 MiB
- Low DRAM window `0x00000000–0x10000000` (256 MiB)
- High DRAM window `0x80000000–…`
- Platform bus and PCIe (GPEX host bridge)
- ACPI GED for power management
- Fw_cfg for device information

## Boot flow

```
QEMU virt → pflash @ 0x1c000000
  │
  ├─ bootblock (XIP from flash, 128 KiB)
  │   • _start → disable_interrupts → set stack
  │   → copy exception vectors to DRAM
  │   → set_exception_base_address
  │   → bl main (src/lib/bootblock.c)
  │      → init_timer()
  │      → bootblock_main_with_timestamp()
  │         → bootblock_mainboard_early_init()
  │         → console_init()       ← UART initialised
  │         → bootblock_mainboard_init()
  │         → run_romstage()
  │
  ├─ romstage (loaded from CBFS → DRAM 0x250000)
  │   • stage_entry() → main()
  │     → console_init()
  │     → romstage_main()
  │       → cbmem_initialize_empty()
  │       → run_ramstage()
  │
  └─ ramstage (loaded from CBFS → DRAM 0x40000)
      • hardwaremain.c
        → ACPI / SMBIOS table generation
        → payload loading
```

## Memory map (QEMU virt machine)

| Address range | Size | Description |
|---|---|---|
| `0x00000000` – `0x0FFFFFFF` | 256 MiB | Low DRAM |
| `0x10000000` – `0x1BFFFFFF` | 192 MiB | MMIO (PCI, devices) |
| `0x1C000000` – `0x1FFFFFFF` | 64 MiB  | pflash (BIOS) |
| `0x80000000` – … | variable | High DRAM (>256 MiB) |

### Coreboot-internal DRAM layout

| Address | Size | Use |
|---|---|---|
| `0x0000–0x07FF` | 2 KiB  | FMAP cache |
| `0x0800–0x27FF` | 8 KiB  | CBFS metadata cache |
| `0x2800–0x2BFF` | 1 KiB  | Timestamps |
| `0x3000–0x4FFF` | 8 KiB  | CBFS data cache |
| `0x5000–0x24FFF` | 128 KiB | Pre-RAM CBMEM console |
| `0x25000–0x34FFF` | 64 KiB  | Stack |
| `0x40000–0x23FFFF` | 2 MiB  | Ramstage load address |
| `0x250000–0x28FFFF` | 256 KiB | Romstage load address |
| `0x1C000000–0x1C01FFFF` | 128 KiB | Bootblock (flash XIP) |

## CSR register map (selected)

| CSR | Number | Description |
|---|---|---|
| `LOONGARCH_CSR_CRMD` | 0x00 | Current mode (PLV, IE, PG) |
| `LOONGARCH_CSR_PRMD` | 0x01 | Previous mode |
| `LOONGARCH_CSR_ECFG` | 0x04 | Exception config |
| `LOONGARCH_CSR_ESTAT` | 0x05 | Exception status |
| `LOONGARCH_CSR_ERA` | 0x06 | Exception return address |
| `LOONGARCH_CSR_EBASE` | 0x0C | Exception entry base |
| `LOONGARCH_CSR_CPUID` | 0x20 | CPU core ID |
| `LOONGARCH_CSR_TMID` | 0x40 | Timer ID |
| `LOONGARCH_CSR_CNTC` | 0x43 | Constant timer count |
| `LOONGARCH_CSR_DMWIN0` | 0x180 | Direct map window 0 |
| `LOONGARCH_CSR_DMWIN1` | 0x181 | Direct map window 1 |

## Key hardware addresses

| Symbol | Address | Device |
|---|---|---|
| `VIRT_UART0` | `0x1fe001e0` | NS16550 serial |
| `VIRT_FLASH` | `0x1c000000` | pflash (BIOS) |
| `VIRT_FWCFG` | `0x1e020000` | QEMU fw_cfg |
| `VIRT_GED_REG` | `0x100e0020` | ACPI GED reset |
| `VIRT_RTC` | `0x100d0100` | LS7A RTC |
| `VIRT_PCI_CFG` | `0x20000000` | PCIe ECAM |
| `VIRT_PCI_MEM` | `0x40000000` | PCIe MMIO |
| `VIRT_PCI_IO` | `0x18004000` | PCIe PIO |

## Toolchain

The board expects a LoongArch64 cross compiler with prefix
`loongarch64-linux-gnu-` or `loongarch64-unknown-linux-gnu-`.
Install from your distribution or use coreboot's `crossgcc`:

```bash
make crossgcc-loongarch64 CPUS=$(nproc)
```

## Build

```bash
make menuconfig
# Mainboard → Emulation → QEMU loongarch64
# Payload → Don't add a payload
make
```

The resulting ROM is at `build/coreboot.rom` (4 MiB).

## Test

```bash
qemu-system-loongarch64 -M virt -m 512M \
  -bios build/coreboot.rom -serial stdio
```

## ACPI tables

LoongArch64 uses hardware-reduced ACPI (ACPI 6.5+).  The FADT sets
HW_REDUCED_ACPI and configures the ACPI GED register at `0x100e0020`
for system reset.  MADT entries use the following interrupt controller
types (non-APIC):

| Type | Name | Description |
|---|---|---|
| 0x11 | CORE_PIC | Per-CPU core interrupt controller |
| 0x12 | LIO_PIC | Legacy I/O interrupt controller (PCH‑PIC) |
| 0x13 | EIO_PIC | Extended I/O interrupt controller |
| 0x14 | MSI_PIC | MSI interrupt controller (PCH‑MSI) |
| 0x15 | BIO_PIC | Bridge I/O interrupt controller |
| 0x16 | HT_PIC  | HyperTransport interrupt controller |

## SMBIOS tables

SMBIOS type 4 (Processor Information) reads the LoongArch PRID
from `cpucfg` register 0 and reports core count from `CONFIG_MAX_CPUS`.

## Known issues / TODO

1. **CBFS master header** — cbfstool writes wrong `offset` and
   `rom_size` fields in the CBFS master header (see workaround below).

2. **fw_cfg integration** — QEMU provides ACPI tables, SMBIOS data,
   and the memory map via fw_cfg (`0x1e020000`).  A coreboot driver
   should read these to avoid hard‑coded values.

3. **MMU / page tables** — The platform PEI in EDK2 initialises the
   page table and enables the MMU.  Coreboot should do the same for
   proper DMA and device access.

4. **PCI / virtio** — The GPEX host bridge at `0x20000000` (ECAM)
   needs a coreboot driver to enumerate PCI devices.

### CBFS master header workaround

```bash
#!/bin/sh
rom="$1"
printf '\x00\x02\x02\x00' | dd of="$rom" bs=1 seek=$((0x3FFFFC)) conv=notrunc
printf '\x00\x00\x00\x80' | dd of="$rom" bs=1 seek=$((0x20214)) conv=notrunc
printf '\x00\xfe\x3d\x00' | dd of="$rom" bs=1 seek=$((0x20208)) conv=notrunc
```

## References

- [EDK2 LoongArchVirt platform](https://github.com/tianocore/edk2/tree/master/OvmfPkg/LoongArchVirt)
- [QEMU LoongArch virt machine](https://gitlab.com/qemu-project/qemu/-/tree/master/hw/loongarch)
- [LoongArch instruction set manual](https://loongson.github.io/LoongArch-Documentation/)
- [ACPI specification 6.5](https://uefi.org/specs/ACPI/6.5/)
