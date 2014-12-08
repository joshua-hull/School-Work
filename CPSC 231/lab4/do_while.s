/* This program executes a DO loop that computes the triangular number for 4
 *
 * The C code looks like this:
 *
 * main()
 * {
 *		int sum, n;
 *		sum = 0;
 * 	n = 4;
 *
 *		do {
 *			sum = sum + n;
 *			n--;
 *		} while (n >= 0);

 * 	printf("The triangular number for %d is %d\n", 4, sum)
 * }
 */

	.global main

main:
	push 	{r7, lr}
	sub 	sp, sp, #8
	add 	r7, sp, #0
    
	mov	r2, #0		/* sum = zero */
	mov	r3, #4		/* n = 4      */

do_top:
	add	r2, r2, r3	/* sum = sum + n */
	sub r3, r3, #1	/* n--           */

    cmp r3, #0		/* check n against zero */
	bne do_top		/* branch if not zero to "do_top" */

	movw	r4, #:lower16:out_line
	movt	r4, #:upper16:out_line
   mov 	r0, r4
	mov	r1, #4
	bl	printf

 	mov	r3, #0
	mov	r0, r3
	add	r7, r7, #8
	mov 	sp, r7
	pop	{r7, pc}

	.size	main, .-main

	.section  .rodata	
out_line: .asciz "The triangular number for %d is %d\n"
