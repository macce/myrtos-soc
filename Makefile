include config.mk

###############################################################################
# Defines
###############################################################################

# Place to install kernel stuff.
OBJ_INSTALL_PATH := $(SYSTEM_ROOT)/soc/obj
LINK_CMD_INSTALL_PATH := $(SYSTEM_ROOT)/soc/link_cmd
INCLUDE_INSTALL_PATH := $(SYSTEM_ROOT)/soc/include

###############################################################################
# Rules
###############################################################################

.PHONY:	all conf_gen
all conf_gen:
	make -C $(ARCH) $@

.PHONY:	install
install:	$(LIBRARIES)
	install -d $(LIB_INSTALL_PATH) $(INCLUDE_INSTALL_PATH)
	for lib in $(LIBRARIES); do \
		install -m 644 $$lib $(LIB_INSTALL_PATH); \
	done
	for inc in $(INCLUDES); do \
		install -m 644 $$inc $(INCLUDE_INSTALL_PATH); \
	done

.PHONY:	clean
clean:
	rm -rf $(SOC_OBJ_DIR)

.PHONY:	distclean
distclean:
	rm -rf $(SOC_OBJ_ROOT)
