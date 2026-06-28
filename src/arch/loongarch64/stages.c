/* SPDX-License-Identifier: GPL-2.0-only */

#include <cbmem.h>
#include <arch/stages.h>
#include <halt.h>

void __noreturn stage_entry(uintptr_t stage_arg)
{
	main();
	halt();
}
