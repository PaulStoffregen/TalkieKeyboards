/*
  PS2Keyboard.cpp - PS2Keyboard library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Christian Weichel <info@32leaves.net>

  ** Mostly rewritten Paul Stoffregen <paul@pjrc.com> 2010, 2011
  ** Modified for use beginning with Arduino 13 by L. Abraham Smith, <n3bah@microcompdesign.com> * 
  ** Modified for easy interrup pin assignement on method begin(datapin,irq_pin). Cuningan <cuninganreset@gmail.com> **

  for more information you can read the original wiki in arduino.cc
  at http://www.arduino.cc/playground/Main/PS2Keyboard
  or http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html

  Version 2.4 (March 2013)
  - Support Teensy 3.0, Arduino Due, Arduino Leonardo & other boards
  - French keyboard layout, David Chochoi, tchoyyfr at yahoo dot fr

  Version 2.3 (October 2011)
  - Minor bugs fixed

  Version 2.2 (August 2011)
  - Support non-US keyboards - thanks to Rainer Bruch for a German keyboard :)

  Version 2.1 (May 2011)
  - timeout to recover from misaligned input
  - compatibility with Arduino "new-extension" branch
  - TODO: send function, proposed by Scott Penrose, scooterda at me dot com

  Version 2.0 (June 2010)
  - Buffering added, many scan codes can be captured without data loss
    if your sketch is busy doing other work
  - Shift keys supported, completely rewritten scan code to ascii
  - Slow linear search replaced with fast indexed table lookups
  - Support for Teensy, Arduino Mega, and Sanguino added

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "PS2Key.h"

// The ISR for the external interrupt
void PS2Keyboard::ps2interrupt(void)
{
	//static uint8_t bitcount=0;
	//static uint8_t incoming=0;
	//static uint32_t prev_ms=0;
	uint32_t now_ms;
	uint8_t n, val;

	val = digitalRead(DataPin);
	now_ms = millis();
	if (now_ms - prev_ms > 250) {
		bitcount = 0;
		incoming = 0;
	}
	prev_ms = now_ms;
	n = bitcount - 1;
	if (n <= 7) {
		incoming |= (val << n);
	}
	bitcount++;
	if (bitcount == 11) {
		uint8_t i = head + 1;
		if (i >= PS2KEYBOARD_BUFFER_SIZE) i = 0;
		if (i != tail) {
			buffer[i] = incoming;
			head = i;
		}
		bitcount = 0;
		incoming = 0;
	}
}

uint8_t PS2Keyboard::get_scan_code(void)
{
	uint8_t c, i;

	i = tail;
	if (i == head) return 0;
	i++;
	if (i >= PS2KEYBOARD_BUFFER_SIZE) i = 0;
	c = buffer[i];
	tail = i;
	return c;
}

// http://www.quadibloc.com/comp/scan.htm
// http://www.computer-engineering.org/ps2keyboard/scancodes2.html

// These arrays provide a simple key map, to turn scan codes into ISO8859
// output.  If a non-US keyboard is used, these may need to be modified
// for the desired output.
//

const PROGMEM PS2Keymap_t PS2Keymap_US = {
  // without shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '`', 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
	0, 0, 'z', 's', 'a', 'w', '2', 0,
	0, 'c', 'x', 'd', 'e', '4', '3', 0,
	0, ' ', 'v', 'f', 't', 'r', '5', 0,
	0, 'n', 'b', 'h', 'g', 'y', '6', 0,
	0, 0, 'm', 'j', 'u', '7', '8', 0,
	0, ',', 'k', 'i', 'o', '0', '9', 0,
	0, '.', '/', 'l', ';', 'p', '-', 0,
	0, 0, '\'', 0, '[', '=', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, ']', 0, '\\', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
  // with shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '~', 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'Q', '!', 0,
	0, 0, 'Z', 'S', 'A', 'W', '@', 0,
	0, 'C', 'X', 'D', 'E', '$', '#', 0,
	0, ' ', 'V', 'F', 'T', 'R', '%', 0,
	0, 'N', 'B', 'H', 'G', 'Y', '^', 0,
	0, 0, 'M', 'J', 'U', '&', '*', 0,
	0, '<', 'K', 'I', 'O', ')', '(', 0,
	0, '>', '?', 'L', ':', 'P', '_', 0,
	0, 0, '"', 0, '{', '+', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '}', 0, '|', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
	0
};


const PROGMEM PS2Keymap_t PS2Keymap_German = {
  // without shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '^', 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
	0, 0, 'y', 's', 'a', 'w', '2', 0,
	0, 'c', 'x', 'd', 'e', '4', '3', 0,
	0, ' ', 'v', 'f', 't', 'r', '5', 0,
	0, 'n', 'b', 'h', 'g', 'z', '6', 0,
	0, 0, 'm', 'j', 'u', '7', '8', 0,
	0, ',', 'k', 'i', 'o', '0', '9', 0,
	0, '.', '-', 'l', PS2_o_DIAERESIS, 'p', PS2_SHARP_S, 0,
	0, 0, PS2_a_DIAERESIS, 0, PS2_u_DIAERESIS, '\'', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '+', 0, '#', 0, 0,
	0, '<', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
  // with shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, PS2_DEGREE_SIGN, 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'Q', '!', 0,
	0, 0, 'Y', 'S', 'A', 'W', '"', 0,
	0, 'C', 'X', 'D', 'E', '$', PS2_SECTION_SIGN, 0,
	0, ' ', 'V', 'F', 'T', 'R', '%', 0,
	0, 'N', 'B', 'H', 'G', 'Z', '&', 0,
	0, 0, 'M', 'J', 'U', '/', '(', 0,
	0, ';', 'K', 'I', 'O', '=', ')', 0,
	0, ':', '_', 'L', PS2_O_DIAERESIS, 'P', '?', 0,
	0, 0, PS2_A_DIAERESIS, 0, PS2_U_DIAERESIS, '`', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '*', 0, '\'', 0, 0,
	0, '>', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
	1,
  // with altgr
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, 0, 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, '@', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_SUPERSCRIPT_TWO, 0,
	0, 0, 0, 0, PS2_CURRENCY_SIGN, 0, PS2_SUPERSCRIPT_THREE, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, PS2_MICRO_SIGN, 0, 0, '{', '[', 0,
	0, 0, 0, 0, 0, '}', ']', 0,
	0, 0, 0, 0, 0, 0, '\\', 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '~', 0, '#', 0, 0,
	0, '|', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 }
};


const PROGMEM PS2Keymap_t PS2Keymap_French = {
  // without shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, PS2_SUPERSCRIPT_TWO, 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'a', '&', 0,
	0, 0, 'w', 's', 'q', 'z', PS2_e_ACUTE, 0,
	0, 'c', 'x', 'd', 'e', '\'', '"', 0,
	0, ' ', 'v', 'f', 't', 'r', '(', 0,
	0, 'n', 'b', 'h', 'g', 'y', '-', 0,
	0, 0, ',', 'j', 'u', PS2_e_GRAVE, '_', 0,
	0, ';', 'k', 'i', 'o', PS2_a_GRAVE, PS2_c_CEDILLA, 0,
	0, ':', '!', 'l', 'm', 'p', ')', 0,
	0, 0, PS2_u_GRAVE, 0, '^', '=', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '$', 0, '*', 0, 0,
	0, '<', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
  // with shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, 0, 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'A', '1', 0,
	0, 0, 'W', 'S', 'Q', 'Z', '2', 0,
	0, 'C', 'X', 'D', 'E', '4', '3', 0,
	0, ' ', 'V', 'F', 'T', 'R', '5', 0,
	0, 'N', 'B', 'H', 'G', 'Y', '6', 0,
	0, 0, '?', 'J', 'U', '7', '8', 0,
	0, '.', 'K', 'I', 'O', '0', '9', 0,
	0, '/', PS2_SECTION_SIGN, 'L', 'M', 'P', PS2_DEGREE_SIGN, 0,
	0, 0, '%', 0, PS2_DIAERESIS, '+', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, PS2_POUND_SIGN, 0, PS2_MICRO_SIGN, 0, 0,
	0, '>', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
	1,
  // with altgr
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, 0, 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, '@', 0, 0,
	0, 0, 0, 0, 0, 0, '~', 0,
	0, 0, 0, 0, 0 /*PS2_EURO_SIGN*/, '{', '#', 0,
	0, 0, 0, 0, 0, 0, '[', 0,
	0, 0, 0, 0, 0, 0, '|', 0,
	0, 0, 0, 0, 0, '`', '\\', 0,
	0, 0, 0, 0, 0, '@', '^', 0,
	0, 0, 0, 0, 0, 0, ']', 0,
	0, 0, 0, 0, 0, 0, '}', 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '¤', 0, '#', 0, 0,
	0, '|', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 }
};


