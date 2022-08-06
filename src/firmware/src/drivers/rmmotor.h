#include <Arduino.h>
#include <FlexCAN_T4.h>

#include "algorithms/Buffers.h"


#ifndef RMMOTOR_H
#define RMMOTOR_H

class RMMotor {
	private:
        int id = -1;
		unsigned int byte_num = -1;
        unsigned int canBusID = -1; 
        CAN_message_t* sendMsgPtr;
        CAN_message_t* recMsgPtr;

    public:
        RMMotor();
    	RMMotor(CAN_message_t*, CAN_message_t*, int, unsigned int, unsigned int);
        void setPower(float);
        int read(HIDBuffer*);

};

struct Motor_LUT{
    // setup CAN and motor-Look-Up-Table
    FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
    FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can2;

    CAN_message_t canReceiveMessages[3][11];
    CAN_message_t canSendMessages[6][2];

    RMMotor motors[30];
};


void new_motor(Motor_LUT*, int, unsigned int, unsigned int, unsigned int);
void initCAN(Motor_LUT*);
void writeCAN(Motor_LUT*);

#endif