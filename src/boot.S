//Lots of info in: DAI0527A_baremetal_boot_code_for_ARMv8_A_processors.pdf
//we initially use the bottom of memory below the kernel image for the EL3 stack
#define INITIAL_STACK     0x80000

.section ".text.boot"

.globl _start
_start:
	mrs	x0, mpidr_el1		
	and	x0, x0,#0xFF		// Check processor id
	cbz	x0, master		// Hang for all non-primary CPU
  //fall through
proc_hang: 
  wfi
	b 	proc_hang

master:
  //zero out BSS
	adr	x0, bss_begin
	adr	x1, bss_end
	sub	x1, x1, x0
	bl 	memzero

  //init the stack
	mov	sp, #INITIAL_STACK 
	bl	main
	b 	proc_hang		// should never come here
