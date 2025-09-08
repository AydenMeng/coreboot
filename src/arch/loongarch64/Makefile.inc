## SPDX-License-Identifier: GPL-2.0-only

loongarch64_flags = -I$(src)/arch/loongarch64/ -mabi=lp64d -march=loongarch64

loongarch64_asm_flags =

################################################################################
## bootblock
################################################################################
ifeq ($(CONFIG_ARCH_BOOTBLOCK_LOONGARCH64),y)

bootblock-y += bootblock.S
bootblock-y += romstage.c
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

bootblock-generic-ccopts += $(loongarch64_flags)

$(objcbfs)/bootblock.debug: $$(bootblock-objs)
	@printf "    LINK       $(subst $(obj)/,,$(@))\n"
	$(LD_bootblock) $(LDFLAGS_bootblock) -o $@ -L$(obj) \
		-T $(call src-to-obj,bootblock,$(CONFIG_MEMLAYOUT_LD_FILE)) --whole-archive --start-group $(filter-out %.ld,$(bootblock-objs)) \
		$(LIBGCC_FILE_NAME_bootblock) --end-group $(COMPILER_RT_bootblock)

endif

################################################################################
## romstage
################################################################################
ifeq ($(CONFIG_ARCH_ROMSTAGE_LOONGARCH64),y)

romstage-y += arch_timer.c
romstage-y += boot.c
romstage-y += stages.c
romstage-y += \
	$(top)/src/lib/memchr.c \
	$(top)/src/lib/memcmp.c \
	$(top)/src/lib/memcpy.c \
	$(top)/src/lib/memmove.c \
	$(top)/src/lib/memset.c

romstage-$(CONFIG_COLLECT_TIMESTAMPS) += timestamp.c

# Build the romstage

$(objcbfs)/romstage.debug: $$(romstage-objs)
	@printf "    LINK       $(subst $(obj)/,,$(@))\n"
	$(LD_romstage) $(LDFLAGS_romstage) -o $@ -L$(obj) -T $(call src-to-obj,romstage,$(CONFIG_MEMLAYOUT_LD_FILE)) --whole-archive --start-group $(filter-out %.ld,$(romstage-objs)) --end-group $(COMPILER_RT_romstage)

romstage-c-ccopts += $(loongarch64_flags)
romstage-S-ccopts += $(loongarch64_asm_flags)

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

$(eval $(call create_class_compiler,rmodules,power8))

ramstage-$(CONFIG_COLLECT_TIMESTAMPS) += timestamp.c

ramstage-srcs += src/mainboard/$(MAINBOARDDIR)/mainboard.c

# Build the ramstage

$(objcbfs)/ramstage.debug: $$(ramstage-objs)
	@printf "    CC         $(subst $(obj)/,,$(@))\n"
	$(LD_ramstage) $(LDFLAGS_ramstage) -o $@ -L$(obj) -T $(call src-to-obj,ramstage,$(CONFIG_MEMLAYOUT_LD_FILE)) --whole-archive --start-group $(filter-out %.ld,$(ramstage-objs)) --end-group $(COMPILER_RT_ramstage)

ramstage-c-ccopts += $(loongarch64_flags)
ramstage-S-ccopts += $(loongarch64_asm_flags)

endif
