/*
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 2310-001 Assignment 3
 */
 
    .text
    .global my_encrypt
    .type my_encrypt, %function

/* encryption routine
 *
 *  input parameters:
 *    r0 - address of original string
 *        (assumption: string ends in null byte)
 *    r1 - address of output string
 *    r2 - address of key string
 *		  (assumption: string ends in null byte)
 *    r3 - encryption switch. 0 = encrypt, 1 = decrypt
 *
 *  return value:
 *    none
 *
 *  other output parameters:
 *    r1 - address of output string
 *
 *  effect/output:
 *    use private key encryption to either encrypt or decrypt the original 
 *    string and place it at the address given in the output string parameter.
 *  
 *  typical calling sequence
 *    ldr r0, =str
 *    ldr r1, =out_str
 *    ldr r2, =key_str
 *    ldr r3, {switch}
 *    bl my_encrypt
 *  local register usage:
 *    r4 - char of str
 *    r5 - char of out_str
 *    r6 - char of key_str / value for key char
 *	  r7 - offset in str/out_str
 *	  r8 - offset in key_str
 */ 

my_encrypt:
    push	{r4, r5, r6, r7, r8, lr}

    mov		r7, #0						@ Initialize offset to 0.
    mov		r8, #0						@ Initialize offset to 0.

    cmp 	r3, #0						@ Check if we are encrypting or not.
    beq		.encrypt

.dec_loop:
    ldrb	r4, [r0, r7]				@ Load char of str into r4.
    ldrb	r6, [r2, r8]				@ Load char of key into r6.

    cmp		r4, #0						@ Check for NULL character in str
    beq		.done						@ If NULL character, then we're done.

    cmp		r6, #0						@ Check for NULL character in key.
    bgt		.dec_key_good				@ If not NULL, continue.
	ldrb	r6, [r2]					@ Load in the first char of key.
	mov		r8, #0						@ Reset key offset to zero.			

.dec_key_good:
	sub		r6, r6, #0x60				/* Subtract 0x60 to get the value of the
    									 * key letter. a=1, b=2, etc.
    									 */
    cmp		r6, #0						@ Non-alpha characters are not valid in
    bgt		.dec_key_val_good			@ the key so set their value to zero.
    mov		r6, #0

.dec_key_val_good:
	sub		r4, r4, #0x60				/* Subtract 0x60 to get the value of the
    									 * str letter.
    									 */
    cmp		r4, #0						@ Non-alpha characters do not get
                      					@ decoded.
    blt		.dec_no_decode_whitespace

    sub		r5, r4, r6					@ Take difference of values.
    cmp		r5, #0						@ Check for negative.
    bge		.dec_out_str_val_good
    add		r5, r5, #26					@ If negative, add back 26.
    bal     .dec_out_str_val_good


.dec_no_decode_whitespace:
    mov     r5, r4
.dec_out_str_val_good:
	add		r5, r5, #0x60				@ Get back to ASCII characters.
	strb	r5, [r1, r7]				@ Store the decoded char in str_out
	add		r7, r7, #1					@ Increment index of str/out_str
	add		r8, r8, #1					@ Increment index of key
	bal		.dec_loop					@ Move on to the next character.



.encrypt:
.enc_loop:
    ldrb	r4, [r0, r7]				@ Load char of str into r4.
    ldrb	r6, [r2, r8]				@ Load char of key into r6.

    cmp		r4, #0						@ Check for NULL character in str
    beq		.done						@ If NULL character, then we're done.

    cmp		r6, #0						@ Check for NULL character in key.
    bgt		.enc_key_good				@ If not NULL, continue.
	ldrb	r6, [r2]					@ Load in the first char of key.
	mov		r8, #0						@ Reset key offset to zero.			

.enc_key_good:
	sub		r6, r6, #0x60				/* Subtract 0x60 to get the value of the
    									 * key letter. a=1, b=2, etc.
    									 */
    cmp		r6, #0						@ Non-alpha characters are not valid in
    bgt		.enc_key_val_good			@ the key so set their value to zero.
    mov		r6, #0

.enc_key_val_good:
	sub		r4, r4, #0x60				/* Subtract 0x60 to get the value of the
    									 * str letter.
    									 */
    cmp		r4, #0						@ Non-alpha characters do not get
                      					@ decoded.
    blt		.enc_no_encode_whitespace

    add		r5, r4, r6					@ Add the values together
    cmp		r5, #26						@ Check for "overflow"
    ble		.enc_out_str_val_good
    sub		r5, r5, #26					@ If "overflow", subtract 26 to get back
    bal     .enc_out_str_val_good

.enc_no_encode_whitespace:
    mov     r5, r4
.enc_out_str_val_good:
	add		r5, r5, #0x60				@ Get back to ASCII characters.
	strb	r5, [r1, r7]				@ Store the decoded char in str_out
	add		r7, r7, #1					@ Increment index of str/out_str
	add		r8, r8, #1					@ Increment index of key
	bal		.enc_loop					@ Move on to the next character.

.done:
	mov		r5, #0
	strb	r5, [r1, r7]				/* Make sure we have to NULL char on the
										 * end of the out string.
										 */
	pop    	{r4, r5, r6, r7, r8, lr}    /* Put back everything the way we found 
										 * it. 
										 */  
    bx		lr							@ Time to go home.
