@ Joshua Hull (jhull@clemson.edu)
@ CPSC 231 Lab 11
@ 1.) 2^31-1 or 2147483647
@ 2.) Yes because the the range of an unsigned integer is [0,2^32-1]
@ 3.) 8.48866s for recursive, 0.008s

	.text
	.align	2
fib:
	push    {r4}     @ save any register used besides {r0-r3}

	mov    r1, #0   			@ prevprev = 0
	mov    r2, #1 				@ prev = 1

	mov    r4, #2 				@feb(2) = 2

	mov    r3, #2 				@ i = 2

	cmp    r0, #0 				@ fib(0)
	beq    base_0
	cmp    r0, #1 				@ fib(1)
	beq    base_1

fib_loop:

	add     r3, r2, r1 			@ fib(n) = fib(n-1) + fib(n-2)

	mov     r1, r2  			@ prevprev = prev
	mov     r2, r3 				@ prev = fib(n)

	cmp 	r0, r4       		@ i < n
	beq	    done
	add     r4, r4, #1 			@ i++
	bal     fib_loop

done:
	mov    r0, r3

	pop    {r4}      @ restore any register used besides {r0-r3}
	bx     lr        @ return to caller

base_0:
	mov    r3, #0              	@ fib(0) = 0
	bal    done

base_1:
	mov    r3, #1               @ fib(1) = 1
	bal    done

test:
    push   {r4, lr}

    @ call fib on given number
	mov    r4, r0
	bl     fib

	@ print the answer
	mov    r1, r4
	mov    r2, r0
	ldr    r0, =.LC0
	bl     printf

	pop    {r4, pc}
	
	.global main
main:
	push   {r4, lr}

	@ call 4 test cases
	mov    r0, #0
	bl     test

	mov    r0, #1
	bl     test

	mov    r0, #6
	bl     test

	mov    r0, #10
	bl     test

	pop    {r4,  pc}

	.section .rodata
	.align 2
.LC0:
	.ascii "Element %d (zero-based) in the Fibonacci sequence is %d\012\000"
