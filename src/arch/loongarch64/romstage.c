/* SPDX-License-Identifier: GPL-2.0-only */

#include <arch/cpu.h>
#include <console/console.h>
#include <timestamp.h>
#include <romstage.h>
#include <romstage_common.h>

void main(void)
{
	timestamp_add_now(TS_ROMSTAGE_START);

	/* Assumes the hardware was set up during the bootblock */
	console_init();

	romstage_main();
}
