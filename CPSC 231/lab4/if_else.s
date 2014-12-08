/* This program demonstrates the use of the compare synthetic
 * instruction along with a branch instruction to generate an
 * "if-then-else" programming language construct.
 *
 * The code for this in C is:
 *
 * int main()
 * {
 *		int x, y, z;
 *		x = 1;
 *		y = 2;
 *		z = 3;
 *
 *		if (x == y)
 *	   	z = 1;
 *		else
 * 		z = 0;
 *
 *    printf("Z = %d\n", z);
 * }
 *
 */

	.global main

main:
	push 	{r7, lr}
  	sub	sp, sp, #16
	add 	r7, sp, #0

	mov	r0, #1		/* 1 -> X */
	mov	r1, #2		/* 2 -> Y */
	mov	r2, #3		/* 3 -> Z */

	/* if-then-else comparison */
    cmp r0, r1		 		/* compare X to Y */
	bne else_part			/* branch if not equal to the else part */
	
then_part:
	mov	r2, #1		/* 1 -> Z */
	bal end_if				/* skip beyond else part */

else_part:
	mov	r2, #0		/* 0 -> Z  */

end_if:
   movw 	r3, #:lower16:prt_line        
   movt 	r3, #:upper16:prt_line
	mov  	r0, r3 
   mov	r1, r2
	bl 	printf 

done:
   mov	r3, #0
	mov	r0, r3
	add	r7, r7, #16
	mov   sp, r7
	pop 	{r7, pc}

	.size	main, .-main

	.section .rodata
prt_line: .asciz "Z = %d\n"
