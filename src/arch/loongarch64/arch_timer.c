#include <delay.h>
#include <timer.h>
#include <csr.h>
#include <csr_func.h>

#define CONSTANT_TIMER_FREQ_HZ	100000000ULL
static uint64_t timer_last_us;

void init_timer(void) { timer_last_us = 0; }

void timer_monotonic_get(struct mono_time *mt)
{
	uint64_t ticks = csr_read(LOONGARCH_CSR_CNTC);
	uint64_t now_us = ticks / (CONSTANT_TIMER_FREQ_HZ / USECS_PER_SEC);
	if (now_us <= timer_last_us) now_us = timer_last_us + 1;
	timer_last_us = now_us;
	mono_time_set_usecs(mt, now_us);
}
