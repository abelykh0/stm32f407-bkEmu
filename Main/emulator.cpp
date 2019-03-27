#include "Emulator.h"

#include <string.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "m4vgalib/vga.h"
#include "m4vgalib/timing.h"
#include "etl/stm32f4xx/gpio.h"
#include "Keyboard/ps2Keyboard.h"
#include "Emulator/bkEmu.h"
#include "resources/keyboard.h"

using namespace etl::stm32f4xx;

#define DEBUG_BAND_HEIGHT (DEBUG_ROWS * 8 * 2)

uint8_t _buffer16K_1[0x4000];
uint8_t _buffer16K_2[0x4000];

static uint8_t _palette1[6] = { Black, Blue, Green, Red, Black, White };
static uint8_t _palette2[6] = { Black, LightGrey, Grey, White, Black, White };
static uint8_t _paletteStatus[] = { LightBlue, White };

// Debug screen video RAM
// DEBUG_COLUMNS x DEBUG_ROWS characters
static uint8_t  _debugPixels[DEBUG_COLUMNS * 8 * DEBUG_ROWS];
static uint8_t  _debugBorderColor;

// Spectrum video RAM + border color
// 256x256 or 512x256 pixels
__attribute__((section(".vga_local_ram")))
static BkScreenData _bkScreenData;

// Used in saveState / restoreState
static BkScreenData* _savedScreenData = (BkScreenData*)_buffer16K_2;

// Debug band
static VideoSettings _videoSettings {
	&vga::timing_800x600_56hz, // Timing
	1, 2,  // Scale
	DEBUG_COLUMNS, DEBUG_ROWS, _debugPixels, (uint16_t*)_paletteStatus,
	&_debugBorderColor
};
static uint16_t _bkBandHeight = _videoSettings.Timing->video_end_line
		- _videoSettings.Timing->video_start_line - DEBUG_BAND_HEIGHT;
StatusScreen DebugScreen(&_videoSettings, _bkBandHeight, DEBUG_BAND_HEIGHT);
static vga::Band _debugBand {
	&DebugScreen, DEBUG_BAND_HEIGHT, nullptr
};

// BK screen band
static VideoSettings _bkVideoSettings {
	&vga::timing_800x600_56hz, // Timing
	1, 2, // Scale
	64, 32, _bkScreenData.Pixels, (uint16_t*)_palette1, &_bkScreenData.BorderColor
};
BkScreen MainScreen(&_bkVideoSettings, 0, _bkBandHeight);
static vga::Band _band {
	&MainScreen, _bkBandHeight, &_debugBand
};

static bool _showingKeyboard;
static bool _settingDateTime;
static uint32_t _frames;
static char* _newDateTime = (char*)_buffer16K_2;
static bool _helpShown;

extern RTC_HandleTypeDef hrtc;

void initializeVideo()
{
	vga::init();

	// This changes the CPU clock speed
	vga::configure_timing(*_videoSettings.Timing);

	// Inform HAL that the CPU clock speed changed
	SystemCoreClockUpdate();
}

void startVideo()
{
	DebugScreen.Clear();
	MainScreen.Clear();
	vga::configure_band_list(&_band);
	vga::video_on();
}

void showErrorMessage(const char* errorMessage)
{
	DebugScreen.SetAttribute(0x0310); // red on blue
	DebugScreen.PrintAlignCenter(2, errorMessage);
	DebugScreen.SetAttribute(0x3F10); // white on blue
}

void showTitle(const char* title)
{
	DebugScreen.PrintAlignCenter(0, title);
}

void showKeyboardSetup()
{
	saveState();
	_showingKeyboard = true;

	DebugScreen.Clear();
	DebugScreen.PrintAlignCenter(2, "Press any key to return");

	//MainScreen.ShowScreenshot(spectrumKeyboard);
	_bkScreenData.BorderColor = 0; // Black
}

bool showKeyboardLoop()
{
	if (!_showingKeyboard)
	{
		return false;
	}

	int32_t scanCode = Ps2_GetScancode();
	if (scanCode == 0 || (scanCode & 0xFF00) != 0xF000)
	{
		return true;
	}

	_showingKeyboard = false;
	restoreState(true);
	return false;
}

void toggleHelp()
{
	if (_helpShown)
	{
		clearHelp();
	}
	else
	{
		showHelp();
	}
}

void clearHelp()
{
	DebugScreen.HideCursor();
	DebugScreen.Clear();

	_helpShown = false;
}

void showHelp()
{
	DebugScreen.HideCursor();
	DebugScreen.Clear();

#ifdef BOARD2
	DebugScreen.PrintAt(0, 1, "F1  - show / hide help");
	DebugScreen.PrintAt(0, 2, "F3  - load snapshot from SD card");
	DebugScreen.PrintAt(0, 3, "F5  - reset");

	DebugScreen.PrintAt(35, 1, "F7  - color / black & white");
	DebugScreen.PrintAt(35, 2, "F10 - show keyboard layout");
	DebugScreen.PrintAt(35, 3, "F12 - show registers");
#else
	DebugScreen.PrintAt(0, 0, "F1  - show / hide help");
	DebugScreen.PrintAt(0, 1, "F2  - save snapshot to SD card");
	DebugScreen.PrintAt(0, 2, "F3  - load snapshot from SD card");
	DebugScreen.PrintAt(0, 3, "F5  - reset");

	DebugScreen.PrintAt(35, 0, "F6  - set date and time");
	DebugScreen.PrintAt(35, 1, "F7  - color / black & white");
	DebugScreen.PrintAt(35, 2, "F10 - show keyboard layout");
	DebugScreen.PrintAt(35, 3, "F12 - show registers");
#endif

	_helpShown = true;
}

