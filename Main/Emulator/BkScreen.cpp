#include "bkscreen.h"
#include <string.h>
#include "m4vgalib/vga.h"
#include "draw2color.h"
#include "draw4bw.h"
#include "bkemu.h"

//   0020 Screen mode 0 - 512x256, FF - 256x256
//   0021 Screen inversion 0 - off, FF - on

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
		White | LightBlue << 8,
		White | White << 8
};

BkScreen::BkScreen(VideoSettings settings) :
		BkScreen(settings, 0,
				settings.Timing->video_end_line
						- settings.Timing->video_start_line)
{
}

BkScreen::BkScreen(VideoSettings settings, uint16_t startLine, uint16_t height)
{
	this->Settings = settings;

	this->_startLine = startLine;
	this->_hResolutionNoBorder = this->Settings.TextColumns * 8;
	this->_hResolution = this->Settings.Timing->video_pixels / 2;
	this->_vResolution = height / 2;

	this->_horizontalBorder = (this->_hResolution - this->_hResolutionNoBorder) / 2;
	this->_verticalBorder = (this->_vResolution - this->Settings.TextRows * 8) / 2;
}

uint8_t* BkScreen::GetPixelPointer(uint16_t line)
{
    return &this->Settings.Pixels[line * 64];
}

uint8_t* BkScreen::GetPixelPointer(uint16_t line, uint8_t character)
{
	return this->GetPixelPointer(line) + character * 2;
}

void BkScreen::Clear()
{
	memset(this->Settings.Pixels, 0xFF, 0x4000);
}

//__attribute__((section(".ramcode")))
Rasterizer::RasterInfo BkScreen::rasterize(
		unsigned cycles_per_pixel, unsigned line_number, Pixel *target)
{
	uint8_t borderColor = 0x10; //*this->Settings.BorderColor;
	bool isNarrow = (RamBuffer[0x0020] == 0);

	unsigned scaledLine = (line_number - this->_startLine) / this->Settings.Scale;
	if (scaledLine == 0)
	{
		this->_frames++;
	}

	if (scaledLine < this->_verticalBorder
			|| scaledLine >= (unsigned) (this->_vResolution - this->_verticalBorder))
	{
		memset(&target[0], borderColor, this->_hResolution * (isNarrow ? 2 : 1));
	}
	else
	{
		// Border to the left
		memset(&target[0], borderColor, this->_horizontalBorder * (isNarrow ? 2 : 1));

		uint16_t vline = scaledLine - this->_verticalBorder;
		uint32_t* bitmap = (uint32_t*)this->GetPixelPointer(vline);
		uint8_t* dest = &target[this->_horizontalBorder * (isNarrow ? 2 : 1)];

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
		memset(&target[(this->_hResolution - this->_horizontalBorder) * (isNarrow ? 2 : 1)],
				borderColor, this->_horizontalBorder * (isNarrow ? 2 : 1));
	}

	Rasterizer::RasterInfo result;
	result.offset = 0;
	result.length = this->_hResolution * (isNarrow ? 2 : 1);
	result.cycles_per_pixel = cycles_per_pixel * (isNarrow ? 1 : 2);
	result.repeat_lines = 1;
	return result;
}
}
