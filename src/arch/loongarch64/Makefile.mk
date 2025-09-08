## SPDX-License-Identifier: GPL-2.0-only

loongarch64_flags = -I$(src)/arch/loongarch64/ -mabi=lp64d -march=loongarch64

loongarch64_asm_flags =

################################################################################
## bootblock
################################################################################
ifeq ($(CONFIG_ARCH_BOOTBLOCK_LOONGARCH64),y)

bootblock-y += bootblock.S
bootblock-y += lib/memory/mem_copy.c
bootblock-y += lib/fpu/init_fpu.S
bootblock-y += lib/exception/rom_exception.S
bootblock-y += lib/exception/init_interrupts.S
bootblock-y += lib/exception/exception_entry.S
bootblock-y += lib/exception/exception_handler.c
bootblock-y += arch_timer.c
bootblock-y += boot.c
bootblock-y += \
	$(top)/src/lib/memchr.c \
	$(top)/src/lib/memcmp.c \
	$(top)/src/lib/memcpy.c \
	$(top)/src/lib/memmove.c \
	$(top)/src/lib/memset.c

# Build the bootblock

$(eval $(call link_stage,bootblock))
$(eval $(call link_stage,decompressor))

endif

################################################################################
## romstage
################################################################################
ifeq ($(CONFIG_ARCH_ROMSTAGE_LOONGARCH64),y)

romstage-y += arch_timer.c
romstage-y += boot.c
romstage-y += romstage.c
romstage-y += stages.c
romstage-y += \
	$(top)/src/lib/memchr.c \
	$(top)/src/lib/memcmp.c \
	$(top)/src/lib/memcpy.c \
	$(top)/src/lib/memmove.c \
	$(top)/src/lib/memset.c

romstage-$(CONFIG_COLLECT_TIMESTAMPS) += timestamp.c

romstage-srcs += $(wildcard $(src)/mainboard/$(MAINBOARDDIR)/romstage.c)

# Build the romstage

romstage-c-ccopts += $(loongarch64_flags)
romstage-S-ccopts += $(loongarch64_asm_flags)

$(eval $(call link_stage,romstage))
endif

################################################################################
## ramstage
################################################################################
ifeq ($(CONFIG_ARCH_RAMSTAGE_LOONGARCH64),y)

ramstage-y += stages.c
ramstage-y += arch_timer.c
ramstage-y += boot.c
ramstage-y += tables.c
ramstage-y += \
	$(top)/src/lib/memchr.c \
	$(top)/src/lib/memcmp.c \
	$(top)/src/lib/memcpy.c \
	$(top)/src/lib/memmove.c \
	$(top)/src/lib/memset.c

$(eval $(call create_class_compiler,rmodules,loongarch64))

ramstage-$(CONFIG_COLLECT_TIMESTAMPS) += timestamp.c

ramstage-c-ccopts += $(loongarch64_flags)
ramstage-S-ccopts += $(loongarch64_asm_flags)

# Build the ramstage

$(eval $(call link_stage,ramstage))

endif
