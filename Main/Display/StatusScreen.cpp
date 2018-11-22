#include "StatusScreen.h"
#include <string.h>
#include "m4vgalib/vga.h"
#include "m4vgalib/rast/unpack_1bpp.h"

namespace Display
{

uint8_t _palette[2] = { LightBlue, White };

StatusScreen::StatusScreen(VideoSettings settings, uint16_t startLine, uint16_t height)
	: Screen(settings, startLine, height)
{
	this->_hResolution = 560;
	this->_horizontalBorder = (this->_hResolution - this->_hResolutionNoBorder) / 2;
	this->_attributeCount = 0;
}

void StatusScreen::InvertColor()
{
	for (int i = 0; i < 8; i++)
	{
		uint8_t* pixels = this->GetPixelPointer(this->_cursor_y + i, this->_cursor_x);
		*pixels = ~(*pixels);
	}
}

//__attribute__((section(".ramcode")))
Rasterizer::RasterInfo StatusScreen::rasterize(
		unsigned cycles_per_pixel, unsigned line_number, Pixel *target)
{
	uint8_t borderColor = *this->Settings.BorderColor;
	uint16_t scaledLine = (line_number - this->_startLine) / 2;
	uint16_t scaledResolution = this->_hResolution;

	Rasterizer::RasterInfo result;
	result.offset = 125;
	result.length = scaledResolution;
	result.cycles_per_pixel = cycles_per_pixel;

	if (this->_verticalBorder > 0)
	{
		if (scaledLine == 0)
		{
			uint32_t fill = borderColor << 8 | borderColor;
			fill |= fill << 16;
			for (uint32_t* ptr = (uint32_t*)target; ptr <= (uint32_t*)target + scaledResolution / 4; ptr++)
			{
				*ptr = fill;
			}
			result.repeat_lines = this->_verticalBorder * 2 - 1;
		}
		else if (scaledLine == (unsigned)(this->_vResolution - this->_verticalBorder - 1))
		{
			uint32_t fill = borderColor << 8 | borderColor;
			fill |= fill << 16;
			for (uint32_t* ptr = (uint32_t*)target; ptr <= (uint32_t*)target + scaledResolution / 4; ptr++)
			{
				*ptr = fill;
			}
			result.repeat_lines = (this->_vResolution - this->_verticalBorder) * 2 - 1;
		}
	}
	else
	{
		uint8_t vline = scaledLine - this->_verticalBorder;
		uint32_t* bitmap = (uint32_t*)this->GetPixelPointer(vline);
		uint8_t* dest = &target[this->_horizontalBorder];
		rast::unpack_1bpp_impl(bitmap, _palette, dest, this->Settings.TextColumns / 4);
		result.repeat_lines = 1;
	}

	return result;
}
}
