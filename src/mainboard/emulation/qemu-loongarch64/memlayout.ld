/* SPDX-License-Identifier: GPL-2.0-only */

#include <memlayout.h>

#include <arch/header.ld>

SECTIONS
{
	REGION(flash, 0x1c000000, 4M, 0) \

	DRAM_START(0x90000000)
	BOOTBLOCK(0x90000000, 32K)

	ROMSTAGE(0x90008000, 128K)

	FMAP_CACHE(0x90028000, 2K)
	CBFS_MCACHE(0x9002a000, 8K)
	TIMESTAMP(0x90032000, 1K)
	CBFS_CACHE(0x90033000, 8K)
	PRERAM_CBMEM_CONSOLE(0x9003b000, 128K)



	/* By default all memory addresses are affected by the value of HRMOR
	 * (Hypervisor Real Mode Offset Register) which is ORed to them. HRMOR
	 * has initial value of 0x8000000 in QEMU and is changed to 0 in
	 * ramstage. This means that before ramstage 0 actually points to
	 * 0x8000000. */
	RAMSTAGE(0x9005b000, 2M)
	STACK(0x90280000, 1536K)
}
