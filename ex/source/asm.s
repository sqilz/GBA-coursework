.THUMB				@ turn on thumb
.ALIGN  2			@ align code correctly for GBA
.GLOBL  PlotPixels		@ name of function goes here



.THUMB_FUNC			@ we are about to declare a thumb function
PlotPixels:				@ function start

push { r4-r7, lr }	@ push r4-r7 and link register onto stack. Your function might use these
					@ registers, so we need to preserve the values just in case!
					@ we don't need to worry about r0-r3 as it is assumed they will be regularly messed up anyway
	

@ the magic happens here!
@ r0-r3 will automatically contain the parameters sent when calling the function.


@	ldr r4, [r0]		@ load integer x from .c file into register4
@	ldr r5, [r1]		@ load iterations
@	again:				@label to jump to
@	cmp r4, r5			@ compare r4 and r5
@	bne start			@if branch not equal jump to start label
@	b store				@else jump to store
@
@	start:
@		add r4, #1		@ add 1 to r4
@		b again			@ jump to again/ another iteration start
@	store:
@		str r4,[r0]			@send register value back to variable in .c
		
	
	
	@ldr r4, #0x5000000			@Frame 1 in MODE_4 of DSPCNT that will hold menu image. Not using Frame 0 as transitions between menu and game 
	
	
	
	

pop { r4-r7 }		@ pop first 4 values from stack back into r4-r7, and also
pop { r3 }			@ pop the next value from stack (stored value for lr) into some unused register, e.g. r3 -
					@ we cannot overwrite lr so we have to do it via a normal register
bx r3				@ "branch and exchange" (return) back to C, using the previous value for lr stored in r3
@ ==================================
