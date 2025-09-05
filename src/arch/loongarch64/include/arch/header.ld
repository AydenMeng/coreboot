/* SPDX-License-Identifier: GPL-2.0-only */

/* We use ELF as output format. So that we can debug the code in some form. */
OUTPUT_FORMAT("elf64-loongarch", "elf64-loongarch", "elf64-loongarch")
OUTPUT_ARCH(loongarch)

PHDRS
{
	to_load PT_LOAD;
}

#if ENV_BOOTBLOCK || ENV_RAMSTAGE
ENTRY(_start)
#else
ENTRY(stage_entry)
#endif
