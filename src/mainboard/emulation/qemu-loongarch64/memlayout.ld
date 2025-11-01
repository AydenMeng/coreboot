/* SPDX-License-Identifier: GPL-2.0-only */

#include <memlayout.h>

#include <arch/header.ld>

#define QEMU_VIRT_FLASH        0x1c000000
#define QEMU_BOOTBLOCK_SIZE    (160K)
/* Leave a small guard window after the bootblock so that the ROM-stage load
 * address stays strictly above the bootblock image once the linker accounts
 * for the bootblock's metadata and alignment requirements.  Without this
 * cushion ld.bfd aborts with "romstage overlaps the previous region" when the
 * bootblock grows close to its 160 KiB budget. */
#define QEMU_ROM_GUARD         (32K)

SECTIONS
{
	FMAP_CACHE(0x00000000, 2K)
	CBFS_MCACHE(0x00000800, 8K)

	TIMESTAMP(0x00002800, 1K)

	CBFS_CACHE(0x00003000, 8K)

	PRERAM_CBMEM_CONSOLE(0x00005000, 128K)

	STACK(0x00025000, 64K)

	RAMSTAGE(0x00040000, 2M)

        BOOTBLOCK(QEMU_VIRT_FLASH, QEMU_BOOTBLOCK_SIZE)
        ROMSTAGE(QEMU_VIRT_FLASH + QEMU_BOOTBLOCK_SIZE + QEMU_ROM_GUARD, 128K)

}
