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
uint16_t _palette3[4] = {
		Black | Black << 8,
		Black | White << 8,
		White | Black << 8,
		White | White << 8
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

//__attribute__((section(".ramcode")))
Rasterizer::RasterInfo BkScreen::rasterize(
		unsigned cycles_per_pixel, unsigned line_number, Pixel *target)
{
	uint8_t borderColor = 0x10; //*this->Settings.BorderColor;
	bool isNarrow = (RamBuffer[0x0020] == 0);
	uint8_t charactersIn16bit = (isNarrow ? 2 : 1);

	unsigned scaledLine = (line_number - this->_startLine) / this->Settings.Scale;
	if (scaledLine == 0)
	{
		this->_frames++;
	}

	if (scaledLine < this->_verticalBorder
			|| scaledLine >= (unsigned) (this->_vResolution - this->_verticalBorder))
	{
		memset(&target[0], borderColor, this->_hResolution / charactersIn16bit);
	}
	else
	{
		// Border to the left
		memset(&target[0], borderColor, this->_horizontalBorder / charactersIn16bit);

		uint16_t vline = scaledLine - this->_verticalBorder;
		uint32_t* bitmap = (uint32_t*)this->GetPixelPointer(vline);
		uint8_t* dest = &target[this->_horizontalBorder / charactersIn16bit];

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
		memset(&target[(this->_hResolution - this->_horizontalBorder) / charactersIn16bit],
				borderColor, this->_horizontalBorder / charactersIn16bit);
	}

	Rasterizer::RasterInfo result;
	result.offset = 0;
	result.length = this->_hResolution * charactersIn16bit;
	result.cycles_per_pixel = cycles_per_pixel * (isNarrow ? 1 : 2);
	result.repeat_lines = 1;
	return result;
}
}
