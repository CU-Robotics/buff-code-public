#include <Arduino.h>

#include "ref_sys.h"

DR16 receiver;

//		Timing variables
unsigned long top_time;
unsigned long cycle_time = 10000;

// Runs once
void setup() {
	Serial.begin(1000000);

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
	
    ref_data curr_data;

    ref_sys curr_ref;

	top_time = micros();

	blink();		

	curr_ref.read();	// Read from the dr16 rx


	if (micros() - top_time > cycle_time){
		Serial.print("Over the Cycle Limit: ");
		Serial.println(micros() - top_time);
	}
	while (micros() - top_time < cycle_time){}
}