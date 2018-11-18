.syntax unified

@ Arguments:
@  r0  pixels
@  r1  palette
@  r2  destination
.section	.text.Draw2Color,"ax",%progbits
//.section .ramcode,"ax",%progbits
.balign 4
.global Draw2Color
.thumb_func
Draw2Color:
      @ parameters
      pix   .req r0
      pal   .req r1
      dst   .req r2

	  and r3, pix, #3
	  ldrb r3, [pal, r3]
	  strb r3, [dst], #1
	.rept 15
	  lsr pix, #2
	  and r3, pix, #3
	  ldrb r3, [pal, r3]
	  strb r3, [dst], #1
	.endr

    bx lr
