#include "PS2Key.h"

PS2Keyboard keyboard1;
PS2Keyboard keyboard2;
PS2Keyboard keyboard3;
PS2Keyboard keyboard4;
PS2Keyboard keyboard5;

void setup() {
	while (!Serial && millis() < 2000) ; // wait up to 2 sec for serial monitor
	Serial.println("Five Keyboard Test:");
	keyboard1.begin(23, 22);
	keyboard2.begin(21, 20);
	keyboard3.begin(3, 2);
	keyboard4.begin(8, 4);
	keyboard5.begin(14, 15);
	Serial.begin(9600);
}

void loop() {
	if (keyboard1.available()) {
		int n = keyboard1.read();
		Serial.print("keyboard1 = ");
		Serial.println(n);
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
	}

}
