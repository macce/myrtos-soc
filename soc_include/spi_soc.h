/*****************************************************************************
 * spi_soc.h - SoC interface for SPI driver.
 *
 *****************************************************************************/

#ifndef SPI_SOCK_H
#define SPI_SOCK_H

/*****************************************************************************
 * Interface towards boot code.
 *****************************************************************************/

/* Configure used SPI ports, including pins and interrupt handlers.
   Do NOT enable interrupts. */
void init_SPI();

/* Enable all necessary interrupts for used SPI ports. */
void SPI_start_hook();

#endif
