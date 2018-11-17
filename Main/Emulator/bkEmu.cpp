// BK-0010 page mapping
// ====================
// 0000..3FFF RAM (16K)
//   0020 Screen mode 0 - 512x256, FF - 256x256
//   0021 Screen inversion 0 - off, FF - on
// 4000..7FFF Video RAM (16K)
// 8000..9FFF ROM Monitor and drivers (8K)
// A000..FF7F ROM Basic (24448 bytes)
// FF80..FFFF I/O ports
//   FFB0 Keyboard status
//   FFB2 Keyboard key code
//   FFB3 Scroll register
//   FFC6 System Timer counter start value
//   FFC8 System Timer counter
//   FFCA System Timer control

#include "bkEmu.h"
#include "pdp/defines.h"
#include "resources/basic.h"
#include "resources/monitor.h"
#include "Keyboard/ps2Keyboard.h"

uint8_t RamBuffer[RAM_AVAILABLE];
pdp_regs pdp;
unsigned short last_branch;
flag_t io_stop_happened;
BkScreen* _bkScreen;
flag_t bkmodel = 0;
double ticks_screen = 0.0;
const int TICK_RATE = 3000000; /* CPU clock speed */
double frame_delay = TICK_RATE / 25; /* Delay in ticks between video frames */

void run_2(pdp_regs* p, int flag);

void bk_setup(BkScreen* bkScreen)
{
	_bkScreen = bkScreen;
	bk_reset();
}

int32_t bk_loop()
{
	run_2(&pdp, 0);
	return 0;
}

void bk_reset()
{
	for (int x = 0; x < 8; ++x)
	{
		pdp.regs[x] = 0;
	}

	pdp.ir = 0;
	pdp.psw = 0200;
}

/*
 * Load a byte from the given address.
 */
extern "C" int ll_byte(pdp_regs* p, c_addr addr, d_byte* byte)
{
	if (addr >= (uint16_t) 0xFF80)
	{
		// I/O port
		*byte = 0;
	}
	else if (addr >= (uint16_t) 0xA000)
	{
		// ROM Basic
		*byte = basic[addr - (uint16_t)0xA000];
	}
	else if (addr >= (uint16_t) 0x8000)
	{
		// ROM Monitor
		*byte = monitor[addr - (uint16_t)0x8000];
	}
	else if (addr >= (uint16_t)0x4000)
	{
		// Video RAM
		*byte = _bkScreen->Settings.Pixels[addr - (uint16_t)0x4000];
	}
	else
	{
		// RAM
		*byte = RamBuffer[addr];
	}

	return OK;
}

/*
 * Load a word from the given address.
 */
extern "C" int ll_word(pdp_regs* p, c_addr addr, d_word* word)
{
	int result;
	uint8_t byte1;
	if ((result = ll_byte(p, addr, &byte1)) != OK)
	{
		return result;
	}

	uint8_t byte2;
	if ((result = ll_byte(p, addr + 1, &byte2)) != OK)
	{
		return result;
	}

	*word = (byte2 << 8) | byte1;

	return OK;
}

/*
 * Store a byte at the given address.
 */
extern "C" int sl_byte(pdp_regs* p, c_addr addr, d_byte byte)
{
	if (addr >= (uint16_t) 0xFF80)
	{
		// I/O port
	}
	else if (addr >= (uint16_t) 0xA000)
	{
		// Can't write to ROM
	}
	else if (addr >= (uint16_t) 0x4000)
	{
		// Video RAM
		_bkScreen->Settings.Pixels[addr - (uint16_t) 0x4000] = byte;
	}
	else
	{
		// RAM
		RamBuffer[addr] = byte;
	}

	return OK;
}

/*
 *  Store a word at the given address.
 */
extern "C" int sl_word(pdp_regs* p, c_addr addr, d_word word)
{
	int result;
	if ((result = sl_byte(p, addr, (uint8_t) word)) != OK)
	{
		return result;
	}

	if ((result = sl_byte(p, addr + 1, (uint8_t) (word >> 8))) != OK)
	{
		return result;
	}

	return OK;
}

/*
 * Reset the UNIBUS devices.
 */
extern "C" void q_reset()
{
}

