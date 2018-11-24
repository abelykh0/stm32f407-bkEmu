#ifndef __BKEMU_H
#define __BKEMU_H

#include "BkScreen.h"
#include "pdp/defines.h"

using namespace bk;

#define RAM_AVAILABLE 0x4000
extern uint8_t RamBuffer[RAM_AVAILABLE];
extern BkScreen* _bkScreen;
extern pdp_regs pdp;

void bk_setup(BkScreen* bkScreen);
int32_t bk_loop();
void bk_reset();

#endif
