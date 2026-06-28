/* SPDX-License-Identifier: GPL-2.0-only */
#include <bootblock_common.h>
#include <console/console.h>

#define UART_THR ((volatile uint8_t *)0x1fe001e0)
static inline void uart_putc(char c) { *UART_THR = c; }

void bootblock_mainboard_early_init(void)
{
	uart_putc('B');
}

void bootblock_mainboard_init(void)
{
	uart_putc('F');
}
