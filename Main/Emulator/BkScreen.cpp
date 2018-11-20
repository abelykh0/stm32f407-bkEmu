#include "bkscreen.h"
#include <string.h>
#include "m4vgalib/vga.h"
#include "draw2color.h"
#include "draw4bw.h"
#include "bkemu.h"

//   0020 Screen mode 0 - 512x256, FF - 256x256
//   0021 Screen inversion 0 - off, FF - on

uint16_t port0177664;

namespace bk
{

uint8_t _palette1[4] = { Black, Blue, Green, Red };
uint8_t _palette2[4] = { Black, LightGrey, Grey, White };
uint32_t _palette3[16] = {
		Black << 24 | Black << 16 | Black << 8 | Black,
		Black << 24 | Black << 16 | Black << 8 | White,
		Black << 24 | Black << 16 | White << 8 | Black,
		Black << 24 | Black << 16 | White << 8 | White,
		Black << 24 | White << 16 | Black << 8 | Black,
		Black << 24 | White << 16 | Black << 8 | White,
		Black << 24 | White << 16 | White << 8 | Black,
		Black << 24 | White << 16 | White << 8 | White,
		White << 24 | Black << 16 | Black << 8 | Black,
		White << 24 | Black << 16 | Black << 8 | White,
		White << 24 | Black << 16 | White << 8 | Black,
		White << 24 | Black << 16 | White << 8 | White,
		White << 24 | White << 16 | Black << 8 | Black,
		White << 24 | White << 16 | Black << 8 | White,
		White << 24 | White << 16 | White << 8 | Black,
		White << 24 | White << 16 | White << 8 | White
};
uint16_t _palette4[4] = {
		LightBlue | LightBlue << 8,
		LightBlue | White << 8,
		White     | LightBlue << 8,
		White     | White << 8
};

BkScreen::BkScreen(VideoSettings settings) :
		BkScreen(settings, 0,
				settings.Timing->video_end_line
						- settings.Timing->video_start_line)
{
}

BkScreen::BkScreen(VideoSettings settings, uint16_t startLine, uint16_t height)
	: Screen(settings, startLine, height)
{
	port0177664 = 01330;

	this->_attributeCount = 0;
}

void BkScreen::InvertColor()
{
}

__attribute__((section(".ramcode")))
Rasterizer::RasterInfo BkScreen::rasterize(
		unsigned cycles_per_pixel, unsigned line_number, Pixel *target)
{
	uint8_t borderColor = 0x10; //*this->Settings.BorderColor;
	bool isNarrow = (RamBuffer[0x0020] == 0);
	uint8_t divider = (isNarrow ? 1 : 2);

	uint16_t scaledLine = (line_number - this->_startLine) / 2;
	if (scaledLine == 0)
	{
		this->_frames++;
	}

	if (scaledLine < this->_verticalBorder
			|| scaledLine >= (unsigned)(this->_vResolution - this->_verticalBorder))
	{
		memset(&target[0], borderColor, this->_hResolution / divider);
	}
	else
	{
		// Border to the left
		//memset(&target[0], borderColor, this->_horizontalBorder / divider);

		uint8_t vline = scaledLine - this->_verticalBorder;

		// scroll
		vline += (port0177664 - 0330);

		uint32_t* bitmap = (uint32_t*)this->GetPixelPointer(vline);
		uint8_t* dest = &target[this->_horizontalBorder / divider];

		if (isNarrow)
		{
			for (int x = 0; x < 16; x++)
			{
				Draw4BW(*bitmap, _palette3, dest);
				dest += 32;
				bitmap++;
			}
		}
		else
		{
			for (int x = 0; x < 16; x++)
			{
				Draw2Color(*bitmap, _palette1, dest);
				dest += 16;
				bitmap++;
			}
		}

		// Border to the right
		//memset(&target[(this->_hResolution - this->_horizontalBorder) / divider],
		//		borderColor, this->_horizontalBorder / divider);
	}

	Rasterizer::RasterInfo result;
	result.offset = 0;
	result.length = this->_hResolution / divider;
	result.cycles_per_pixel = cycles_per_pixel * divider;
	result.repeat_lines = 1;
	return result;
}
}
