#include "PS2Key.h"
#include "words.h"

PS2Keyboard keyboard1;
PS2Keyboard keyboard2;
PS2Keyboard keyboard3;
PS2Keyboard keyboard4;
PS2Keyboard keyboard5;

const PROGMEM PS2Keymap_t myKeymap = {
  // without shift
        {0, 9, 0, 5, 3, 1, 2, 12,
        0, 10, 8, 6, 4, 28, 14, 0,
        0, 69 /*Lalt*/, 55 /*Lshift*/, 0, 67 /*Lctrl*/, 29, 15, 0,
        0, 0, 56, 44, 43, 30, 16, 0,
        0, 58, 57, 45, 31, 18, 17, 0,
        0, 70, 59, 46, 33, 32, 19, 0,
        0, 61, 60, 48, 47, 34, 20, 0,
        0, 0, 62, 49, 35, 21, 22, 0,
        0, 63, 50, 36, 37, 24, 23, 0,
        0, 64, 65, 51, 52, 38, 25, 0,
        0, 0, 53, 0, 39, 26, 0, 0,
        42 /*CapsLock*/, 66 /*Rshift*/, 54 /*Enter*/, 40, 0, 41, 0, 0,
        0, 0, 0, 0, 0, 0, 27 /*PS2_BACKSPACE*/ , 0,
        0, 99, 0, 96, 92, 0, 0, 0,
        103, 104, 100, 97, 98, 93, 13, 88 /*NumLock*/,
        11, 95, 101, 91, 90, 94, 86, 0,
        0, 0, 0, 7 },
  // with shift
        {0, 9, 0, 5, 3, 1, 2, 12,
        0, 10, 8, 6, 4, 28, 14, 0,
        0, 69 /*Lalt*/, 55 /*Lshift*/, 0, 67 /*Lctrl*/, 29, 15, 0,
        0, 0, 56, 44, 43, 30, 16, 0,
        0, 58, 57, 45, 31, 18, 17, 0,
        0, 70, 59, 46, 33, 32, 19, 0,
        0, 61, 60, 48, 47, 34, 20, 0,
        0, 0, 62, 49, 35, 21, 22, 0,
        0, 63, 50, 36, 37, 24, 23, 0,
        0, 64, 65, 51, 52, 38, 25, 0,
        0, 0, 53, 0, 39, 26, 0, 0,
        42 /*CapsLock*/, 66 /*Rshift*/, 54 /*Enter*/, 40, 0, 41, 0, 0,
        0, 0, 0, 0, 0, 0, 27 /*PS2_BACKSPACE*/ , 0,
        0, 99, 0, 96, 92, 0, 0, 0,
        103, 104, 100, 97, 98, 93, 13, 88 /*NumLock*/,
        11, 95, 101, 91, 90, 94, 86, 0,
        0, 0, 0, 7 },
        0
};

void setup() {
	while (!Serial && millis() < 2000) ; // wait up to 2 sec for serial monitor
	Serial.println("Five Keyboard Test:");
	keyboard1.begin(23, 22, myKeymap);
	keyboard2.begin(21, 20, myKeymap);
	keyboard3.begin(3, 2, myKeymap);
	keyboard4.begin(8, 4, myKeymap);
	keyboard5.begin(14, 15, myKeymap);
	Serial.begin(9600);
}

void loop() {
	if (keyboard1.available()) {
		int n = keyboard1.read();
		Serial.print("keyboard1 = ");
		Serial.println(n);
		const uint8_t *p = wordlist[104*0+n-1];
		Serial.println(p[0], HEX);
	}
	if (keyboard2.available()) {
		int n = keyboard2.read();
		Serial.print("keyboard2 = ");
		Serial.println(n);
	}
	if (keyboard3.available()) {
		int n = keyboard3.read();
		Serial.print("keyboard3 = ");
		Serial.println(n);
	}
	if (keyboard4.available()) {
		int n = keyboard4.read();
		Serial.print("keyboard4 = ");
		Serial.println(n);
	}
	if (keyboard5.available()) {
		int n = keyboard5.read();
		Serial.print("keyboard5 = ");
		Serial.println(n);
		//Serial.println(*wordlist[104*4+n-1], HEX);
	}

}
