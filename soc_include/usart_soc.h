/*****************************************************************************
 * usart_soc.h - SoC interface for USART driver.
 *
 *****************************************************************************/

#ifndef USART_SOC_H
#define USART_SOC_H

/*****************************************************************************
 * Interface towards boot code.
 *****************************************************************************/

/* Configure used USART ports, including pins and interrupt handlers.
   Do NOT enable interrupts. */
void init_USART();

/* Enable all necessary interrupts for used USART ports. */
void USART_start_hook();

#endif
