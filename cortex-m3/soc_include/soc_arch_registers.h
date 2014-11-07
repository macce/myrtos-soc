/*****************************************************************************
 * soc_arch_registers.h - Generic register defines for Cortex-M3
 *   implementations.
 *
 *****************************************************************************/

#ifndef SOC_ARCH_REGISTERS_H
#define SOC_ARCH_REGISTERS_H

#define SYST_CSR   ((volatile unsigned long *) 0xE000E010)
#define SYST_RVR   ((volatile unsigned long *) 0xE000E014)
#define SYST_CALIB ((volatile unsigned long *) 0xE000E01C)

#define VTOR       ((volatile unsigned long *) 0xE000ED08)
#define AIRCR      ((volatile unsigned long *) 0xE000ED0C)
#define SHPR2      ((volatile unsigned long *) 0xE000ED1C)
#define SHPR3      ((volatile unsigned long *) 0xE000ED20)

#define NVIC_ISER0 ((volatile unsigned long *) 0xE000E100)
#define NVIC_ISER1 ((volatile unsigned long *) 0xE000E104)

#define NVIC_IPR0  ((volatile unsigned long *) 0xE000E400)
#define NVIC_IPR1  ((volatile unsigned long *) 0xE000E404)
#define NVIC_IPR2  ((volatile unsigned long *) 0xE000E408)
#define NVIC_IPR3  ((volatile unsigned long *) 0xE000E40C)
#define NVIC_IPR4  ((volatile unsigned long *) 0xE000E410)
#define NVIC_IPR5  ((volatile unsigned long *) 0xE000E414)
#define NVIC_IPR6  ((volatile unsigned long *) 0xE000E418)
#define NVIC_IPR7  ((volatile unsigned long *) 0xE000E41C)
#define NVIC_IPR8  ((volatile unsigned long *) 0xE000E420)
#define NVIC_IPR9  ((volatile unsigned long *) 0xE000E424)


/* Addresses to exception vectors in RAM. */
#define VECTOR_SVC     ((volatile unsigned long *) 0x2000002C)
#define VECTOR_SYSTICK ((volatile unsigned long *) 0x2000003C)

#endif
