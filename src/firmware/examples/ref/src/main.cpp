#include <Arduino.h>

#include "ref_sys.h"

//		Timing variables
unsigned long top_time;
unsigned long cycle_time = 10000;

// Runs once
void setup() {
	Serial.begin(9600);

	if (Serial)
		Serial.println("-- TEENSY SERIAL START --");
    
 	// Hardware setup
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
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
	
    byte arr[64];

    ref_sys curr_ref;

	top_time = micros();

	blink();		

	curr_ref.read_serial();	// Read from the dr16 rx

    curr_ref.hid_buff_write(arr);

    Serial.println("Current stage");
    Serial.println(arr[1]);


	if (micros() - top_time > cycle_time){
		Serial.print("Over the Cycle Limit: ");
		Serial.println(micros() - top_time);
	}
	while (micros() - top_time < cycle_time){}
}