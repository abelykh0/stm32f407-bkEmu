#ifndef __STATUSSCREEN_H
#define __STATUSSCREEN_H

#include "Screen.h"
#include "m4vgalib/rasterizer.h"

#define LightBlue 0B00010000
#define White     0B00111111

using namespace vga;

namespace Display
{

class StatusScreen: public Screen
{
protected:
    virtual void InvertColor() override;

public:
    StatusScreen(VideoSettings settings, uint16_t startLine, uint16_t height);

	virtual RasterInfo rasterize(unsigned cycles_per_pixel, unsigned line_number,
			Pixel *target) override;

	virtual ~StatusScreen() = default;
};

}

#endif