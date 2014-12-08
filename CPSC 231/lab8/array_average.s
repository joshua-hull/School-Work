    .syntax unified
    .section .text.startup,"ax",%progbits
    .global main

main:
    push    {r4, r5, lr}
    sub     sp, sp, #68             @ Allocate space on the stack for our array.

    mov     r4, sp
	ldr     r5, =array_values

    ldmia   r5!, {r0, r1, r2, r3}   @ Load the first four values into registers.
    stmia   r4!, {r0, r1, r2, r3}   @ Store those on the stack.
    ldmia   r5!, {r0, r1, r2, r3}   @ Repeat for the second four...
    stmia   r4!, {r0, r1, r2, r3}
    ldmia   r5!, {r0, r1, r2, r3}   @ And the thrid four...
    stmia   r4!, {r0, r1, r2, r3}
    ldmia   r5!, {r0, r1, r2, r3}   @ And finally the last four.
    stmia   r4!, {r0, r1, r2, r3}

    mov     r2, #0                  @ sum = 0              
    mov     r3, #0                  @ i = 0

.loop:
    ldr     r1, [sp, r3]            @ r1 = array_values[i]
    add     r2, r2, r1              @ sum = sum + r1
    add     r3, r3, #4              @ i++

    cmp     r3, #64                 @ for i < 16
    bne     .loop

    lsr     r1, r2, #4              @ average = sum 

    ldr     r0, =.print_string       @ Average already in r1 from above.
    b       printf

    add     sp, sp, #68
    pop     {r4, r5, lr}

    bx      lr
    .size   main, .-main

    .section  .rodata

array_values:
    .word 20
    .word 10
    .word 14
    .word 18
    .word 2
    .word 5
    .word 8
    .word 18
    .word 4
    .word 12
    .word 17
    .word 18
    .word 21
    .word 54
    .word 71
    .word 13

    .section  .rodata.str1.1,"aMS",%progbits,1

.print_string:
    .ascii	"Average = %d\012\000"

.end_program:
	.ident    "GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section  .note.GNU-stack,"",%progbits
