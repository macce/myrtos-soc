/*****************************************************************************
 * spi.c - C source for STM32 SPI driver.
 *
 * This file implements functionality to set up and handle the SPI interface
 * of the STM32 family.
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

static rtos_u32 sending_pid = 0;
static rtos_u8 rx_buf[16];

/*****************************************************************************
 * External Function Declarations
 *****************************************************************************/


/*****************************************************************************
 * Local Function Prototypes
 *****************************************************************************/

#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI1_USED)
/* Set up SPI1. */
static void init_SPI1();
/* Exception handler for the DMA1 CH2 interrupt. */
static void handler_DMA1_CH2();
/* Exception handler for the DMA1 CH3 interrupt. */
static void handler_DMA1_CH3();
#endif

#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI2_USED)
/* Set up SPI12. */
static void init_SPI2();
/* Exception handler for the DMA1 CH4 interrupt. */
static void handler_DMA1_CH4();
/* Exception handler for the DMA1 CH5 interrupt. */
static void handler_DMA1_CH5();
#endif

/*****************************************************************************
 * Local Function Implementations
 *****************************************************************************/
#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI1_USED)
static void init_SPI1()
{
  /* Set up the PA7 pin for alternate function push/pull. This is the
     MOSI pin for SPI1. PA6 is set up for floating input mode. PA6 is
     the MISO pin for SPI1. PA5 = SCK. PA4 = NSS.
     FIXME: Pin ids should not be hardcoded here, they depend on the specific
     device. */
  *STM32_GPIOA_CRL = (*STM32_GPIOA_CRL & 0x0000FFFF) |
    (2 << 30) | /* CNF7  - Alternate function output push/pull. */
    (2 << 28) | /* MODE7 - Output, max speed 2 MHz. */
    (1 << 26) | /* CNF6  - Floating input. */
    (0 << 24) | /* MODE6 - Input mode. */
    (2 << 22) | /* CNF5  - Alternate function output push/pull. */
    (2 << 20) | /* MODE5 - Output, max speed 2 MHz. */
    (2 << 18) | /* CNF4  - Alternate function output push/pull. */
    (2 << 16);  /* MODE4 - Output, max speed 2 MHz. */

  /* SPI1 uses APB2 and PCLK2 (72 MHz). */
  *STM32_SPI1_CR1 =
    (0x7 << 3) | /* BR - Baudrate = 72 MHz / 256 ~= 282 kbps. */
    (0x1 << 2) | /* MSTR - Configured as master. */
    (0x1 << 1) | /* CPOL - Clock to 1 when idle. */
    (0x1 << 0);  /* CPHA - Clock phase - capture on second transition. */

  *STM32_SPI1_CR2 =
    (0x1 << 2);  /* SSOE - Enable slave select in master mode. */

  /* Set up DMA peripheral register address for RX. */
  *STM32_DMA1_CPAR2 = (rtos_u32) STM32_SPI1_DR;

  /* Set up DMA peripheral register address for TX. */
  *STM32_DMA1_CPAR3 = (rtos_u32) STM32_SPI1_DR;

  /* Set up DMA1 channel 2 configuration register. */
  *STM32_DMA1_CCR2 =
    (1 << 7) | /* MINC - Memory Increment Mode enabled. */
    (0 << 4) | /* DIR - Data transfer TO memory. */
    (1 << 1) | /* TCIE - Transfer Complete Interrupt enabled. */
    (1 << 0);  /* EN - Channel enabled. */

  /* Set up DMA1 channel 3 configuration register. */
  *STM32_DMA1_CCR3 =
    (1 << 7) | /* MINC - Memory Increment Mode enabled. */
    (1 << 4) | /* DIR - Data transfer FROM memory. */
    (1 << 1);  /* TCIE - Transfer Complete Interrupt enabled. */

  /* Set up the exception vector for DMA1 channel 2 (used by SPI1 RX). */
  *VECTOR_DMA1_CH2 = (unsigned long) handler_DMA1_CH2;

  /* Set up the exception vector for DMA1 channel 3 (used by SPI1 TX). */
  *VECTOR_DMA1_CH3 = (unsigned long) handler_DMA1_CH3;
}
#endif

