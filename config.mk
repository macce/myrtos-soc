###############################################################################
# RTOS Build Configuration File
#
# This file is included from all Makefiles in the kernel and SoC components.
# It may also be included from other components (such as system). It defines
# a number of make variables that are used to find object files, libraries,
# C-headers etc that are necessary during the build.
#
###############################################################################

# Find out SOC_ROOT:
SOC_ROOT := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

SOC_BUILD_VARIANT := STM32_DEBUG
ARCH := cortex-m3
FAMILY := STM32
DEVICE := F103RBT6
DEBUG := yes # yes/no
OPTIMIZE := no # yes/no

# Define the toolchain to use:
SOC_TOOLCHAIN := codesourcery/arm-2010q1

# Toolchain setup:
include $(SOC_ROOT)/build/build.mk
# Miscellaneous build support:
include $(SOC_ROOT)/build/misc.mk
# Make rules common to many components:
include $(SOC_ROOT)/build/rules.mk

# Static path setup to SoC components:
SOC_INCLUDE_DIR := $(SOC_ROOT)/soc_include
SOC_ARCH_INCLUDE_DIR := $(SOC_ROOT)/$(ARCH)/soc_include
SOC_FAMILY_INCLUDE_DIR := $(SOC_ROOT)/$(ARCH)/$(FAMILY)/soc_include
SOC_OBJ_ROOT := $(SOC_ROOT)/obj
SOC_OBJ_DIR := $(SOC_OBJ_ROOT)/$(SOC_BUILD_VARIANT)

# Static path setup to system components:
KERNEL_INCLUDE_DIRS := $(SYSTEM_ROOT)/kernel/include
SYSTEM_CONFIG_DIR := $(SYSTEM_ROOT)
