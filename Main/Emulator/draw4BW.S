.syntax unified

@ Arguments:
@  r0  pixels
@  r1  palette
@  r2  destination
.section	.text.Draw4BW,"ax",%progbits
//.section .ramcode,"ax",%progbits
.balign 4
.global Draw4BW
.thumb_func
Draw4BW:
      @ parameters
      pix   .req r0
      pal   .req r1
      dst   .req r2

	  and r3, pix, #15
	  ldr r3, [pal, r3]
	  str r3, [dst], #4
	.rept 7
	  lsr pix, #4
	  and r3, pix, #15
	  ldr r3, [pal, r3]
	  str r3, [dst], #4
	.endr

    bx lr
