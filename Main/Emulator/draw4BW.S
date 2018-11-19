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

	  and r3, pix, #1
	  ldrb r3, [pal, r3]
	  strb r3, [dst], #1
	.rept 31
	  lsr pix, #1
	  and r3, pix, #1
	  ldrb r3, [pal, r3]
	  strb r3, [dst], #1
	.endr

    bx lr
