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

	  ror pix, #31 // << 1
	  and r3, pix, #6
	  ldrh r3, [pal, r3]
	  strh r3, [dst], #2
	.rept 15
	  ror pix, #2
	  and r3, pix, #6
	  ldrh r3, [pal, r3]
	  strh r3, [dst], #2
	.endr

    bx lr
