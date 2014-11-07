/*****************************************************************************
 * gpio_soc.h - SoC interface for GPIO driver.
 *
 *****************************************************************************/

#ifndef GPIO_SOC_H
#define GPIO_SOC_H

/*****************************************************************************
 * Interface towards boot code.
 *****************************************************************************/

/* Configure used GPIO ports, including pins and interrupt handlers.
   Do NOT enable interrupts. */
void init_GPIO();

/* Enable all necessary interrupts for used GPIO ports. */
void GPIO_start_hook();

#endif
