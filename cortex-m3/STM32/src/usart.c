/*****************************************************************************
 * usart.c - C source for STM32 USART driver.
 *
 * This file implements functionality to set up and handle the UARTS and
 * USARTS of the STM32 family.
 *
 *****************************************************************************/

#include "rtos_types.h"

#include "soc_arch_registers.h"
#include "soc_family_registers.h"

#include "kernel.h"

/*****************************************************************************
 * Defines, Constants, Typedefs and Structs
 *****************************************************************************/


/*****************************************************************************
 * Variable Declarations
 *****************************************************************************/

static rtos_u8 *transmit_buf = 0;
static rtos_u32 transmit_buf_pos = 0;
static rtos_u8 transmit_bytes_left = 0;
static rtos_u8 *receive_buf = 0;
static rtos_u32 receive_buf_pos = 0;
static rtos_u8 receive_bytes_left = 0;
static rtos_u32 receiver = 0;

static rtos_u32 sending_pid = 0;
static rtos_u32 receiving_pid = 0;

/*****************************************************************************
 * External Function Declarations
 *****************************************************************************/


/*****************************************************************************
 * Local Function Prototypes
 *****************************************************************************/

/* Set up USART2 to handle serial communication at 9600 bps. */
static void init_USART2();

/* Exception handler for the USART2 interrupt. */
static void handler_USART2();

/*****************************************************************************
 * Local Function Implementations
 *****************************************************************************/

static void init_USART2()
{
   /* Set up the PA2 pin for alternate function push/pull. This is the
      TX pin for USART2. PA3 is set up for floating input mode. PA3 is
      the RX pin for USART2. */
   *STM32_GPIOA_CRL = (*STM32_GPIOA_CRL & 0xffff00ff) |
      (2 << 10) | /* CNF2  - Alternate function output push/pull. */
      (2 << 8)  | /* MODE2 - Output, max speed 2 MHz.  */
      (1 << 14) | /* CNF3  - Floating input. */
      (0 << 12);  /* MODE3 - Input mode. */

   /* Set up the Baud Rate Register for 9600 bps. See RM0008, chapter
      24.3.4. The value to be programmed in the BRR should be 234.375.
      DIV_Mantissa=234 DIV_Fraction=6. */
   *STM32_USART2_BRR =
      (234 << 4) | 6; /* 9600 bps */   
   
   /* After-reset default values for other registers are suitable for an
      8-N-1 setup. */

   /* Set up the exception handler. */
   *VECTOR_USART2 = (unsigned long) handler_USART2;
}

static void handler_USART2()
{
   rtos_u32 status = 0;
   rtos_address ready_message = 0;

   /* Check USART2 status register. The possible interrupts are TXE or
      RXNE. */
   status = *STM32_USART2_SR;

   if (status & (1 << 7)) /* Check TXE. */
   {
      /* TXE is true. Start transmission of next byte. This will clear
	 TXE and eventually trigger a new TXE interrupt. */

      if (transmit_bytes_left == 1)
      {
	 /* Disable TXE interrupt BEFORE starting transmission of the last
	    byte. Then send it. */
	 *STM32_USART2_CR1 &= ~(1 << 7); /* TXEIE - TXE int disable. */
	 *STM32_USART2_DR = transmit_buf[transmit_buf_pos];

	 rtos_signal_psem(sending_pid);
      }
      else if (transmit_bytes_left > 1)
      {
	 *STM32_USART2_DR = transmit_buf[transmit_buf_pos++];
	 transmit_bytes_left--;
      }
   }

   if (status & (1 << 5)) /* Check RXNE. */
   {
      /* RXNE is true. Read data byte from data register. This will clear
	 RXNE. */

      if (receive_bytes_left == 1)
      {
	 *STM32_USART2_CR1 &= ~(1 << 5); /* RXNEIE - RXNE int disable. */
	 receive_buf[receive_buf_pos++] = *STM32_USART2_DR;

	 rtos_signal_psem(receiving_pid);
      }
      else
      {
	 receive_buf[receive_buf_pos++] = *STM32_USART2_DR;
	 receive_bytes_left--;
      }
   }
}

/*****************************************************************************
 * Global Function Implementations
 *****************************************************************************/

void init_USART()
{
   init_USART2();
}

void USART_start_hook()
{
   /* Enable USART2 receiver and transmitter. */
   *STM32_USART2_CR1 |=
      (1 << 13) | /* UE - Enable USART. */
      (1 << 3)  | /* TE - Enable transmitter. */
      (1 << 2);   /* RE - Enable receiver. */

   /* Enable interrupt 38 (USART2) in the NVIC. */
   *NVIC_ISER1 |=
      (1 << 6);
}

void serial_send(rtos_u8 *data, rtos_u32 count)
{
   transmit_buf = data;
   transmit_bytes_left = count;
   transmit_buf_pos = 0;

   sending_pid = rtos_current_pid();
   /* Trigger transmission. */
   *STM32_USART2_CR1 |= (1 << 7); /* TXEIE - TXE int enable. */

   rtos_wait_psem();
}

void serial_receive(rtos_u8 *data, rtos_u32 count)
{
   receive_buf = data;
   receive_bytes_left = count;
   receive_buf_pos = 0;

   receiving_pid = rtos_current_pid();
   /* Trigger transmission. */
   *STM32_USART2_CR1 |= (1 << 5); /* RXNEIE - RXNE int enable. */

   rtos_wait_psem();
}
