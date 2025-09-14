/* SPDX-License-Identifier: GPL-2.0-only */

#include <cbmem.h>
#include <romstage.h>
#include <romstage_common.h>
#include <console/console.h>
#include <program_loading.h>

void __noreturn romstage_main(void)
{
	printk(BIOS_DEBUG, "mxd-debug");
	cbmem_initialize_empty();
	run_ramstage();
}