#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI2_USED)
static void init_SPI2()
{
  /* Set up the PB15 pin for alternate function push/pull. This is the
     MOSI pin for SPI2. PB14 is set up for floating input mode. PB14 is
     the MISO pin for SPI2. PB13 = SCK PB12=NSS. */
  *STM32_GPIOB_CRH = (*STM32_GPIOB_CRH & 0x0000FFFF) |
    (2 << 30) | /* CNF15  - Alternate function output push/pull. */
    (2 << 28) | /* MODE15 - Output, max speed 2 MHz.  */
    (1 << 26) | /* CNF14  - Floating input. */
    (0 << 24) | /* MODE14 - Input mode. */
    (2 << 22) | /* CNF13  - Alternate function output push/pull. */
    (2 << 20) | /* MODE13 - Output, max 2 MHz. */
    (2 << 18) | /* CNF12  - Alternate function output push/pull. */
    (2 << 16);  /* MODE12 - Output max 2 MHz. */

  /* SPI2 uses APB1 and PCLK1 (36 MHz). */
  *STM32_SPI2_CR1 =
    (0x7 << 3) | /* BR - Baudrate = 36 MHz / 256 ~= 141 kbps. */
    (0x1 << 2) | /* MSTR - Configured as master. */
    (0x1 << 1) | /* CPOL - Clock to 1 when idle. */
    (0x1 << 0);  /* CPHA - Clock phase - capture on second transition. */

  *STM32_SPI2_CR2 =
    (0x1 << 2);  /* SSOE - Enable slave select in master mode. */

  /* Set up DMA peripheral register address for RX. */
  *STM32_DMA1_CPAR4 = (rtos_u32) STM32_SPI2_DR;

  /* Set up DMA peripheral register address for TX. */
  *STM32_DMA1_CPAR5 = (rtos_u32) STM32_SPI2_DR;

  /* Set up DMA1 channel 4 configuration register. */
  *STM32_DMA1_CCR4 =
    (1 << 7) | /* MINC - Memory Increment Mode enabled. */
    (0 << 4) | /* DIR - Data transfer TO memory. */
    (1 << 1) | /* TCIE - Transfer Complete Interrupt enabled. */
    (1 << 0);  /* EN - Channel enabled. */

  /* Set up DMA1 channel 5 configuration register. */
  *STM32_DMA1_CCR5 =
    (1 << 7) | /* MINC - Memory Increment Mode enabled. */
    (1 << 4) | /* DIR - Data transfer FROM memory. */
    (1 << 1);  /* TCIE - Transfer Complete Interrupt enabled. */

  /* Set up the exception vector for DMA1 channel 4 (used by SPI2 RX). */
  *VECTOR_DMA1_CH4 = (unsigned long) handler_DMA1_CH4;

  /* Set up the exception vector for DMA1 channel 5 (used by SPI2 TX). */
  *VECTOR_DMA1_CH5 = (unsigned long) handler_DMA1_CH5;
}
#endif

#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI1_USED)
static void handler_DMA1_CH2()
{
  /* Clear all interrupt flags for DMA1, channel 2. */
  *STM32_DMA1_IFCR = (1 << 4);

  /* Disable DMA1 channel 2. Necessary before reprogramming 'CNDTR2'
     for the next transfer. */
  *STM32_DMA1_CCR2 &= ~(1 << 0); /* EN - Channel disabled. */
}

static void handler_DMA1_CH3()
{
  /* Clear all interrupt flags for DMA1, channel 3. */
  *STM32_DMA1_IFCR = (1 << 8);

  /* Disable DMA1 channel 3. Necessary before reprogramming 'CNDTR3'
     for the next transfer. */
  *STM32_DMA1_CCR3 &= ~(1 << 0); /* EN - Channel disabled. */

  /* Signal the process semaphore for the process that triggered the SPI
     transfer. */
  rtos_signal_psem(sending_pid);
}
#endif

#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI2_USED)
static void handler_DMA1_CH4()
{
  /* Clear all interrupt flags for DMA1, channel 4. */
  *STM32_DMA1_IFCR = (1 << 12);

  /* Disable DMA1 channel 4. Necessary before reprogramming 'CNDTR4'
     for the next transfer. */
  *STM32_DMA1_CCR4 &= ~(1 << 0); /* EN - Channel disabled. */
}

