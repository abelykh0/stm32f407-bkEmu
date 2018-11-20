#include "bkinput.h"
#include "bkEmu.h"
#include "Emulator/pdp/defines.h"
#include "Keyboard/ps2Keyboard.h"

// 0020 Screen mode 0 - 512x256, FF - 256x256
// 0023 Keyboard 0 - LAT, 80 - RUS

// bit 6 : interrupt enable
// bit 7 : status, 1 new key code available
uint8_t port0177660 = 0x40;

// bit 0..6 : key code
uint8_t port0177662;

// bit 6 : 0 key pressed
uint16_t port0177716 = 0x40;

#define TTY_VECTOR      060     /* standard vector  */
#define TTY_VECTOR2     0274    /* AR2 (ALT) vector */

char keyMap1[] = {
	// A    B    C    D    E    F    G    H    I    J
	  'F', 'I', 'S', 'W', 'T', 'A', 'P', 'R', '[', 'O',

	// K    L    M    N    O    P    Q    R    S    T
	  'L', 'D', 'X', 'T', ']', 'Z', 'J', 'K', 'Y', 'E',

	// U    V    W    X    Y    Z
	  'G', 'M', 'C', '^', 'N', 'Q'
};
char keyMap2[] = {
	// a    b    c    d    e    f    g    h    i    j
	  'f', 'i', 's', 'w', 't', 'a', 'p', 'r', '[', 'o',

	// k    l    m    n    o    p    q    r    s    t
	  'l', 'd', 'x', 't', ']', 'z', 'j', 'k', 'y', 'e',

	// u    v    w    x    y    z
	  'g', 'm', 'c', '^', 'n', 'q'
};

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

	char symbol;
	switch (scanCode)
	{
	case KEY_LEFTARROW:
		symbol = 0x08;
		break;
	case KEY_RIGHTARROW:
		symbol = 0x19;
		break;
	case KEY_UPARROW:
		symbol = 0x1A;
		break;
	case KEY_DOWNARROW:
		symbol = 0x1B;
		break;
	case KEY_BACKSPACE:
		symbol = 0x18;
		break;
	case KEY_ENTER:
		symbol = 0x0A;
		break;
	case KEY_LEFTCONTROL: // РУС
	case KEY_L_GUI:
		symbol = 0x0E;
		break;
	case KEY_RIGHTCONTROL: // ЛАТ
	case KEY_R_GUI:
		symbol = 0x0F;
		break;
	case KEY_ALT: // АР2
		symbol = 0x0F;
		break;
	default:
		symbol = Ps2_ConvertScancode(scanCode);
		if (RamBuffer[0x0023] == 0x80)
		{
			// РУС
			if (symbol >= 'A' && symbol <= 'Z')
			{
				symbol = keyMap1[symbol - 'A'];
			}
			else if (symbol >= 'a' && symbol <= 'z')
			{
				symbol = keyMap2[symbol - 'a'];
			}
		}

		break;
	}

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
