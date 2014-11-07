/*****************************************************************************
 * soc_family_registers.h - Generic register defines for STM32
 *   implementations.
 *
 *****************************************************************************/

#ifndef SOC_FAMILY_REGISTERS_H
#define SOC_FAMILY_REGISTERS_H

/* General Purpose IO (GPIO) */
#define STM32_GPIOA_BASE   ((volatile unsigned long *) 0x40010800)
#define STM32_GPIOB_BASE   ((volatile unsigned long *) 0x40010C00)
#define STM32_GPIOC_BASE   ((volatile unsigned long *) 0x40011000)
#define STM32_GPIOD_BASE   ((volatile unsigned long *) 0x40011400)
#define STM32_GPIOE_BASE   ((volatile unsigned long *) 0x40011800)
#define STM32_GPIOF_BASE   ((volatile unsigned long *) 0x40011C00)
#define STM32_GPIOG_BASE   ((volatile unsigned long *) 0x40012000)

#define STM32_GPIOA_CRL    ((volatile unsigned long *) 0x40010800)
#define STM32_GPIOA_CRH    ((volatile unsigned long *) 0x40010804)
#define STM32_GPIOA_IDR    ((volatile unsigned long *) 0x40010808)
#define STM32_GPIOA_ODR    ((volatile unsigned long *) 0x4001080C)
#define STM32_GPIOA_BSRR   ((volatile unsigned long *) 0x40010810)
#define STM32_GPIOA_BRR    ((volatile unsigned long *) 0x40010814)
#define STM32_GPIOA_LCKR   ((volatile unsigned long *) 0x40010818)

#define STM32_GPIOB_CRL    ((volatile unsigned long *) 0x40010C00)
#define STM32_GPIOB_CRH    ((volatile unsigned long *) 0x40010C04)
#define STM32_GPIOB_IDR    ((volatile unsigned long *) 0x40010C08)
#define STM32_GPIOB_ODR    ((volatile unsigned long *) 0x40010C0C)
#define STM32_GPIOB_BSRR   ((volatile unsigned long *) 0x40010C10)
#define STM32_GPIOB_BRR    ((volatile unsigned long *) 0x40010C14)
#define STM32_GPIOB_LCKR   ((volatile unsigned long *) 0x40010C18)

#define STM32_GPIOC_CRL    ((volatile unsigned long *) 0x40011000)
#define STM32_GPIOC_CRH    ((volatile unsigned long *) 0x40011004)
#define STM32_GPIOC_IDR    ((volatile unsigned long *) 0x40011008)
#define STM32_GPIOC_ODR    ((volatile unsigned long *) 0x4001100C)
#define STM32_GPIOC_BSRR   ((volatile unsigned long *) 0x40011010)
#define STM32_GPIOC_BRR    ((volatile unsigned long *) 0x40011014)
#define STM32_GPIOC_LCKR   ((volatile unsigned long *) 0x40011018)

#define STM32_GPIOD_CRL    ((volatile unsigned long *) 0x40011400)
#define STM32_GPIOD_CRH    ((volatile unsigned long *) 0x40011404)
#define STM32_GPIOD_IDR    ((volatile unsigned long *) 0x40011408)
#define STM32_GPIOD_ODR    ((volatile unsigned long *) 0x4001140C)
#define STM32_GPIOD_BSRR   ((volatile unsigned long *) 0x40011410)
#define STM32_GPIOD_BRR    ((volatile unsigned long *) 0x40011414)
#define STM32_GPIOD_LCKR   ((volatile unsigned long *) 0x40011418)

#define STM32_GPIOE_CRL    ((volatile unsigned long *) 0x40011800)
#define STM32_GPIOE_CRH    ((volatile unsigned long *) 0x40011804)
#define STM32_GPIOE_IDR    ((volatile unsigned long *) 0x40011808)
#define STM32_GPIOE_ODR    ((volatile unsigned long *) 0x4001180C)
#define STM32_GPIOE_BSRR   ((volatile unsigned long *) 0x40011810)
#define STM32_GPIOE_BRR    ((volatile unsigned long *) 0x40011814)
#define STM32_GPIOE_LCKR   ((volatile unsigned long *) 0x40011818)

#define STM32_GPIOF_CRL    ((volatile unsigned long *) 0x40011C00)
#define STM32_GPIOF_CRH    ((volatile unsigned long *) 0x40011C04)
#define STM32_GPIOF_IDR    ((volatile unsigned long *) 0x40011C08)
#define STM32_GPIOF_ODR    ((volatile unsigned long *) 0x40011C0C)
#define STM32_GPIOF_BSRR   ((volatile unsigned long *) 0x40011C10)
#define STM32_GPIOF_BRR    ((volatile unsigned long *) 0x40011C14)
#define STM32_GPIOF_LCKR   ((volatile unsigned long *) 0x40011C18)

