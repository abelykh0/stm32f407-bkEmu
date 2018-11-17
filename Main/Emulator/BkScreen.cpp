#include "bkscreen.h"
#include <string.h>
#include "m4vgalib/vga.h"

namespace bk
{

/* BK0010 Palette */
uint8_t _palette[4] = { Black, Blue, Green, Red };

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
	this->_hResolution = this->Settings.Timing->video_pixels / this->Settings.Scale;
	this->_vResolution = height / this->Settings.Scale;

	this->_horizontalBorder = (this->_hResolution - this->_hResolutionNoBorder) / 2;
	this->_verticalBorder = (this->_vResolution - this->Settings.TextRows * 8) / 2;

	this->_pixelCount = 256 * 256;
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
	uint8_t borderColor = 0x10;//*this->Settings.BorderColor;

	unsigned scaledLine = (line_number - this->_startLine) / this->Settings.Scale;
	if (scaledLine == 0)
	{
		this->_frames++;
	}

	if (scaledLine < this->_verticalBorder
			|| scaledLine >= (unsigned) (this->_vResolution - this->_verticalBorder))
	{
		memset(&target[0], borderColor, this->_hResolution);
	}
	else
	{
		// Border to the left
		memset(&target[0], borderColor, this->_horizontalBorder);

		uint16_t vline = scaledLine - this->_verticalBorder;
		uint32_t* bitmap = (uint32_t*)this->GetPixelPointer(vline);
		uint8_t* dest = &target[this->_horizontalBorder];

		for (int x = 0; x < 8; x++)
		{
			uint32_t pixelInfo = *bitmap;

			for (int i = 0; i < 16; i++)
			{
				uint8_t pixelColor = pixelInfo & 0x03;
				*dest = _palette[pixelColor];
				dest++;
				pixelInfo >>= 2;
			}

			bitmap++;
		}
		//memset(dest, 0x00, 256);

		// Border to the right
		memset(&target[this->_hResolution - this->_horizontalBorder],
				borderColor, this->_horizontalBorder);
	}

	Rasterizer::RasterInfo result;
	result.offset = 0;
	result.length = this->_hResolution;
	result.cycles_per_pixel = cycles_per_pixel * this->Settings.Scale;
	result.repeat_lines = (unsigned) (this->Settings.Scale - 1);
	return result;
}
}
