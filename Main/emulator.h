#ifndef __EMULATOR_H__
#define __EMULATOR_H__

#include "Display/Screen.h"
#include "Emulator/BkScreen.h"
#include "Display/StatusScreen.h"
#include "fatfs.h"

using namespace Display;
using namespace bk;

#define DEBUG_COLUMNS 70
#define DEBUG_ROWS 4

typedef struct
{
	uint8_t  Pixels[0x4000];
	uint8_t  BorderColor;
} BkScreenData;

extern StatusScreen DebugScreen;
extern BkScreen MainScreen;
extern uint8_t _buffer16K_1[0x4000];
extern uint8_t _buffer16K_2[0x4000];

void showKeyboardSetup();
bool showKeyboardLoop();

void setDateTimeSetup();
bool setDateTimeLoop();

void showHelp();
void clearHelp();
void toggleHelp();
void restoreHelp();

void initializeVideo();
void startVideo();
void showErrorMessage(const char* errorMessage);
void showTitle(const char* title);

void saveState();
void restoreState(bool restoreScreen);

void showRegisters();

#endif /* __EMULATOR_H__ */
