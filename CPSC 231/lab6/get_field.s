	.global main

main:
	push {r7, lr}
	sub sp, sp, #16
	add r7, sp, #0

	ldr r0, =0x12345678       /* put the source value in r0 */
	ldr r2, =0x7              /* put the location of the field's msb in r2 */
	ldr r3, =0x8              /* put the size of field in r3 */
	
/* Beginning of your code */

    lsr r1, r0, r2
    
    ldr r4, =0x1
    lsl r4, r3
    sub r4, r4, #0x1
    and r1, r1, r4  

/* End of your code */

/* show output on screen */
	movw r0, #:lower16:prt_line
	movt r0, #:upper16:prt_line
	bl printf
	add r7, r7, #16
	mov sp, r7
	pop {r7, pc}

	.size main, .-main
	.section .rodata

prt_line: .asciz "0x%X\n"