void restoreHelp()
{
	if (_helpShown)
	{
		showHelp();
	}
	else
	{
		clearHelp();
	}
}

void setDateTimeSetup()
{
	_settingDateTime = true;

	DebugScreen.Clear();
	showTitle("Set Date and Time. ENTER, ESC, BS");

	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	char* formattedDateTime = (char*)_buffer16K_1;
	sprintf(formattedDateTime, "%04d-%02d-%02d %02d:%02d:%02d",
			date.Year + 2000, date.Month, date.Date,
			time.Hours, time.Minutes, time.Seconds);
	DebugScreen.PrintAlignCenter(1, formattedDateTime);

	_frames = DebugScreen._frames + 5;

	DebugScreen.PrintAt(0, 2, "Enter new date and time (yyyy-mm-dd hh:mm:ss):");
	DebugScreen.SetCursorPosition(0, 3);
	DebugScreen.ShowCursor();
	memset(_newDateTime, 0, 20);
}

bool setDateTimeLoop()
{
	if (!_settingDateTime)
	{
		return false;
	}

	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	uint8_t x = DebugScreen._cursor_x;

	if (DebugScreen._frames > _frames)
	{
		HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
		char* formattedDateTime = (char*)_buffer16K_1;
		sprintf(formattedDateTime, "%04d-%02d-%02d %02d:%02d:%02d",
				date.Year + 2000, date.Month, date.Date,
				time.Hours, time.Minutes, time.Seconds);
		DebugScreen.HideCursor();
		DebugScreen.PrintAlignCenter(1, formattedDateTime);
		DebugScreen.SetCursorPosition(x, 3);
		DebugScreen.ShowCursor();
		_frames = DebugScreen._frames + 5;
	}

	int32_t scanCode = Ps2_GetScancode();
	if (scanCode == 0 || (scanCode & 0xFF00) == 0xF000)
	{
		return true;
	}

	int year, month, day;
	int hour, minute, second;

	scanCode = ((scanCode & 0xFF0000) >> 8 | (scanCode & 0xFF));
	switch (scanCode)
	{
	case KEY_BACKSPACE:
		if (DebugScreen._cursor_x > 0)
		{
			DebugScreen.PrintAt(DebugScreen._cursor_x - 1, DebugScreen._cursor_y, " ");
			DebugScreen.SetCursorPosition(DebugScreen._cursor_x - 1, DebugScreen._cursor_y);
			_newDateTime[DebugScreen._cursor_x] = '\0';
		}
		break;

	case KEY_ENTER:
	case KEY_KP_ENTER:
		DebugScreen.HideCursor();
		if (sscanf(_newDateTime, "%04d-%02d-%02d %02d:%02d:%02d",
				&year, &month, &day,
				&hour, &minute, &second) == 6)
		{
			HAL_RTC_Init(&hrtc);

			memset(&time, 0, sizeof(time));
			time.Hours = hour;
			time.Minutes = minute;
			time.Seconds = second;
			HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);

			memset(&date, 0, sizeof(date));
			date.Year = year - 2000;
			date.Month = month;
			date.Date = day;
			HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);

			_settingDateTime = false;
			restoreState(false);
			return false;
		}
		else
		{
			DebugScreen.SetAttribute(0x0310); // red on blue
			DebugScreen.PrintAt(0, 7, "Invalid date and time");
			DebugScreen.SetAttribute(0x3F10); // white on blue
			DebugScreen.SetCursorPosition(x, 5);
			DebugScreen.ShowCursor();
		}
		break;

	case KEY_ESC:
		_settingDateTime = false;
		restoreState(false);
		return false;

	default:
		char character = Ps2_ConvertScancode(scanCode);
		if (DebugScreen._cursor_x < 20 && character != '\0')
		{
			char* text = (char*)_buffer16K_1;
			text[0] = character;
			_newDateTime[DebugScreen._cursor_x] = character;
			text[1] = '\0';
			DebugScreen.Print(text);
		}
		break;
	}

	return true;
}

void saveState()
{
	*_savedScreenData = _bkScreenData;
}

void restoreState(bool restoreScreen)
{
	if (restoreScreen)
	{
		_bkScreenData = *_savedScreenData;
	}

	restoreHelp();
}

void togglePalette()
{
	if (_bkVideoSettings.Attributes == (uint16_t*)_palette1)
	{
		_bkVideoSettings.Attributes = (uint16_t*)_palette2;
	}
	else
	{
		_bkVideoSettings.Attributes = (uint16_t*)_palette1;
	}
}

void showRegisters()
{
	DebugScreen.Clear();
	showTitle("Registers. ESC - clear");

    char* buf = (char*)_buffer16K_1;

    sprintf(buf, "R0 %04x  R3 %04x  SP %04x",
    	pdp.regs[0], pdp.regs[3], pdp.regs[6]);
    DebugScreen.PrintAlignCenter(1, buf);
    sprintf(buf, "R1 %04x  R4 %04x  PC %04x",
    	pdp.regs[1], pdp.regs[4], pdp.regs[7]);
    DebugScreen.PrintAlignCenter(2, buf);
    sprintf(buf, "R2 %04x  R5 %04x         ",
    	pdp.regs[2], pdp.regs[5]);
    DebugScreen.PrintAlignCenter(3, buf);
}
