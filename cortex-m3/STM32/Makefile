include ../../config.mk

###############################################################################
# Defines
###############################################################################

INCLUDE_FLAGS := \
	-I$(SYSTEM_CONFIG_DIR) \
	-I$(SOC_INCLUDE_DIR) \
	-I$(SOC_ARCH_INCLUDE_DIR) \
	-Isoc_include \
	-I$(DEVICE)/soc_include \
	-I$(KERNEL_INCLUDE_DIRS)

CFLAGS += $(INCLUDE_FLAGS)
ASFLAGS += $(INCLUDE_FLAGS)

###############################################################################
# Config Generator
###############################################################################

CONF_GEN_FILES := $(foreach file, STM32.rb, $(CONF_GEN_DIR)/$(file))

###############################################################################
# Objects
###############################################################################

OBJECTS := $(foreach object, stm32.o usart.o gpio.o spi.o, \
	$(SOC_OBJ_DIR)/$(object))

###############################################################################
# Rules
###############################################################################

.PHONY:	all conf_gen
all:	$(SOC_OBJ_DIR) $(OBJECTS)

conf_gen:	$(CONF_GEN_DIR) $(CONF_GEN_FILES)

.PHONY: clean
clean:
	rm -f $(OBJECTS)

$(SOC_OBJ_DIR) $(CONF_GEN_DIR):
	mkdir -p $@

$(SOC_OBJ_DIR)/%.o:	src/%.c Makefile
	$(CC) $(CFLAGS) -o$@ $<

$(CONF_GEN_DIR)/%.rb:	conf_gen/%.rb
	cp $< $@
