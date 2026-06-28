## SPDX-License-Identifier: GPL-2.0-only

bootblock-y += mainboard.c
bootblock-y += uart.c
bootblock-y += rom_media.c

romstage-y += cbmem.c
romstage-y += romstage.c
romstage-y += uart.c
romstage-y += rom_media.c

ramstage-y += ramstage.c
ramstage-y += rom_media.c

CPPFLAGS_common += -I$(src)/mainboard/$(MAINBOARDDIR)/include
