/* SPDX-License-Identifier: GPL-2.0-only */

#include <cpu/cpu.h>
#include <device/mmio.h>
#include <smbios.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <console/console.h>

/* Read PRID from CPUCFG register 0 */
static uint32_t loongarch_cpucfg(int reg)
{
	uint32_t val;
	__asm__ __volatile__("cpucfg %0, %1" : "=&r"(val) : "r"(reg));
	return val;
}

static void smbios_processor_id(u32 *processor_id)
{
	processor_id[0] = loongarch_cpucfg(0);
	processor_id[1] = 0;
}

static int smbios_processor_manufacturer(u8 *start)
{
	return smbios_add_string(start, "Loongson");
}


static int smbios_processor_version(u8 *start)
{
	char buf[48];
	snprintf(buf, sizeof(buf), "Loongson (LoongArch) rev %#x",
		 loongarch_cpucfg(0));
	return smbios_add_string(start, buf);
}

int smbios_write_type4(unsigned long *current, int handle)
{
	static unsigned int cnt = 0;
	char buf[8];
	struct smbios_type4 *t = smbios_carve_table(*current,
					SMBIOS_PROCESSOR_INFORMATION,
					sizeof(*t), handle);

	snprintf(buf, sizeof(buf), "CPU%d", cnt++);
	t->socket_designation = smbios_add_string(t->eos, buf);

	smbios_processor_id(t->processor_id);
	t->processor_manufacturer = smbios_processor_manufacturer(t->eos);
	t->processor_version = smbios_processor_version(t->eos);
	t->processor_family = SMBIOS_PROCESSOR_FAMILY_OTHER;
	t->processor_type = SMBIOS_PROCESSOR_TYPE_CENTRAL;

	/* Use u16 fields for the actual count; u8 fields capped at 255. */
	unsigned int ncpus = CONFIG_MAX_CPUS;
	if (ncpus > 255) ncpus = 255;
	t->core_count  = (u8)ncpus;
	t->thread_count = (u8)ncpus;
	t->core_enabled = (u8)ncpus;
	t->core_count2  = CONFIG_MAX_CPUS;
	t->thread_count2 = CONFIG_MAX_CPUS;

	t->l1_cache_handle = 0xffff;
	t->l2_cache_handle = 0xffff;
	t->l3_cache_handle = 0xffff;

	t->serial_number = smbios_add_string(t->eos,
				smbios_processor_serial_number());
	t->status = SMBIOS_PROCESSOR_STATUS_CPU_ENABLED |
		    SMBIOS_PROCESSOR_STATUS_POPULATED;
	t->processor_upgrade = PROCESSOR_UPGRADE_UNKNOWN;

	t->external_clock = 0;
	t->current_speed = smbios_cpu_get_current_speed_mhz();
	t->max_speed = smbios_cpu_get_max_speed_mhz();

	t->processor_characteristics = PROCESSOR_64BIT_CAPABLE;
	if (t->core_count > 1)
		t->processor_characteristics |= PROCESSOR_MULTI_CORE;

	const int len = smbios_full_table_len(&t->header, t->eos);
	*current += len;
	return len;
}

int smbios_write_type7_cache_parameters(unsigned long *current,
					int *handle, int *max_struct_size,
					struct smbios_type4 *type4)
{
	/* No cache information available — skip */
	return 0;
}
