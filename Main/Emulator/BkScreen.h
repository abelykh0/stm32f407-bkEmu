#ifndef __BKSCREEN_H
#define __BKSCREEN_H

#include "Display/Screen.h"
#include "m4vgalib/rasterizer.h"

#define Black     0B00000000
#define Red       0B00000011
#define Green     0B00001100
#define Blue      0B00110000
#define LightGrey 0B00010101
#define Grey      0B00101010
#define LightBlue 0B00010000
#define White     0B00111111

extern uint16_t port0177664;

using namespace Display;
using namespace vga;

namespace bk
{

class BkScreen: public Screen
{
protected:
    virtual void InvertColor() override;

public:
    BkScreen(VideoSettings settings);
    BkScreen(VideoSettings settings, uint16_t startLine, uint16_t height);

	virtual RasterInfo rasterize(unsigned cycles_per_pixel, unsigned line_number,
			Pixel *target) override;

	virtual ~BkScreen() = default;
};

}

#endif
