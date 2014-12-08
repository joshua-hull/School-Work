	.global	fact
fact:
	push  {r1, lr}        @ Copy these values to the stack so that they can restored at the end of the function. 

	cmp   r0, #1          @	If i > 1; The factorial of 1 is defined as being 1. 1! = 1
	bne  .L2              @	If i > 1 then branch to L2.
	b    .L3              @ Else branch to L3.

.L2:
	mov  r1, r0           @ r1 = r0; Copy i to r1 so we can store it on the recursive call and decrement r0.
	sub  r0, r0, #1       @ r0 -= 1; Decrement i. 
	bl   fact	      	  @ Call the function fact with a decremented r0. This is the recursive call. n! = n * (n-1)!

	mul  r0, r1, r0       @ r0 *= r1; Have a running multiple in r0 and multiply r1 to the running multiple.

.L3:
	pop  {r1, pc}         @ Pop r1 and lr back to r1 and the PC. This goes back to line 14 until we get back to the original call and then goes back to 36. Through this loop r1 increments back up to 10.
	.size    fact, .-fact

	.align   2
	.section .rodata
.LC0:
	.ascii   "The factorial of %d is %d\012\000"

	.text
	.align   2
	.global  main
main:
	push {lr}             @ save the link register so we can jump back to it after this function is over

	mov  r0, #10          @ store the number we are taking the factorial of into r0 which will be the argument to function "fact"
	mov  r3, r0           @ copy it to r3 since r0 will be overwritten with the return value from "fact"

	bl   fact             @ call function "fact" to take the factorial of 10 which is in r0

	mov  r2, r0           @ move the return value of "fact" which is in r0 into r2 which is the third argument to printf (notice we do this before we overwrite r0 with the print string address two lines down)
	mov  r1, r3           @ move r3, which has the original number we were taking the factorial of (10) into r1 which is the second argument to printf
	ldr  r0, =.LC0        @ copy the address of the print string into r0, the first argument of printf
	bl   printf           @ branch to the printf function to print the answer

	mov  r0, #0           @ move 0 into r0 which is the return value of function main

	pop  {pc}             @ pop the top of the stack (which should be the value of the "lr" register that we pushed in the beginning of main) and store the popped value into register pc. This has the effect of making us branch back to the old value of "lr" from before main was called.

	.size    main, .-main
	.ident   "GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section .note.GNU-stack,"",%progbits