#define BREAK     0x01
#define MODIFIER  0x02
#define SHIFT_L   0x04
#define SHIFT_R   0x08
#define ALTGR     0x10
#define MODIFIER2 0x20

char PS2Keyboard::get_iso8859_code(void)
{
	uint8_t s;
	char c;

	while (1) {
		s = get_scan_code();
		if (!s) return 0;
		//Serial.printf("%02X ", s);
		if (s == 0xF0) {
			state |= BREAK;
		} else if (s == 0xE0) {
			state |= MODIFIER;
		} else if (s == 0xE1) {
			state |= MODIFIER2;
		} else {
			if (state & BREAK) {
				if (s == 0x12) {
					state &= ~SHIFT_L;
				} else if (s == 0x59) {
					state &= ~SHIFT_R;
				} else if (s == 0x11 && (state & MODIFIER)) {
					state &= ~ALTGR;
				}
				// CTRL, ALT & WIN keys could be added
				// but is that really worth the overhead?
				state &= ~(BREAK | MODIFIER);
				continue;
			}
			if (s == 0x12) {
				state |= SHIFT_L;
				//continue;  // allow left shift key
			} else if (s == 0x59) {
				state |= SHIFT_R;
				//continue;  // allow right shift key
			} else if (s == 0x11 && (state & MODIFIER)) {
				state |= ALTGR;
				return 71; // right alt key
			} else if (s == 0x1F && (state & MODIFIER)) {
				return 68; // left windows key
			} else if (s == 0x27 && (state & MODIFIER)) {
				return 72; // right windows key
			} else if (s == 0x2F && (state & MODIFIER)) {
				return 73; // command key
			} else if (s == 0x14 && (state & MODIFIER)) {
				return 74; // right control key
			} else if (s == 0x12 && (state & MODIFIER)) {
				return 85; // print screen key
			}
			c = 0;
			if (state & MODIFIER) {
				switch (s) {
				  case 0x70: c = PS2_INSERT;      break;
				  case 0x6C: c = PS2_HOME;        break;
				  case 0x7D: c = PS2_PAGEUP;      break;
				  case 0x71: c = PS2_DELETE;      break;
				  case 0x69: c = PS2_END;         break;
				  case 0x7A: c = PS2_PAGEDOWN;    break;
				  case 0x75: c = PS2_UPARROW;     break;
				  case 0x6B: c = PS2_LEFTARROW;   break;
				  case 0x72: c = PS2_DOWNARROW;   break;
				  case 0x74: c = PS2_RIGHTARROW;  break;
				  case 0x7C: c = PS2_PRINTSCREEN; break;
				  case 0x4A: c = 89;              break; // keypad /
				  case 0x5A: c = 102; 	          break; // keypad enter
				  default: break;
				}
			} else if ((state & MODIFIER2) && (s == 0x14)) {
				c = PS2_PAUSEBREAK;
				state = 0;
			} else if ((state & ALTGR) && keymap->uses_altgr) {
				if (s < PS2_KEYMAP_SIZE)
					c = pgm_read_byte(keymap->altgr + s);
			} else if (state & (SHIFT_L | SHIFT_R)) {
				if (s < PS2_KEYMAP_SIZE)
					c = pgm_read_byte(keymap->shift + s);
			} else {
				if (s < PS2_KEYMAP_SIZE)
					c = pgm_read_byte(keymap->noshift + s);
			}
			state &= ~(BREAK | MODIFIER | MODIFIER2);
			if (c) return c;
		}
	}
}

