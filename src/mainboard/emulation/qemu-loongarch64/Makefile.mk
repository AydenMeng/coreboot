## SPDX-License-Identifier: GPL-2.0-only

bootblock-y += rom_media.c
bootblock-y += uart.c

romstage-y += cbmem.c
romstage-y += romstage.c
romstage-y += rom_media.c

ramstage-y += ramstage.c
ramstage-y += rom_media.c

CPPFLAGS_common += -I$(src)/mainboard/$(MAINBOARDDIR)/include
