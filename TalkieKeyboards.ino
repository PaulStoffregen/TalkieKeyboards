#include <Talkie.h>
#include "PS2Key.h"
#include "words.h"

Talkie voice;

PS2Keyboard keyboard1;
PS2Keyboard keyboard2;
PS2Keyboard keyboard3;
PS2Keyboard keyboard4;
PS2Keyboard keyboard5;
PS2Keyboard keyboard6;
PS2Keyboard keyboard7;
PS2Keyboard keyboard8;

extern const PROGMEM PS2Keymap_t myKeymap;

void setup() {
	pinMode(18, INPUT_PULLUP);
	attachInterrupt(18, shutup, FALLING);
	pinMode(5, OUTPUT);
	digitalWrite(5, HIGH);               // turn on amplifier
	delay(10);
	//while (!Serial && millis() < 2000) ; // wait up to 2 sec for serial monitor
	Serial.println("Five Keyboard Test:");
	keyboard1.begin(23, 22, myKeymap);
	keyboard2.begin(21, 20, myKeymap);
	keyboard3.begin(3, 2, myKeymap);
	keyboard4.begin(8, 4, myKeymap);
	keyboard5.begin(14, 15, myKeymap);
	keyboard6.begin(16, 17, myKeymap);
	keyboard7.begin(0, 1, myKeymap);
	keyboard8.begin(10, 9, myKeymap);
	Serial.begin(9600);
}

void shutup()
{
	SCB_AIRCR = 0x05FA0004; // reset
}

void say(int keyboard, int key)
{
	Serial.print("keyboard: ");
	Serial.print(keyboard);
	Serial.print(", key: ");
	Serial.println(key);
	voice.sayQ(wordlist[(keyboard-1) * 104 + (key-1)]);
}

void loop() {
	if (keyboard1.available()) {
		say(1, keyboard1.read());
	}
	if (keyboard2.available()) {
		say(2, keyboard2.read());
	}
	if (keyboard3.available()) {
		say(3, keyboard3.read());
	}
	if (keyboard4.available()) {
		say(4, keyboard4.read());
	}
	if (keyboard5.available()) {
		say(5, keyboard5.read());
	}
	if (keyboard6.available()) {
		say(6, keyboard6.read());
	}
	if (keyboard7.available()) {
		say(7, keyboard7.read());
	}
	if (keyboard8.available()) {
		say(8, keyboard8.read());
	}
}


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
