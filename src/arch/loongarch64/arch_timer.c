/* SPDX-License-Identifier: GPL-2.0-only */
/* LoongArch64 timer - uses a loop-based udelay from src/lib/timer.c */
#include <delay.h>
#include <timer.h>
 #include <cpu/cpu.h>
 #include <stdint.h>

#define CONSTANT_TIMER_FREQ_HZ  100000000ULL
static uint64_t timer_last_us;

void init_timer(void) { timer_last_us = 0; }

void timer_monotonic_get(struct mono_time *mt)
{
 /*
  * Read the constant-frequency counter (100 MHz on QEMU virt).
  * rdtime.d returns the tick count in rd and CSR_TID in rj.
  */
 	uint64_t ticks;
 	uint64_t cpuid;
 	__asm__ __volatile__("rdtime.d %0, %1" : "=r"(ticks), "=r"(cpuid));
	uint64_t now_us = ticks / (CONSTANT_TIMER_FREQ_HZ / USECS_PER_SEC);
	if (now_us <= timer_last_us) now_us = timer_last_us + 1;
	timer_last_us = now_us;
	mono_time_set_usecs(mt, now_us);
}
