@@@
@@@ boot_asm.S - Implementation of assembly routines for Cortex-M3 BSPs.
@@@

@@ Code to be generated for the thumb-2 instruction set.
	.syntax	unified
	.thumb

@@@ Code to .text segment:
	.section	.text


@@@ 
@@@ Function:	handler_mode_boot
@@@   Exception handler for the first SVC (syscall) exception.
@@@   The first SVC is issued from the boot code to change to handler mode.
@@@   This handler then calls rtos_init in the portable part to initialize
@@@   the OS. The initialization must be done from handler mode as we start
@@@   the first process by returning to it from handler mode. This is the
@@@   standard way of starting the first process in Cortex-M3.
@@@
@@@ Parameters:
@@@   None.
@@@ 
	.global	handler_mode_boot
	.thumb_func
	.extern	rtos_init
handler_mode_boot:
	bl	rtos_init			@ Initialize OS.
