	.global fib
fib:
    push   {r1, r2, lr} @ you should always save the link register in any function that calls another function since that would cause the link register to be overwritten and you would lose your return address. We push r1 and r2 since they hold the last two elements of the fib sequence for the calling function and we don't want to mess those up.

	cmp    r0, #0
	bne    not_zero     @ if the argument is not 0 (k != 0), then we can skip the following

    @ if the argument is 0 (k == 0), than we just return 0 since the 0th element of the fibonacci sequence is 0 by definition
	mov    r0, #0
	pop    {r1, r2, pc} @ remember to pop anything we previously pushed. And in this case, pop the link register to the pc so we immediately branch to it (lr was our return address)

not_zero:

	cmp    r0, #1
	bne    not_one      @ now we check if the argument is not 1 (k != 1) and skip the following if so

    @ if the argument was one (k == 1), then by definition element 1 of the fibonacci sequence (once again element 1 in a zero based sequence) is simply 1 so we return 1
	mov    r0, #1
	pop    {r1, r2, pc}

not_one:

    @ if the argument is not 0 or 1 (the else statement in the C code) than we cannot instantly return its fibonacci value, we will have to find the fibonacci value for the current number (or element) by summing the two previous fibonacci elements

	mov    r1, r0      @ we copy r0 into r1 since we will need the value of r0 later and r0 is going to be changed when we call fib again

	@ what we do below is call fib on the values r1 - 1 and r1 - 2 and sum them together to get the fibonacci number for r1

	sub    r0, r1, #1  @ calculate r1 - 1 and store it in r0 to be the argument to fib
	bl     fib         @ recursively call fib

	mov    r2, r0      @ move the return value into r2 so it's not messed up by the next call to fib

	sub    r0, r1, #2  @ now we do the same thing as above for r1 - 2
	bl     fib

	add    r0, r2, r0  @ remember we saved the return value of the first call to fib in r2 so we add it to r0 which is the return value of the second call and store the result back into r0 to be the return value of the current function

	@ we just performed fib(r1 - 1) + fib(r1 - 2) which gives us fib(r1) to pass back up to any calling function
	@ since we copied r0 into r1 on line 25, we effectively found fib(r0) which is the fibonacci value for the number that was passed into this fib call to begin with

	@ restore register r1 and r2, and pop the link register's value straight to the pc to branch to the return address
	@ we're not arbitrarily choosing to save r1 and r2 onto the stack, if you look at lines 30 and 35 where we branch to "fib" again, we currently have stuff in the registers r1 and r2 that we don't want to mess up so to fix that we have the next call to fib save those registers for us.
	pop    {r1, r2, pc}

test:
	push   {r4, lr}

	@ call fib on given number
	mov    r4, r0
	bl     fib

	@ print the answer
	mov    r1, r4
	mov    r2, r0
	ldr    r0, =print_string
	bl     printf

	pop    {r4, pc}

	.global main
main:
	push   {lr}

	@ call 4 test cases
@	mov    r0, #0
@	bl     test

@	mov    r0, #1
@	bl     test

@	mov    r0, #6
@	bl     test

@	mov    r0, #10
@	bl     test

    mov    r0, #40
    bl     test
	pop    {pc}

print_string:
	.ascii "Element %d (zero-based) in the Fibonacci sequence is %d\012\000"
