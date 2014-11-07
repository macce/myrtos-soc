/*****************************************************************************
 * gpio.c - C source for STM32 GPIO driver.
 *
 * This file implements functionality to set up and handle the GPIO
 * interfaces of the STM32 family.
 *
 *****************************************************************************/

#include "rtos_types.h"
#include "system_config.h"

#include "soc_arch_registers.h"
#include "soc_family_registers.h"

#include "kernel.h"

/*****************************************************************************
 * Defines, Constants, Typedefs and Structs
 *****************************************************************************/


/*****************************************************************************
 * Variable Declarations
 *****************************************************************************/


/*****************************************************************************
 * External Function Declarations
 *****************************************************************************/


/*****************************************************************************
 * Local Function Prototypes
 *****************************************************************************/


/*****************************************************************************
 * Local Function Implementations
 *****************************************************************************/


/*****************************************************************************
 * Global Function Implementations
 *****************************************************************************/

void init_GPIO()
{
}

void GPIO_start_hook()
{
}

void GPIO_set_bits(rtos_u32 port_id, rtos_u32 bits)
{
   *(STM32_GPIOA_BSRR + (port_id << 8)) = bits & 0x0000FFFF;
}

void GPIO_reset_bits(rtos_u32 port_id, rtos_u32 bits)
{
   *(STM32_GPIOA_BSRR + (port_id << 8)) = (bits & 0x0000FFFF) << 16;
}

rtos_u32 GPIO_read_bits(rtos_u32 port_id)
{
   return *(STM32_GPIOA_IDR + (port_id << 8));
}
