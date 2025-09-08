/* SPDX-License-Identifier: GPL-2.0-only */

#include <cbmem.h>
#include <romstage.h>
#include <romstage_common.h>

void __noreturn romstage_main(void)
{
	cbmem_recovery(0);
	while(1);
}
