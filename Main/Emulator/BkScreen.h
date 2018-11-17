#ifndef __BKSCREEN_H
#define __BKSCREEN_H

#include <Display/font8x8.h>
#include <Display/VideoSettings.h>
#include "m4vgalib/rasterizer.h"

#define Black     0B00000000
#define Red       0B00000011
#define Green     0B00001100
#define Blue      0B00110000
#define Yellow    0B00001111
#define Cyan      0B00111100
#define Magenta   0B00110011
#define Brown     0B00010111
#define Violet    0B00110111
#define DarkRed   0B00000001
#define LightBlue 0B00010000
#define White     0B00111111

using namespace Display;
using namespace vga;

namespace bk
{

class BkScreen: public Rasterizer
{
private:
	uint16_t _startLine;
    uint16_t _horizontalBorder;
    uint8_t _verticalBorder;

protected:
    uint16_t _hResolution;
    uint16_t _hResolutionNoBorder;
    uint16_t _vResolution;

	virtual uint8_t* GetPixelPointer(uint16_t line);
	virtual uint8_t* GetPixelPointer(uint16_t line, uint8_t character);

public:
    uint32_t _pixelCount;
    volatile uint32_t _frames = 0;

    VideoSettings Settings;
    BkScreen(VideoSettings settings);
    BkScreen(VideoSettings settings, uint16_t startLine, uint16_t height);

	void Clear();

	virtual RasterInfo rasterize(unsigned cycles_per_pixel, unsigned line_number,
			Pixel *target) override;

	virtual ~BkScreen() = default;
};

}

#endif
