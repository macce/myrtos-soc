/*****************************************************************************
 * stm32.c - Main C source for the STM32 BSP.
 *
 * This file implements functionality to set up peripherals on an STM32
 * SoC. The init_STM32 function is the only entrypoint in this file.
 *
 *****************************************************************************/

#include "soc_arch_registers.h"
#include "soc_family_registers.h"
#include "kernel.h"
#include "system_config.h"

#ifdef SYSTEM_SOC_CORTEXM3_STM32_USART_USED
#include "usart_soc.h"
#endif

#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_USED
#include "gpio_soc.h"
#endif

#ifdef SYSTEM_SOC_CORTEXM3_STM32_SPI_USED
#include "spi_soc.h"
#endif

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

/* Set up PLL and sysclk. SYSCLK = 72 MHz from an 8 MHz
   external crystal multiplied 9 times by the on-chip PLL. */
static void init_sysclk();

/* Enable all relevant peripheral clocks. */
static void init_peripheral_clocks();

/* Set up exception priorities and grouping. SVC and PendSV will share the
   same, lowest exception priority. That way, they will never preempt
   each other and they will not block interrupt handlers. */
static void init_exception_prio();

/* Set up IO mode, config and output data registers for all ports where
   these registers should have a non-default value. */
static void init_io(void);

/*****************************************************************************
 * Local Function Implementations
 *****************************************************************************/

static void init_sysclk()
{
  /* Out of a system-reset, the board runs from the internal 8 MHz
     RC-oscillator (HSI), which is used directly as SYSCLK. Now, set
     up the board to use the HSE 8 MHz crystal oscillator, fed into
     the PLL, set up to multiply by 9. This creates a SYSCLK of 72 MHz.
     See RM0008 (STM32 Reference Manual), chapter 6.2 and 6.3. */

  /* Set up the PLL source clock and multiplication factor. Set up PCLK1.
     Use the out-of-reset 1:1 prescaler value for the AHB and APB2
     prescalers, making PCLK2 72 MHz. Set up APB1 clock prescaler to 1:2.
     PCLK1 will be 36 MHz. */
  *STM32_RCC_CFGR |=
    (0x7 << 18) | /* PLLMUL - PLL multiplies by 9. */
    (0x1 << 16) | /* PLLSRC - HSE oscillator drives PLL.*/
    (0x4 << 8);   /* PPRE1  - PCLK1 is HCLK / 2. */

  /* Activate the HSE clock. Wait for it to become stable. */
  *STM32_RCC_CR |= (0x1 << 16);  /* HSEON - HSE on. */
  while (((*STM32_RCC_CR) & (0x1 << 17)) == 0);

  /* Activate the PLL. Wait for it to become stable. */
  *STM32_RCC_CR |= (0x1 << 24); /* PLLON - PLL on. */
  while (((*STM32_RCC_CR) & (0x1 << 25)) == 0);

  /* Now switch in the PLL do drive SYSCLK. Wait for switch to
     take place. */
  *STM32_RCC_CFGR |= (0x2 << 0); /* SW - PLL drives SYSCLK. */
  while ((((*STM32_RCC_CFGR) >> 2) & 0x3) != 0x2);
}

static void init_peripheral_clocks()
{
  /* Enable relevant peripheral clocks. TODO: Can be optimized by writing
     several bits to the control register at once. */

  /* Enable DMA1 clock. */
#if defined(SYSTEM_SOC_CORTEXM3_STM32_DMA1_USED)
  *STM32_RCC_AHBENR |= (1 << 0); /* DMA1EN - Enable DMA1 clock. */
#endif

  /* First, enable PCLK2 to required GPIO ports. */
#if defined(SYSTEM_SOC_CORTEXM3_STM32_USART_USART2_USED) ||	\
  defined(SYSTEM_SOC_CORTEXM3_STM32_GPIO_PA_USED) ||		\
  defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI1_USED)
  /* Enable PCLK2 (GPIOA is used by USART2 and SPI1). */
  *STM32_RCC_APB2ENR |= (1 << 2); /* IOPAEN - Enable GPIOA clock. */
#endif

#if defined(SYSTEM_SOC_CORTEXM3_STM32_GPIO_PB_USED) ||  \
  defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI2_USED)
  /* First, PCLK2 to GPIO port B (GPIOB is used by SPI2). */
  *STM32_RCC_APB2ENR |=
    (0x1 << 3); /* IOPBEN - Enable GPIO port B clock. */
#endif

#if defined(SYSTEM_SOC_CORTEXM3_STM32_GPIO_PC_USED)
  /* First, PCLK2 to GPIO port C (GPIOC is on APB2). */
  *STM32_RCC_APB2ENR |= (0x1 << 4); /* IOPCEN - Enable GPIO port C clock. */
#endif

#if defined(SYSTEM_SOC_CORTEXM3_STM32_USART_USART2_USED)
  /* Enable PCLK1 to USART2 (USART2 is on APB1). */
  *STM32_RCC_APB1ENR |= (0x1 << 17); /* USART2EN - Enable USART2 clock. */
#endif

#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI1_USED)
  /* Enable PCLK2 to SPI1 (SPI1 is on APB2). */
  *STM32_RCC_APB1ENR |= (0x1 << 12); /* SPI1EN - Enable SPI1 clock. */
