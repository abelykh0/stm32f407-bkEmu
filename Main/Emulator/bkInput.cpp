#include "bkinput.h"
#include "Emulator/pdp/defines.h"
#include "Keyboard/ps2Keyboard.h"

// bit 6 : interrupt enable
// bit 7 : status, 1 new key code available
uint16_t port0177660 = 0x40;

// bit 0..6 : key code
uint16_t port0177662;

// bit 6 : 0 key pressed
uint16_t port0177716 = 0x40;

#define TTY_VECTOR      060     /* standard vector  */
#define TTY_VECTOR2     0274    /* AR2 (ALT) vector */

int tty_finish(d_word info)
{
	service(( info & 0200 ) ? TTY_VECTOR2 : TTY_VECTOR);
	//tty_pending_int = 0;
	return OK;
}

bool OnKey(uint32_t scanCode, bool isKeyUp)
{
	if (isKeyUp)
	{
		return false;
	}

	char symbol = Ps2_ConvertScancode(scanCode);
	if (symbol == '\0')
	{
		return false;
	}

	port0177660 |= 0x80;
	port0177716 |= 0x04;
	if (isKeyUp)
	{
		port0177716 &= ~0x80;
	}
	else
	{
		port0177716 |= 0x80;
	}
	port0177662 = symbol & 0x7F;

	ev_register(TTY_PRI, tty_finish, 0, port0177662);

	return true;
}
