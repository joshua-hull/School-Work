/*
 * This program executes a WHILE loop for values less than or equal to 17
 *
 * The C code would look like this:
 *
 * main()
 * {
 *	int a, b, c;
 *	a = 0;
 *      b = 1;
 *	c = 0;
 *
 *	while (a <= 17)
 *	{
 *		a = a + b;
 *		c++;
 *	}
 *
 *      printf("A = %d, B = %d, C = %d\n", a, b, c);
 * }
 */

	.global main

main:
	push {r7, lr}
	sub sp, sp, #16
	add r7, sp, #0

/* initialize the registers */
/* %l0 holds the value of A, %l1 holds the value of B, etc */
	
	mov	r4, #0		    /* put zero in A */
	mov r5, #1		    /* put one in B  */
	mov r6, #0		    /* put zero in C */

test:
	cmp r4, #17	 	    /* compare A and 17 */
	bgt done			/* branch if greater than 0 to "done" */
	add r4, r4, r5	    /* A = A + B */
	add r6, r6, #1	    /* C = C + 1 */
	bal test		    /* branch always to "test" */

done:
    movw	r8, #:lower16:out_line
    movt	r8, #:upper16:out_line
    mov 	r0, r8
    mov 	r1, r4
    mov 	r2, r5
    mov 	r3, r6
	bl	    printf

    mov 	r3, #0
	mov 	r0, r3
	add 	r7, r7, #16
	mov 	sp, r7
    pop	{r7, pc}
	.size 	main, .-main

	.section .rodata
out_line: .asciz "A = %d, B = %d, C = %d\n"
						
