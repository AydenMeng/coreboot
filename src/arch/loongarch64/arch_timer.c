/* SPDX-License-Identifier: GPL-2.0-only */

/*
 * LoongArch64 timer implementation.
 *
 * Uses the stable counter (CSR 0x43, LOONGARCH_CSR_CNTC) which runs at
 * 100 MHz on the QEMU virt machine.  As a fallback to guarantee monotonic
 * growth across rapid consecutive reads (the TCG virtual clock may not
 * advance between two tightly-spaced CSR reads), we keep a software
 * accumulation counter.
 */

#include <delay.h>
#include <timer.h>
#include <csr.h>
#include <csr_func.h>

/* QEMU constant timer runs at 100 MHz (10 ns period) */
#define CONSTANT_TIMER_FREQ_HZ	100000000ULL

/* Software accumulation to ensure monotonic rise even when the hardware
 * counter stalls across consecutive reads inside the same TCG block. */
static uint64_t timer_last_us;

void init_timer(void)
{
	timer_last_us = 0;
}

void timer_monotonic_get(struct mono_time *mt)
{
	uint64_t ticks = csr_read(LOONGARCH_CSR_CNTC);
	uint64_t now_us = ticks / (CONSTANT_TIMER_FREQ_HZ / USECS_PER_SEC);

	/* Never go backwards – the host virtual clock might not tick
	 * between two reads inside the same QEMU translation block. */
	if (now_us <= timer_last_us)
		now_us = timer_last_us + 1;
	timer_last_us = now_us;

	mono_time_set_usecs(mt, now_us);
}