bool PS2Keyboard::available() {
	if (CharBuffer || UTF8next) return true;
	CharBuffer = get_iso8859_code();
	if (CharBuffer) return true;
	return false;
}

int PS2Keyboard::read() {
	uint8_t result;

	result = UTF8next;
	if (result) {
		UTF8next = 0;
	} else {
		result = CharBuffer;
		if (result) {
			CharBuffer = 0;
		} else {
			result = get_iso8859_code();
		}
		if (result >= 128) {
			UTF8next = (result & 0x3F) | 0x80;
			result = ((result >> 6) & 0x1F) | 0xC0;
		}
	}
	if (!result) return -1;
	return result;
}

int PS2Keyboard::readUnicode() {
	int result;

	result = CharBuffer;
	if (!result) result = get_iso8859_code();
	if (!result) return -1;
	UTF8next = 0;
	CharBuffer = 0;
	return result;
}


static uint8_t instance_allocated_map=0x00;
static PS2Keyboard *instances[8];

void ps2key_isr0(void) { instances[0]->ps2interrupt(); }
void ps2key_isr1(void) { instances[1]->ps2interrupt(); }
void ps2key_isr2(void) { instances[2]->ps2interrupt(); }
void ps2key_isr3(void) { instances[3]->ps2interrupt(); }
void ps2key_isr4(void) { instances[4]->ps2interrupt(); }
void ps2key_isr5(void) { instances[5]->ps2interrupt(); }
void ps2key_isr6(void) { instances[6]->ps2interrupt(); }
void ps2key_isr7(void) { instances[7]->ps2interrupt(); }

