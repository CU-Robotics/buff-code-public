#include <Arduino.h>
#include <FlexCAN_T4.h>

#include "buff_can.h"

//		Timing variables
unsigned long top_time;
unsigned long cycle_time = 1000;


BuffCan buffcan;

byte buff[64];
byte msg[8];

// FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
// FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can2;

CAN_message_t can_input;

// Runs once
void setup() {
	Serial.begin(1000000);

	if (Serial)
		Serial.println("-- TEENSY SERIAL START --");

 	// Hardware setup
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	int16_t newPower = (int16_t)(0.1 * 30000);
  // Serial.print("newPower: ");
  // Serial.println(newPower);
    byte byteOne = highByte(newPower);
    byte byteTwo = lowByte(newPower);
    can_input.buf[4] = byteOne;
    can_input.buf[5] = byteTwo;
}

void blink(){
	static bool status = false;
	static unsigned long t = millis();

	if (millis() - t > 250){
		status = !status;
		t = millis();
	}

	digitalWrite(LED_BUILTIN, status);
}


// Runs continuously
void loop() {
	top_time = micros();

	buffcan.read_can1(buff);

	int16_t power = (int16_t)(0.3 * 16384);
	msg[0] = highByte(power);
	msg[1] = lowByte(power);
	Serial.println(power);
	// prettyprint_can_message(

	buffcan.set_input(2, 2, msg);

	// for (int i = 0; i < 4; i++) {
	// 	for (int j = 0; j < 16; j++){
	// 		Serial.print(buff[(i * 16) + j]);
	// 		Serial.print("\t");
	// 	}
	// 	Serial.println();
	// }
	// Serial.println();
	buffcan.write();

	blink();		

	while (micros() - top_time < cycle_time){}
}

