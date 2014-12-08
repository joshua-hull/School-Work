	.syntax unified
	.section .text.startup,"ax",%progbits
	.global main

main:
	push    {r4, r5, lr}           @ Pop these three register onto the stack.
	sub     sp, sp, #44            @ Allocate 44 Bytes of space to store the array by moving the stack pointer.

	mov     r4, sp                 @ Put the new stack pointer address into r4.
	ldr     r5, =array_values      @ Load r5 with the address of the first value in the array.

	ldmia   r5!, {r0, r1, r2, r3}  @ Load the first four elements in the array into r0, r1, r2, and r3. Increment so r5 points to the 5th element.
	stmia   r4!, {r0, r1, r2, r3}  @ Put the first four elements of the array into the stack.
	ldmia   r5!, {r0, r1, r2, r3}  @ Load the second four elements in the array into r0, r1, r2, and r3. Increment so r5 points to the 9th element.
	stmia   r4!, {r0, r1, r2, r3}  @ Put the second four elements of the array into the stack.
	ldmia   r5, {r0, r1}           @ Load the last two elements in the array into r0 and r1.
	stmia   r4, {r0, r1}           @ Put the last two elements on the stack

	mov     r2, #0                 @ Put the value zero into r2. Sum = 0.
	mov     r3, #0                 @ Put the value zero into r3. i = 0.

.loop:
	ldr     r1, [sp, r3]           @ Load r1 with the value located r3(i) * 8 bytes from sp. Load r1 with value of the array at i.
	add     r2, r2, r1             @ Add r2 and r1 and put them in r2. sum = sum + array[i].
	add     r3, r3, #4             @ Increment i. i++.

	cmp     r3, #40                @ Check to make sure we have not gone past the end of the array. i < 10.
	bne     .loop                  @ If we have not then loop.

	ldr     r0, =.print_string     @ Load r0 with the address of the format string.
	mov     r1, r2                 @ Load sum into r1.
	b       printf                 @ Print the output.

	add     sp, sp, #44            @ Restore the stack to it's original postion.
	pop     {r4, r5, lr}           @ Pop the three registers off the stack and restore them.

	bx      lr
	.size   main, .-main

	.section  .rodata

array_values:
	.word	34
	.word	5
	.word	1
	.word	86
	.word	23
	.word	2
	.word	9
	.word	15
	.word	21
	.word	4

	.section  .rodata.str1.1,"aMS",%progbits,1

.print_string:
	.ascii    "sum = %d\012\000"

.end_program:
	.ident    "GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section  .note.GNU-stack,"",%progbits