void PS2Keyboard::begin(uint8_t data_pin, uint8_t irq_pin, const PS2Keymap_t &map) {
	DataPin = data_pin;
	keymap = &map;

	// initialize the pins
	pinMode(irq_pin, INPUT_PULLUP);
	pinMode(data_pin, INPUT_PULLUP);

	bitcount = 0;
	incoming = 0;
	state = 0;
	head = 0;
	tail = 0;
	for (uint8_t i=0; i<8; i++) {
		if (instance_allocated_map & (1<<i)) continue;
		instance_allocated_map |= (1<<i);
		instances[i] = this;
		unsigned int irqnum = digitalPinToInterrupt(irq_pin);
		switch (i) {
		  case 0: attachInterrupt(irqnum, ps2key_isr0, FALLING); break;
		  case 1: attachInterrupt(irqnum, ps2key_isr1, FALLING); break;
		  case 2: attachInterrupt(irqnum, ps2key_isr2, FALLING); break;
		  case 3: attachInterrupt(irqnum, ps2key_isr3, FALLING); break;
		  case 4: attachInterrupt(irqnum, ps2key_isr4, FALLING); break;
		  case 5: attachInterrupt(irqnum, ps2key_isr5, FALLING); break;
		  case 6: attachInterrupt(irqnum, ps2key_isr6, FALLING); break;
		  case 7: attachInterrupt(irqnum, ps2key_isr7, FALLING); break;
		}
		break;
	}
}




























