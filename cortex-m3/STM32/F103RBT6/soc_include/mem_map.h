/*****************************************************************************
 * mem_map.h - Input to linker script, BSP and kernel for the F103RBT6
 *   implementation.
 *
 * TODO: Data such as START_STACK_SIZE and KERNEL_DATA_SIZE should not be
 *   defined here, as these value are not strictly related to the specific
 *   device. Maybe they should be put in some top-level "build config"
 *   directory?
 *
 *****************************************************************************/

#ifndef MEM_MAP_H
#define MEM_MAP_H

#include "system_config.h"

#if (!defined (SYSTEM_SOC_RUN_FROM_RAM) || defined (SYSTEM_SOC_RUN_FROM_ROM))
#error "Only SYSTEM_SOC_RUN_FROM_RAM supported!"
#endif

/* Input to linker script. */
#define SRAM_START        0x20000000
#define SRAM_SIZE             0x5000
#define FLASH_START        0x8000000
#define FLASH_SIZE           0x20000
#define VECTORS_SIZE           0x100

#define START_STACK_SIZE       0x400
#define KERNEL_DATA_SIZE      0x1000

/* Derived addresses: */
#define VECTORS_START                                (SRAM_START)
#define START_STACK_START          (VECTORS_START + VECTORS_SIZE)
#define START_STACK_TOP    (START_STACK_START + START_STACK_SIZE)
#define KERNEL_DATA_START  (START_STACK_START + START_STACK_SIZE)
#define MEMORY_START       (KERNEL_DATA_START + KERNEL_DATA_SIZE)
#define MEMORY_SIZE       (SRAM_START + SRAM_SIZE - MEMORY_START)

#endif
