	.global	fact
fact:
	mov   r1, #1       @ r1 = 1; Initialize the running multiple. The factorial of 1 is defined as 1. 1! = 1

.L3:
	cmp   r0, #1       @ i > 1; The check for our iterative loop. r0 is the number we are getting the factorial of, or the argument passed to our function.
	beq   .L2          @ if !(i > 1) then; Branch to L2 if we are done with our loop.

	mul   r1, r0, r1   @ else r1 *= r0; Have r1 be our running multiple and multiply r0 to it. 
	sub   r0, r0, #1   @ i--; Decrement the index for our iterative loop.
	b     .L3          @ Branch back the begining of our loop.

.L2:
	mov   r0, r1       @ r0 = r1; Copy the resutl of our loop, the factorial, to the expected return value register, r0.

	bx    lr

	.size	fact, .-fact

	.align   2
	.section .rodata
.LC0:
	.ascii  "The factorial of %d is %d\012\000"

	.text
	.align  2
	.global main
main:
	push  {r3, lr}

	mov   r0, #10
	bl    fact

	mov   r2, r0
	mov   r1, #10
	ldr   r0, =.LC0
	bl    printf

	mov   r0, #0

	pop  {r3, pc}

	.size    main, .-main
	.ident   "GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section .note.GNU-stack,"",%progbits
