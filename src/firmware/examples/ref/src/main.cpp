#include <Arduino.h>

#include "ref_sys.h"

//		Timing variables
unsigned long top_time;
unsigned long cycle_time = 10000;

// Runs once
void setup() {
	Serial.begin(115200);

	if (Serial.available())
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

	curr_ref.read_serial();	

    curr_ref.hid_buff_write(arr);

	for(int i = 0; i <= 23; i++ ){
	Serial.print(i);
	Serial.print(" index   ");
    Serial.println(arr[i]);
	}

	
}