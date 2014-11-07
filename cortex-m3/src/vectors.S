@@@
@@@ vectors.S - Initial vectors necessary to initialize the BSP.
@@@

@@ Include mem_map.h to get START_STACK_TOP. 
#include "mem_map.h"

@@ Code to be generated for the thumb-2 instruction set.
	.syntax	unified
	.thumb

@@@ Code to .vectors segment:
	.section        .vectors,"aw"

@@@ 
@@@ The exception vector table.
@@@ 
	.global	CM3_vectors
	.extern	boot_rtos
CM3_vectors:
        .word   START_STACK_TOP
        .word   boot_rtos
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
	.word	0
	.word	0
	.word	0
