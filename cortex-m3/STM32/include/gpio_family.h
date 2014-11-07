/*****************************************************************************
 * gpio_family.h - STM32 specific constants for GPIO driver.
 *
 *****************************************************************************/

#ifndef GPIO_FAMILY_H
#define GPIO_FAMILY_H

#include "rtos_types.h"

/*****************************************************************************
 * Defines, Constants, Typedefs and Structs
 *****************************************************************************/

/* Use these constants for the 'port_id' argument. */
#define GPIO_PORTID_PA 0
#define GPIO_PORTID_PB 1
#define GPIO_PORTID_PC 2
#define GPIO_PORTID_PD 3
#define GPIO_PORTID_PE 4
#define GPIO_PORTID_PF 5
#define GPIO_PORTID_PG 6

#endif
