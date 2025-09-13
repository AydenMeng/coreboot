/* SPDX-License-Identifier: GPL-2.0-only */

#include <memlayout.h>

#include <arch/header.ld>

SECTIONS
{
	FMAP_CACHE(0x00000000, 2K)
	CBFS_MCACHE(0x00000800, 8K)

	TIMESTAMP(0x00002800, 1K)

	CBFS_CACHE(0x00003000, 8K)

	PRERAM_CBMEM_CONSOLE(0x00005000, 128K)

	STACK(0x00025000, 64K)     /* 栈 */
	//HEAP(0x00030000, 64K)       /* 堆 */

	RAMSTAGE(0x00040000, 2M)    /* 起始于 0x90040000，避开 CAR 使用区 */
	/* BOOTBLOCK：位于 Flash 起始位置 */
	BOOTBLOCK(0x1c000000, 32K)

	ROMSTAGE(0x1c008000, 128K)


}