static void handler_DMA1_CH5()
{
  /* Clear all interrupt flags for DMA1, channel 5. */
  *STM32_DMA1_IFCR = (1 << 16);

  /* Disable DMA1 channel 5. Necessary before reprogramming 'CNDTR5'
     for the next transfer. */
  *STM32_DMA1_CCR5 &= ~(1 << 0); /* EN - Channel disabled. */

  /* Signal the process semaphore for the process that triggered the SPI
     transfer. */
  rtos_signal_psem(sending_pid);
}
#endif

/*****************************************************************************
 * Global Function Implementations
 *****************************************************************************/

void init_SPI()
{
#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI1_USED)
  init_SPI1();
#endif

#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI2_USED)
  init_SPI2();
#endif
}

void SPI_start_hook()
{
#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI1_USED)
  /* Enable SPI1. */
  *STM32_SPI1_CR1 |=
    (0x1 << 6); /* SPE - SPI Enable. */

  /* Enable interrupt 12 and 13 (DMA1 CH2 and CH3) in the NVIC. */
  *NVIC_ISER0 |=
    (1 << 13) |
    (1 << 12);
#endif

#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI2_USED)
  /* Enable SPI2. */
  *STM32_SPI2_CR1 |=
    (0x1 << 6); /* SPE - SPI Enable. */

  /* Enable interrupt 14 and 15 (DMA1 CH4 and CH5) in the NVIC. */
  *NVIC_ISER0 |=
    (1 << 15) |
    (1 << 14);
#endif
}

#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI1_USED)
void SPI1_send(rtos_u8 *data, rtos_u32 count)
{
  /* SPI1 TX uses channel 3 on DMA1. */
  sending_pid = rtos_current_pid();

  /* Set up DMA memory address. */
  *STM32_DMA1_CMAR2 = (rtos_u32) rx_buf;

  /* Set up DMA number of data register. */
  *STM32_DMA1_CNDTR2 = count;

  /* Set up DMA memory address. */
  *STM32_DMA1_CMAR3 = (rtos_u32) data;

  /* Set up DMA number of data register. */
  *STM32_DMA1_CNDTR3 = count;

  /* Enable DMA1 channel 2. */
  *STM32_DMA1_CCR2 |=
    (1 << 0);  /* EN - Enable channel. */

  /* Enable DMA1 channel 3. */
  *STM32_DMA1_CCR3 |=
    (1 << 0);  /* EN - Enable channel. */

  /* Enable DMA transfers for SPI1. This will trigger the DMA transfer
     from memory to SPI1 TX. */
  *STM32_SPI1_CR2 |=
    (0x1 << 1) | /* TXDMAEN - Make DMA request whenever TXE is true. */
    (0x0 << 0);  /* RXDMAEN - Make DMA request whenever RXNE is true. */

  rtos_wait_psem();
}
#endif

#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI2_USED)
void SPI2_send(rtos_u8 *data, rtos_u32 count)
{
  /* SPI2 TX uses channel 5 on DMA1. */
  sending_pid = rtos_current_pid();

  /* Set up DMA memory address. */
  *STM32_DMA1_CMAR4 = (rtos_u32) rx_buf;

  /* Set up DMA number of data register. */
  *STM32_DMA1_CNDTR4 = count;

  /* Set up DMA memory address. */
  *STM32_DMA1_CMAR5 = (rtos_u32) data;

  /* Set up DMA number of data register. */
  *STM32_DMA1_CNDTR5 = count;

  /* Enable DMA1 channel 4. */
  *STM32_DMA1_CCR4 |=
    (1 << 0);  /* EN - Enable channel. */

  /* Enable DMA1 channel 5. */
  *STM32_DMA1_CCR5 |=
    (1 << 0);  /* EN - Enable channel. */

  /* Enable DMA transfers for SPI2. This will trigger the DMA transfer
     from memory to SPI2 TX. */
  *STM32_SPI2_CR2 |=
    (0x1 << 1) | /* TXDMAEN - Make DMA request whenever TXE is true. */
    (0x0 << 0);  /* RXDMAEN - Make DMA request whenever RXNE is true. */

  rtos_wait_psem();
}
#endif
