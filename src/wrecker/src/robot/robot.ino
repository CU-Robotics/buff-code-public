#ifndef _FLEXCAN_T4_H_
#include <FlexCAN_T4.h>
#endif

#include "c620.h"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
c620CAN controller1 = c620CAN(1, &sendMsg);

CAN_message_t sendMsg;
CAN_message_t recMsg;

short angle;
short torque;
short rpm;


c620CAN myMotor(1, &msg);
// c620 myMotor();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  can1.begin();
  can1.setBaudRate(1000000);
  Serial.begin(9600);
}

void loop() {
  

  delay(10);
}
