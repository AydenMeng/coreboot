/* SPDX-License-Identifier: GPL-2.0-only */

/*
 * LoongArch64 timer implementation using the stable counter (CNTC).
 *
 * On the QEMU virt machine the constant timer runs at 100 MHz,
 * giving a tick period of 10 ns.  The stable counter value is
 * read from CSR 0x43 (LOONGARCH_CSR_CNTC).
 */

#include <delay.h>
#include <timer.h>
#include <csr_func.h>

/* QEMU constant timer runs at 100 MHz (10 ns period) */
#define CONSTANT_TIMER_FREQ_HZ	100000000ULL

void init_timer(void)
{
	/* The constant timer counter runs from power-on reset with no
	 * configuration needed on QEMU virt.  Nothing to do here. */
}

void timer_monotonic_get(struct mono_time *mt)
{
	uint64_t ticks = csr_read(LOONGARCH_CSR_CNTC);
	mono_time_set_usecs(mt, ticks / (CONSTANT_TIMER_FREQ_HZ / USECS_PER_SEC));
}