void run_2(pdp_regs* p, int flag)
{
	register int result; /* result of execution */
	int result2 = OK; /* result of error handling */
	extern void intr_hand(); /* SIGINT handler */
	//register unsigned priority;	/* current processor priority */
	int rtt = 0; /* rtt don't trap yet flag */
	//d_word oldpc;
	//static char buf[80];

	/*
	 * Clear execution stop flag and install SIGINT handler.
	 */

	//stop_it = 0;
	//signal( SIGINT, intr_hand );
	//Uint32 last_screen_update = SDL_GetTicks();
	//double timing_delta = ticks - SDL_GetTicks() * (TICK_RATE/1000.0);
	//c_addr startpc = p->regs[PC];
	/*
	 * Run until told to stop.
	 */

	//do
//	{
		//addtocybuf(p->regs[PC]);

		/*
		 * Fetch and execute the instruction.
		 */

		result = ll_word(p, p->regs[PC], &p->ir);
		//oldpc = p->regs[PC];
		p->regs[PC] += 2;
		if (result == OK)
		{
			result = (itab[p->ir >> 6].func)();
			//timing(p);
		}

		/*
		 * Mop up the mess.
		 */

		if (result != OK)
		{
			switch (result)
			{
			case BUS_ERROR: /* vector 4 */
				ticks += 64;
				break;
			case ODD_ADDRESS:
				result2 = service((d_word) 04);
				break;
			case CPU_ILLEGAL: /* vector 10 */
				result2 = service((d_word) 010);
				break;
			case CPU_BPT: /* vector 14 */
				result2 = service((d_word) 014);
				break;
			case CPU_EMT: /* vector 30 */
				result2 = service((d_word) 030);
				break;
			case CPU_TRAP: /* vector 34 */
				result2 = service((d_word) 034);
				break;
			case CPU_IOT: /* vector 20 */
				result2 = service((d_word) 020);
				break;
			case CPU_WAIT:
				in_wait_instr = 1;
				result2 = OK;
				break;
			case CPU_RTT:
				rtt = 1;
				result2 = OK;
				break;
			case CPU_HALT:
				io_stop_happened = 4;
				result2 = service((d_word) 004);
				break;
			default:
				// Unexpected return
				flag = 0;
				result2 = OK;
				break;
			}
			if (result2 != OK)
			{
				// Double trap
				ll_word(p, 0177716, &p->regs[PC]);
				p->regs[PC] &= 0177400;
			}
		}

		if ((p->psw & 020) && (rtt == 0))
		{ /* trace bit */
			if (service((d_word) 014) != OK)
			{
				// Double trap
				ll_word(p, 0177716, &p->regs[PC]);
				p->regs[PC] &= 0177400;
				p->regs[SP] = 01000; /* whatever */
			}
		}
		rtt = 0;
		p->total++;

		//if (nflag)
		//	sound_flush();

//		if (bkmodel && ticks >= ticks_timer) {
//			scr_sync();
//			if (timer_intr_enabled) {
//				ev_register(TIMER_PRI, service, 0, 0100);
//			}
//			ticks_timer += half_frame_delay;
//		}

		if (ticks >= ticks_screen)
		{
			/* In full speed, update every 40 real ms */
//		    if (fullspeed) {
//			Uint32 cur_sdl_ticks = SDL_GetTicks();
//		 	if (cur_sdl_ticks - last_screen_update >= 40) {
//			    last_screen_update = cur_sdl_ticks;
//			    scr_flush();
//			}
//		    } else {
//			scr_flush();
//		    }
//		    tty_recv();
			ticks_screen += frame_delay;
			/* In simulated speed, if we're more than 10 ms
			 * ahead, slow down. Avoid rounding the delay up
			 * by SDL. If the sound is on, sound buffering
			 * provides synchronization.
			 */
//		    if (!fullspeed && !nflag) {
//		    	double cur_delta =
//				ticks - SDL_GetTicks() * (TICK_RATE/1000.0);
//			if (cur_delta - timing_delta > TICK_RATE/100) {
//				int msec = (cur_delta - timing_delta) / (TICK_RATE/1000);
//				SDL_Delay(msec / 10 * 10);
//			}
//        }
		}

		/*
		 * See if execution should be stopped.  If so
		 * stop running, otherwise look for events
		 * to fire.
		 */

//		if ( stop_it ) {
//			fprintf( stderr, _("\nExecution interrupted.\n") );
//			flag = 0;
//		} else {
//			priority = ( p->psw >> 5) & 7;
//			if ( pending_interrupts && priority != 7 ) {
//				ev_fire( priority );
//			}
//		}
//		if (checkpoint(p->regs[PC])) {
//			flag = 0;
//		}
	//} while (flag);

	//signal( SIGINT, SIG_DFL );
}