#define STM32_GPIOG_CRL    ((volatile unsigned long *) 0x40012000)
#define STM32_GPIOG_CRH    ((volatile unsigned long *) 0x40012004)
#define STM32_GPIOG_IDR    ((volatile unsigned long *) 0x40012008)
#define STM32_GPIOG_ODR    ((volatile unsigned long *) 0x4001200C)
#define STM32_GPIOG_BSRR   ((volatile unsigned long *) 0x40012010)
#define STM32_GPIOG_BRR    ((volatile unsigned long *) 0x40012014)
#define STM32_GPIOG_LCKR   ((volatile unsigned long *) 0x40012018)

/* Reset and Clock Control (RCC) */
#define STM32_RCC_CR       ((volatile unsigned long *) 0x40021000)
#define STM32_RCC_CFGR     ((volatile unsigned long *) 0x40021004)
#define STM32_RCC_APB2RSTR ((volatile unsigned long *) 0x4002100C)
#define STM32_RCC_AHBENR   ((volatile unsigned long *) 0x40021014)
#define STM32_RCC_APB2ENR  ((volatile unsigned long *) 0x40021018)
#define STM32_RCC_APB1ENR  ((volatile unsigned long *) 0x4002101C)

/* Universal Synchronous Asynchronous Receiver Transmitter (USART) */
#define STM32_USART2_SR    ((volatile unsigned long *) 0x40004400)
#define STM32_USART2_DR    ((volatile unsigned long *) 0x40004404)
#define STM32_USART2_BRR   ((volatile unsigned long *) 0x40004408)
#define STM32_USART2_CR1   ((volatile unsigned long *) 0x4000440C)
#define STM32_USART2_CR2   ((volatile unsigned long *) 0x40004410)
#define STM32_USART2_CR3   ((volatile unsigned long *) 0x40004414)
#define STM32_USART2_GTPR  ((volatile unsigned long *) 0x40004418)

/* Serial Peripherial Interface (SPI) */
#define STM32_SPI3_BASE    ((volatile unsigned long *) 0x40003C00)
#define STM32_SPI2_BASE    ((volatile unsigned long *) 0x40003800)
#define STM32_SPI1_BASE    ((volatile unsigned long *) 0x40013000)

#define STM32_OFFSET_SPI_CR1     0x00
#define STM32_OFFSET_SPI_CR2     0x04
#define STM32_OFFSET_SPI_SR      0x08
#define STM32_OFFSET_SPI_DR      0x0C
#define STM32_OFFSET_SPI_CRCPR   0x10
#define STM32_OFFSET_SPI_RXCRCR  0x14
#define STM32_OFFSET_SPI_TXCRCR  0x18
#define STM32_OFFSET_SPI_I2SCFGR 0x1C
#define STM32_OFFSET_SPI_I2SPR   0x20

#define STM32_SPI2_CR1     ((volatile unsigned long *) 0x40003800)
#define STM32_SPI2_CR2     ((volatile unsigned long *) 0x40003804)
#define STM32_SPI2_SR      ((volatile unsigned long *) 0x40003808)
#define STM32_SPI2_DR      ((volatile unsigned long *) 0x4000380C)
#define STM32_SPI2_CRCPR   ((volatile unsigned long *) 0x40003810)
#define STM32_SPI2_RXCRCR  ((volatile unsigned long *) 0x40003814)
#define STM32_SPI2_TXCRCR  ((volatile unsigned long *) 0x40003818)
#define STM32_SPI2_I2SCFGR ((volatile unsigned long *) 0x4000381C)
#define STM32_SPI2_I2SPR   ((volatile unsigned long *) 0x40003820)

/* DMA. */
#define STM32_DMA1_ISR    ((volatile unsigned long *) 0x40020000)
#define STM32_DMA1_IFCR   ((volatile unsigned long *) 0x40020004)

#define STM32_DMA1_CCR1   ((volatile unsigned long *) 0x40020008)
#define STM32_DMA1_CNDTR1 ((volatile unsigned long *) 0x4002000C)
#define STM32_DMA1_CPAR1  ((volatile unsigned long *) 0x40020010)
#define STM32_DMA1_CMAR1  ((volatile unsigned long *) 0x40020014)

#define STM32_DMA1_CCR4   ((volatile unsigned long *) 0x40020044)
#define STM32_DMA1_CNDTR4 ((volatile unsigned long *) 0x40020048)
#define STM32_DMA1_CPAR4  ((volatile unsigned long *) 0x4002004C)
#define STM32_DMA1_CMAR4  ((volatile unsigned long *) 0x40020050)

#define STM32_DMA1_CCR5   ((volatile unsigned long *) 0x40020058)
#define STM32_DMA1_CNDTR5 ((volatile unsigned long *) 0x4002005C)
#define STM32_DMA1_CPAR5  ((volatile unsigned long *) 0x40020060)
#define STM32_DMA1_CMAR5  ((volatile unsigned long *) 0x40020064)

/* Addresses to exception vectors in RAM. */
#define VECTOR_DMA1_CH4    ((volatile unsigned long *) 0x20000078)
#define VECTOR_DMA1_CH5    ((volatile unsigned long *) 0x2000007C)
#define VECTOR_USART2      ((volatile unsigned long *) 0x200000D8)

#endif