#endif

#if defined(SYSTEM_SOC_CORTEXM3_STM32_SPI_SPI2_USED)
  /* Enable PCLK1 to SPI2 (SPI2 is on APB1). */
  *STM32_RCC_APB1ENR |= (0x1 << 14); /* SPI2EN - Enable SPI2 clock. */
#endif
}

static void init_exception_prio()
{
  /* Set PRIGROUP = 3, which means that all priority bits control the group
     priority. */
  *AIRCR = (0x05FA << 16) | (0x3 << 8);

  /* Set SVC to group priority to 10 and PendSV to 15 (STM32 only uses 4 bits
     for priority). Set SysTick to priority 14. Note that the 4 uppermost bits
     are used to store the priority. */
  *SHPR2 = (0xA << 28);
  *SHPR3 = (0xF << 20) | (0xE << 28);

#ifdef SYSTEM_SOC_CORTEXM3_STM32_USART_USART2_USED
  /* Set USART2 interrupt priority to 13 (pick a number). USART2 interrupt
     number is 38. */
  *NVIC_IPR9 = (*NVIC_IPR9 & 0xFF00FFFF) | (0xD << 20);
#endif

#ifdef SYSTEM_SOC_CORTEXM3_STM32_DMA1_CH2_USED
  /* Set DMA1 channel 2 interrupt priority to 13. Interrupt number is 12. */
  *NVIC_IPR3 = (*NVIC_IPR3 & 0xFFFFFF00) | (0xD << 4);
 #endif

#ifdef SYSTEM_SOC_CORTEXM3_STM32_DMA1_CH3_USED
  /* Set DMA1 channel 3 interrupt priority to 13. Interrupt number is 13. */
  *NVIC_IPR3 = (*NVIC_IPR3 & 0xFFFF00FF) | (0xD << 12);
#endif

#ifdef SYSTEM_SOC_CORTEXM3_STM32_DMA1_CH4_USED
  /* Set DMA1 channel 4 interrupt priority to 13. Interrupt number is 14. */
  *NVIC_IPR3 = (*NVIC_IPR3 & 0xFF00FFFF) | (0xD << 20);
#endif

#ifdef SYSTEM_SOC_CORTEXM3_STM32_DMA1_CH5_USED
  /* Set DMA1 channel 5 interrupt priority to 13. Interrupt number is 15. */
  *NVIC_IPR3 = (*NVIC_IPR3 & 0x00FFFFFF) | (0xD << 28);
#endif
}

static void init_io(void)
{
  /* All IO pin mode and config registers that are defined in the config
     file should have a non-reset value and must be set. */

  /* Start with the CRL/CRH registers: */
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PA_CRL
  *STM32_GPIOA_CRL = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PA_CRL;
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PA_CRH
  *STM32_GPIOA_CRH = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PA_CRH;
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PB_CRL
  *STM32_GPIOB_CRL = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PB_CRL;
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PB_CRH
  *STM32_GPIOB_CRH = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PB_CRH;
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PC_CRL
  *STM32_GPIOC_CRL = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PC_CRL;
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PC_CRH
  *STM32_GPIOC_CRH = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PC_CRH;
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PD_CRL
  *STM32_GPIOD_CRL = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PD_CRL;
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PD_CRH
  *STM32_GPIOD_CRH = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PD_CRH;
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PE_CRL
  *STM32_GPIOE_CRL = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PE_CRL;
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PE_CRH
  *STM32_GPIOE_CRH = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PE_CRH;
#endif

  /* Next come the ODR registers, which should be set for all ports that
     have at least one of the pins used for GPIO in output mode. */
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PA_ODR
  *STM32_GPIOA_ODR = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PA_ODR;
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PB_ODR
  *STM32_GPIOB_ODR = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PB_ODR;
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PC_ODR
  *STM32_GPIOC_ODR = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PC_ODR;
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PD_ODR
  *STM32_GPIOD_ODR = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PD_ODR;
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_PE_ODR
  *STM32_GPIOE_ODR = SYSTEM_SOC_CORTEXM3_STM32_GPIO_PE_ODR;
#endif
}

/*****************************************************************************
 * Global Function Implementations
 *****************************************************************************/

void family_start_hook()
{
  /* Enable on-chip peripherals. */
#ifdef SYSTEM_SOC_CORTEXM3_STM32_USART_USED
  USART_start_hook();
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_USED
  GPIO_start_hook();
#endif
#ifdef SYSTEM_SOC_CORTEXM3_STM32_SPI_USED
  SPI_start_hook();
#endif
}

void soc_family_init(void)
{
  init_sysclk();
  init_exception_prio();

  /* Enable peripheral clocks. */
  init_peripheral_clocks();

  /* Set up IO pin direction and mode registers. */
  init_io();

#ifdef SYSTEM_SOC_CORTEXM3_STM32_GPIO_USED
  /* Set up GPIO. */
  init_GPIO();
#endif

#ifdef SYSTEM_SOC_CORTEXM3_STM32_USART_USED
  /* Set up USART. */
  init_USART();
#endif

#ifdef SYSTEM_SOC_CORTEXM3_STM32_SPI_USED
  /* Set up SPI. */
  init_SPI();
#endif
}
