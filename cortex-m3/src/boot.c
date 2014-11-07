/*****************************************************************************
 * boot.c - Main C source for Cortex-M3 BSPs.
 *
 * This file implements functionality to set up peripherals on a Cortex-M3
 * based board, and hand over to the kernel initialization routine. The
 * boot_rtos function is the only entrypoint in this file.
 *
 * TODO: Move implementation of init_clocks and init_GPIO to STM32
 * subdirectory.
 *
 *****************************************************************************/

#include "soc_arch_registers.h"
#include "soc_family_registers.h"
#include "kernel.h"
#include "system_config.h"

#if (!defined (SYSTEM_SOC_RUN_FROM_ROM) && \
     !defined (SYSTEM_SOC_RUN_FROM_RAM)) || \
  (defined (SYSTEM_SOC_RUN_FROM_ROM) && defined(SYSTEM_SOC_RUN_FROM_RAM))
#error "Specify ONE of SYSTEM_SOC_RUN_FROM_RAM or SYSTEM_SOC_RUN_FROM_ROM!"
#endif

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

/* Variables defined in the linked command file. Used for clearing the BSS
   and to copy initialized data from flash to RAM. */
extern unsigned long _bss_start;
extern unsigned long _bss_end;

#ifdef SYSTEM_SOC_RUN_FROM_ROM
extern unsigned long _data_start;
extern unsigned long _data_end;
extern unsigned long _data_init;
#endif

/*****************************************************************************
 * External Function Declarations
 *****************************************************************************/

extern void rtos_init();
extern void handler_mode_boot();

/*****************************************************************************
 * Local Function Prototypes
 *****************************************************************************/

/* Set up the systick timer to generate SysTick exceptions at regular
   intervals. It does NOT start the timer. */
static void init_systick_timer();

/* Exception handler for the SysTick exception. It calls the kernel tick
   function. */
static void handler_systick();

/*****************************************************************************
 * Local Function Implementations
 *****************************************************************************/

static void init_systick_timer()
{
   /* See TRM 8.2.2. */

   /* Set up the exception handler. */
   *VECTOR_SYSTICK = (unsigned long) handler_systick;

   /* Set up CLKSOURCE and TICKINT.
      Timer is driven by core clock and will trigger the SysTick
      exception (but timer is not yet enabled). */
   *SYST_CSR = 0x6;

   /* Set up the RELOAD value, i.e. the value that will be loaded
      into the timer when the counter reaches 0. We would like a 1000 Hz
      clock. */
   *SYST_RVR = 72000;
}

static void handler_systick()
{
   /* 0xE000E200 is the Interrupt Set-Pending register. Bit 15 is SysTick.
      Counter value is at 0xE000E018 (bits 0-23). */
   rtos_tick();
}

/*****************************************************************************
 * Global Function Implementations
 *****************************************************************************/

void soc_start_hook(void)
{
  /* Enable the SysTick timer. */
  *SYST_CSR |= 0x1;

  /* Enable family-specific peripherals. */
  family_start_hook();

  /* Enable on-board peripherals. */
  board_start_hook();
}

void boot_rtos()
{
   /* We enter here, running as privileged in thread mode. TRM 2.2.
      We use SP_main. TRM 2.2.1 and 5.4.
      NVIC interrupts disabled. NMI and Hard Fault disabled. TRM 5.9. */
   unsigned long *dst;

   /* FIXME: We would like to set up the system clock before copying data and
      initializing the BSS, to make this process quicker. But that means that
      the setting up cannot rely on initialized data. */
#ifdef SYSTEM_SOC_RUN_FROM_ROM
   {
      /* If running from ROM, copy initial Data values from ROM to RAM. */
      unsigned long *src;

      for (src = (unsigned long *) &_data_init,
	      dst = (unsigned long *) &_data_start;
	   dst < (unsigned long *) &_data_end;
	   src++, dst++)
	 *dst = *src;
   }
#endif
   /* Clear BSS. */
   for (dst = (unsigned long *) &_bss_start;
	dst < (unsigned long *) &_bss_end;
	dst++)
      *dst = 0;

   /* Set up the Cortex-M3 systick timer. */
   init_systick_timer();

   /* The family init function should set up system and peripheral clocks. */
   soc_family_init();

#ifdef SYSTEM_SOC_RUN_FROM_RAM
   /* Move NVIC exception table base to SRAM. */
   *VTOR = (1 << 29);
#endif

   /* Set up board components. */
   board_init();

   /* Call portable OS initialization from handler mode.
      This is done by handling the SVC exception with a
      minimal routine that just jumps to 'rtos_init'. */
   *VECTOR_SVC = (unsigned long) handler_mode_boot;

   asm("svc\t0");
}
