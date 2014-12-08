box_init:
    push  {fp}              @ Save our frame pointer.
    
    sub   sp, sp, #12       @ Allocate the space a local version of the box
                            @ struct.

    mov   fp, sp            @ Refrence this local copy.
    str   r1, [fp]          @ local.l = l_arg;
    str   r2, [fp, #4]      @ local.w - w_arg;
    str   r3, [fp, #8]      @ local.h = h_arg;

    ldmia fp, {r1, r2, r3}
    stmia r0, {r1, r2, r3}  @ Copy this local version into the return one.

    add   fp, fp, #12       @ Dealloc local version.
    mov   sp, fp

    pop   {fp}              @ Restore the frame pointer.

    bx    lr                @ E.T. go home...
    
init:
	push  {fp}
	
	sub   sp, sp, #8
	mov   fp, sp

	str   r1, [fp]
	str   r2, [fp, #4]

	ldmia fp, {r1, r2}
	stmia r0, {r1, r2}

	add   fp, fp, #8
	mov   sp, fp

	pop   {fp}

	bx    lr

zero:
	push  {fp}

	sub   sp, sp, #8
	mov   fp, sp

	mov   r3, #0
	str   r3, [fp]
	str   r3, [fp, #4]

	ldmia fp, {r1, r2}
	stmia r0, {r1, r2}

	add   fp, fp, #8
	mov   sp, fp

	pop   {fp}

	bx    lr

	.global main
main:
	push   {fp, lr}

	sub    sp, sp, #8
    sub    sp, sp, #12      @ Allocate additional space for the box struct.
	mov    fp, sp

	mov    r0, fp
	mov    r1, #34
	mov    r2, #-3
	bl     init

	movw   r0, #:lower16:.LC0
	movt   r0, #:upper16:.LC0
	ldr    r1, [fp]
	ldr    r2, [fp, #4]
	bl     printf

	mov    r0, fp
	bl     zero

	movw   r0, #:lower16:.LC0
	movt   r0, #:upper16:.LC0
	ldr    r1, [fp]
	ldr    r2, [fp, #4]
	bl     printf

    add    fp, fp, #8
    mov    r0, fp            @ We are using a different local variable.
    mov    r1, #45          @ arg1 = 45
    mov    r2, #17          @ arg2 = 17
    mov    r3, #3           @ arg3 = 3
    bl     box_init         @ box_init(arg1, arg2, arg3);

                            @ We use a new format string.
    movw   r0, #:lower16:.LC1
    movt   r0, #:upper16:.LC1
    ldr    r1, [fp]     @ Copy the values of the return structure
    ldr    r2, [fp, #4]    @ so that we can print them.
    ldr    r3, [fp, #8]
    bl     printf           @ Print the structure.

	add    sp, sp, #8
    add    sp, sp, #12      @ Deallocate the box struct.

	pop    {fp, pc}

.LC0:
	.ascii "The members of the structure dot are %d, %d\012\000"
	
.LC1:
    .ascii "The memebers of the structure box are %d, %d, %d\012\000"
