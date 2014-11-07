/*****************************************************************************
 * gpio.h - Application interface for GPIO driver.
 *
 *****************************************************************************/

#ifndef GPIO_H
#define GPIO_H

#include "rtos_types.h"

/*****************************************************************************
 * Interface towards application code.
 *****************************************************************************/

/* For all set bit positions in 'bits' set the corresponding bits on the
   specified GPIO port. For 'port_id', use the constants defined in
   gpio_family.h. */
void GPIO_set_bits(rtos_u32 port_id, rtos_u32 bits);


/* For all set bit positions in 'bits' reset the corresponding bits on the
   specified GPIO port. For 'port_id', use the constants defined in
   gpio_family.h. */
void GPIO_reset_bits(rtos_u32 port_id, rtos_u32 bits);


/* Read the specified port. For 'port_id', use the constants defined in
   gpio_family.h. */
rtos_u32 GPIO_read_bits(rtos_u32 port_id);

#endif
