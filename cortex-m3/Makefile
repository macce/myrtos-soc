include ../config.mk

###############################################################################
# Defines
###############################################################################

INCLUDE_FLAGS := \
	-I$(SYSTEM_CONFIG_DIR) \
	-I$(SOC_INCLUDE_DIR) \
	-Isoc_include \
	-I$(FAMILY)/soc_include \
	-I$(FAMILY)/$(DEVICE)/soc_include \
	-I$(KERNEL_INCLUDE_DIRS)

CFLAGS += $(INCLUDE_FLAGS)
ASFLAGS += $(INCLUDE_FLAGS)
CPPFLAGS += -I$(SYSTEM_CONFIG_DIR)

###############################################################################
# Config Generator
###############################################################################

CONF_GEN_FILES := $(foreach file, cortex_m3.rb, $(CONF_GEN_DIR)/$(file))

###############################################################################
# Objects
###############################################################################

OBJECTS := $(foreach object, boot.o boot_asm.o vectors.o link.cmd, \
	$(SOC_OBJ_DIR)/$(object))

###############################################################################
# Rules
###############################################################################

.PHONY:	all conf_gen
all:	family_all $(SOC_OBJ_DIR) $(OBJECTS)

conf_gen:	$(CONF_GEN_DIR) $(CONF_GEN_FILES) family_conf_gen

.PHONY:	install
install:
	make -C STM32 install

.PHONY: clean
clean:
	make -C $(FAMILY) clean
	rm -f $(OBJECTS)

.PHONY:	family_all
family_all:
	make -C $(FAMILY) all

.PHONY:	family_conf_gen
family_conf_gen:
	make -C $(FAMILY) conf_gen

$(SOC_OBJ_DIR) $(CONF_GEN_DIR):
	mkdir -p $@

$(SOC_OBJ_DIR)/%.o:	src/%.c Makefile
	$(CC) $(CFLAGS) -o $@ $<

$(SOC_OBJ_DIR)/%.o:	src/%.S Makefile
	$(AS) $(ASFLAGS) -o $@ $<

$(SOC_OBJ_DIR)/link.cmd:	\
	$(FAMILY)/$(DEVICE)/soc_include/link.cmd \
	$(FAMILY)/$(DEVICE)/soc_include/mem_map.h \
	Makefile
	$(CPP) $(CPPFLAGS) -o $@ -P $<

$(CONF_GEN_DIR)/%.rb:	conf_gen/%.rb
	cp $< $@
