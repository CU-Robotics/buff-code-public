#include <Arduino.h>
#include <FlexCAN_T4.h>

#ifndef BUFFHID_H
#define BUFFHID_H


struct HID_Packet {
  byte data[64];
};

int8_t write_HID(HID_Packet*); // writes HID packet
int8_t read_HID(HID_Packet*); // read HID packet

void set_report_id(HID_Packet*, uint8_t);
int get_report_id(HID_Packet* hid);

void set_device_output(HID_Packet*, byte*, int, int);
void get_device_input(HID_Packet*, byte*, int, int);

void set_can_output(HID_Packet*, CAN_message_t*);
int get_can_input(HID_Packet*, CAN_message_t*);

void parse_can1_output(HID_Packet*, CAN_message_t*);
void parse_can2_output(HID_Packet*, CAN_message_t*);


#endif