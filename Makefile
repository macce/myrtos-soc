include config.mk

###############################################################################
# Defines
###############################################################################

# Include files to install.
INCLUDES := $(wildcard include/*)

# Place to install soc stuff.
OBJ_INSTALL_PATH := $(SYSTEM_ROOT)/soc/obj
LINK_CMD_INSTALL_PATH := $(SYSTEM_ROOT)/soc/link_cmd
INCLUDE_INSTALL_PATH := $(SYSTEM_ROOT)/soc/include

###############################################################################
# Rules
###############################################################################

.PHONY:	all conf_gen
all conf_gen:
	make -C $(ARCH) $@

.PHONY: install_include_dir
install_include_dir:
	install -d $(INCLUDE_INSTALL_PATH)

.PHONY:	install
install:	install_include_dir $(INCLUDES)
	for inc in $(INCLUDES); do \
		install -m 644 $$inc $(INCLUDE_INSTALL_PATH); \
	done
	make -C cortex-m3 install

.PHONY:	clean
clean:
	rm -rf $(SOC_OBJ_DIR)

.PHONY:	distclean
distclean:
	rm -rf $(SOC_OBJ_ROOT)
