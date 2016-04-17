.THUMB				@ turn on thumb
.ALIGN  2			@ align code correctly for GBA
.GLOBL  palettes		@ name of function goes here



.THUMB_FUNC			@ we are about to declare a thumb function
palettes:				@ function start

push { r4-r7, lr }	@ push r4-r7 and link register onto stack. Your function might use these
					@ registers, so we need to preserve the values just in case!
					@ we don't need to worry about r0-r3 as it is assumed they will be regularly messed up anyway
	


@======= palette filling function========================
ldr r0, =#0x05000020	@palette 1 start adress
ldr r1, =#0x5000002E	@palette 1 position 7, 8 colours will be inside this palette

start:
cmp r0, r1
bne colour0



compare2:
cmp r0,r1
bne colour1

compare3:
cmp r0,r1
bne colour2

compare4:
cmp r0,r1
bne colour3

compare5:
cmp r0,r1
bne colour4

compare6:
cmp r0,r1
bne colour5

compare7:
cmp r0,r1
bne colour6

compare8:
cmp r0,r1
bne colour7

colour0:
	ldr r4, =#0x253F
	b store

colour1:
	ldr r4 ,=#0x1CF8
	b store2

colour2: 
	ldr r4 ,=#0x253A
	b store3

colour3: 
	ldr r4 ,=#0x2D7B
	b store4

colour4: 
	ldr r4 ,=#0x35BC
	b store5

colour5: 
	ldr r4 ,=#0x39DD 
	b store6

colour6: 
	ldr r4 ,=#0x3DFE
	b store7

colour7: 
	ldr r4 ,=#0x084C
	b store8

store:
strh r4, [r0]	@strh for 16bit halfword
add r0, #2
b compare2

store2:
strh r4, [r0]	@strh for 16bit halfword
add r0, #2
bne compare3

store3:
strh r4, [r0]	@strh for 16bit halfword
add r0, #2
bne compare4

store4:
strh r4, [r0]	@strh for 16bit halfword
add r0, #2
bne compare5

store5:
strh r4, [r0]	@strh for 16bit halfword
add r0, #2
bne compare6

store6:
strh r4, [r0]	@strh for 16bit halfword
add r0, #2
bne compare7

store7:
strh r4, [r0]	@strh for 16bit halfword
add r0, #2
bne compare8

store8:
strh r4, [r0]	@strh for 16bit halfword
add r0, #2

pop { r4-r7 }		@ pop first 4 values from stack back into r4-r7, and also
pop { r3 }			@ pop the next value from stack (stored value for lr) into some unused register, e.g. r3 -
					@ we cannot overwrite lr so we have to do it via a normal register
bx r3				@ "branch and exchange" (return) back to C, using the previous value for lr stored in r3
