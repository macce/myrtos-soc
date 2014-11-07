/*****************************************************************************
 * usart.h - Application interface for USART driver.
 *
 *****************************************************************************/

#ifndef USART_H
#define USART_H

#include "rtos_types.h"

/*****************************************************************************
 * Interface towards application code.
 *****************************************************************************/

/* Send 'count' bytes from the buffer pointed out by 'data'. Block calling
   process until all data has been sent. */
void USART_send(rtos_u8 *data, rtos_u32 count);


/* Receive 'count' bytes to the buffer pointed out by 'data'. Block calling
   process until all data has been read. */
void USART_receive(rtos_u8 *data, rtos_u32 count);

#endif
