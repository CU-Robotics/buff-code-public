#include <Arduino.h>
#include <iostream>
#include "ref_sys.h"

/*
This code has been used with relative success on a ref system in competition. However, our data scheme is currently being
restructered and this code is changing along with that. I am currently changing where the data is headed once it is interepted from
serial input. I am also streamlining the code and chagning what we found is useless in comptetion. It should make the code faster in
the future.
*/



void ref_sys::init() {

    Serial2.begin(115200);
}

bool ref_sys::read_serial(){

    byte enter_code, temp;          //These 3 lines initialize all our temporary variables
    uint16_t data_length, cmd_id, unix_time, temp_hp, rem_proj, temp_max_hp, temp_stat;  
    uint8_t seq, crc, comp_stat, warning_level, robo_id, robot_level;
    uint32_t temp_launch_speed;

    // ref_sys curr_ref;           //Setting up an instance of our ref_sys class
   
    // put your main code here, to run repeatedly:
    if(Serial2.available() > 1){      //The first instance to check if serial data is available

        bool gotStartByte = false;

        while(Serial2.available() > 1){
            enter_code = Serial2.read();    //It will read in the first byte of data
            
            if(enter_code == 0xA5){         //It looks for this value that signifies that there is about to be a transmission of data
                gotStartByte = true;

                //Serial.println("Enter code received");

                while(Serial2.readBytes(&temp, 1) != 1){        //This waits till another byte of data is available
                }
                
                data_length = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){        //This waits till another byte of data is available
                }
            
                data_length = data_length | (temp << 8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                //Serial.println(data_length);

                while(Serial2.readBytes(&temp, 1) != 1){          //This waits till another byte of data is available
                }
                
                seq = temp;
                
                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                crc = temp;

                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                cmd_id = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                cmd_id = cmd_id | (temp << 8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                //Serial.println(cmd_id, HEX);

                if(cmd_id == 1){  //stage 1

                //Serial.println("received cmd_id inside 1"); 

                while(Serial2.readBytes(&temp, 1) != 1){   
                }        //This waits till another byte of data is available

                comp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left
                comp_stat = comp_stat >> 4;

                if(comp_stat == 0){
                    run_data.curr_stage = 'P';  //pre comp stage
                    //Serial.println("pre comp");
                }

                else if(comp_stat == 1){
                    run_data.curr_stage = 'S';   //Setup
                }

                else if(comp_stat == 2){
                    run_data.curr_stage = 'I';    //Init stage
                }

                else if(comp_stat == 3){
                    run_data.curr_stage = 'F';   //5 sec countdown
                }

                else if(comp_stat == 4){
                    run_data.curr_stage = 'C';   //In combat
                }

                else if(comp_stat == 5){
                    run_data.curr_stage = 'R';   //calc comp results
                }

                while(Serial2.readBytes(&temp, 1) != 1){   
                }

                unix_time = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    
                }        //This waits till another byte of data is available

                unix_time = unix_time | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                run_data.rem_time = int(unix_time);
                                    
                }else if(cmd_id == 3){   //everyone hp

                    //Serial.println("received cmd_id inside 3"); 

                    while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                while(Serial2.readBytes(&temp, 1) != 1){   
                }        //This waits till another byte of data is available

                /////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){  
                }        //This waits till another byte of data is available

                while(Serial2.readBytes(&temp, 1) != 1){            //skipping 2 bytes  
                }        //This waits till another byte of data is available

                ///////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){  
                }        //This waits till another byte of data is available

                while(Serial2.readBytes(&temp, 1) != 1){             
                }        //This waits till another byte of data is available

                ///////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){  
                }
                while(Serial2.readBytes(&temp, 1) != 1){  
                }
                while(Serial2.readBytes(&temp, 1) != 1){      //Skipping 4 bytes  
                }
                while(Serial2.readBytes(&temp, 1) != 1){ 
                }

                //////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){  
                }        //This waits till another byte of data is available

                temp_hp = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){              
                }        //This waits till another byte of data is available

                temp_hp = temp_hp | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                run_data.red_sentry_hp = temp_hp;

                ///////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){  
                }
                while(Serial2.readBytes(&temp, 1) != 1){  
                }
                while(Serial2.readBytes(&temp, 1) != 1){      //Skipping 4 bytes   
                }
                while(Serial2.readBytes(&temp, 1) != 1){   
                }

                //////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                while(Serial2.readBytes(&temp, 1) != 1){    
                }        //This waits till another byte of data is available

                /////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){   
                }
                while(Serial2.readBytes(&temp, 1) != 1){            //skipping 2 bytes   
                }

                ///////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available


                while(Serial2.readBytes(&temp, 1) != 1){            
                }        //This waits till another byte of data is available

                ///////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){
                }
                while(Serial2.readBytes(&temp, 1) != 1){   
                }
                while(Serial2.readBytes(&temp, 1) != 1){      //Skipping 4 bytes    
                }
                while(Serial2.readBytes(&temp, 1) != 1){   
                }

                //////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                temp_hp = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){         
                }        //This waits till another byte of data is available

                temp_hp = temp_hp | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                run_data.blue_sentry_hp = temp_hp;
               
                ///////////////////////////////////////////////////////////////
            
                }else if(cmd_id == 260){ //ref warning

                //Serial.println("received cmd_id inside 260"); 


                while(Serial2.readBytes(&temp, 1) != 1){  
                }        //This waits till another byte of data is available

                warning_level = temp;

                if(warning_level == 1){

                    run_data.ref_warning = 'Y';
                    
                }else if(warning_level == 2){

                    run_data.ref_warning = 'R';
                    
                }else if(warning_level == 3){

                    run_data.ref_warning = 'F';

                }

                while(Serial2.readBytes(&temp, 1) != 1){  
                }        //This waits till another byte of data is available

                robo_id = temp;

                run_data.foul_robot_id = int(robo_id);

                }else if(cmd_id == 201){ //robo stat

                //Serial.println("received cmd_id inside 201"); 

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                robo_id = temp;

                run_data.curr_robot_id = robo_id;

                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                robot_level = temp;

                run_data.curr_robot_level = robot_level;

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_hp = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                temp_hp = temp_hp | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                run_data.curr_robot_hp = temp_hp;

                ////////////////////////////////////////////////////////////////////////////
                
                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                while(Serial2.readBytes(&temp, 1) != 1){  
                }        //This waits till another byte of data is available

                ////////////////////////////////////////////////////////////////////////////
                
                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){          //Robot 1 cooling value read in 
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                run_data.curr_robot_17_cool_val = temp_stat;

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting robot one barrel heat limit 
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                run_data.curr_robot_17_heat_lim = temp_stat;

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting robot one speed limit 
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                run_data.curr_robot_17_speed_lim = temp_stat;

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){  
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting robot two cooling value  
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                if(run_data.curr_robot_id  ==  3 ||run_data.curr_robot_id  ==  4 ||run_data.curr_robot_id  ==  5 ||run_data.curr_robot_id  ==  7 ||
                run_data.curr_robot_id  ==  103 ||run_data.curr_robot_id  ==  104 ||run_data.curr_robot_id  ==  105 ||run_data.curr_robot_id  ==  107){
                
                run_data.second_wpn_cool_val = temp_stat;

                }

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting robot two barrel heat limit 
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                if(run_data.curr_robot_id  ==  3 ||run_data.curr_robot_id  ==  4 ||run_data.curr_robot_id  ==  5 ||run_data.curr_robot_id  ==  7 ||
                run_data.curr_robot_id  ==  103 ||run_data.curr_robot_id  ==  104 ||run_data.curr_robot_id  ==  105 ||run_data.curr_robot_id  ==  107){
                
                run_data.second_wpn_heat_lim = temp_stat;

                }

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting robot two speed limit
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                if(run_data.curr_robot_id  ==  3 ||run_data.curr_robot_id  ==  4 ||run_data.curr_robot_id  ==  5 ||run_data.curr_robot_id  ==  7 ||
                run_data.curr_robot_id  ==  103 ||run_data.curr_robot_id  ==  104 ||run_data.curr_robot_id  ==  105 ||run_data.curr_robot_id  ==  107){
                
                run_data.second_wpn_speed_lim = temp_stat;

                }

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting robot 42mm cooling value
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                if(run_data.curr_robot_id  ==  1 ||run_data.curr_robot_id  ==  101){
                

                run_data.second_wpn_cool_val = temp_stat;

                }

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting robot 42mm barrel heat limit  
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                if(run_data.curr_robot_id  ==  1 ||run_data.curr_robot_id  ==  101){

                run_data.second_wpn_heat_lim = temp_stat;

                }

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting robot 42mm speed limit 
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                if(run_data.curr_robot_id  ==  1 ||run_data.curr_robot_id  ==  101){

                run_data.second_wpn_speed_lim = temp_stat;

                }

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting robot power consumption limit 
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                run_data.robot_power_lim = temp_stat;

                /////////////////////////////////////////////////////////////////////////////
              
                }
                else if(cmd_id == 514){   //power, current and voltage   
                    
                //Serial.println("received cmd_id, inside 514"); 

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){  
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting chasis output voltage 
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                run_data.chasis_volt = temp_stat;    

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting chasis output current
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                run_data.chasis_current = temp_stat;

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_launch_speed = temp;

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available        //This section reads in 4 bytes and assigns them to one uint32 variable

                temp_launch_speed = temp_launch_speed | (temp<<8);
                // temp_launch_speed << 8;

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_launch_speed = temp_launch_speed | (temp<<8);
                // temp_launch_speed << 8;

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_launch_speed = temp_launch_speed | (temp<<8);
                // temp_launch_speed << 8;


                run_data.chasis_power = temp_launch_speed;

                /////////////////////////////////////////////////////////////////
                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting chasis output current
                }        //This waits till another byte of data is available

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting chasis output current
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                run_data.curr_robot_barr_heat = temp_stat;

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting chasis output current
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

               if(run_data.curr_robot_id  ==  3 ||run_data.curr_robot_id  ==  4 ||run_data.curr_robot_id  ==  5 ||run_data.curr_robot_id  ==  7 ||
                run_data.curr_robot_id  ==  103 ||run_data.curr_robot_id  ==  104 ||run_data.curr_robot_id  ==  105 ||run_data.curr_robot_id  ==  107){
                
                run_data.curr_robot_second_barr_heat = temp_stat;
                }

                ////////////////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){
                }        //This waits till another byte of data is available

                temp_stat = temp;     //Reading in a byte of data and bit shifting it 8 bits to the left

                while(Serial2.readBytes(&temp, 1) != 1){    //Setting chasis output current
                }        //This waits till another byte of data is available

                temp_stat = temp_stat | (temp<<8);       //Performing a bitwise or to join the 2 bytes into an 16 bit integer

                if(run_data.curr_robot_id  ==  1 ||run_data.curr_robot_id  ==  101){

                run_data.curr_robot_second_barr_heat = temp_stat;

                }

                ////////////////////////////////////////////////////////////////////////////

                
                                    
                }else if(cmd_id == 519){  //RT launch info

                //Serial.println("received cmd_id inside 519"); 

                ///////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available        //Skipping 1 byte of data

                ///////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp = temp >> 8;
                comp_stat = temp & 0b00000111;

                if(comp_stat ==  1){

                    run_data.launch_id = 1;

                }else if(comp_stat == 2){

                    run_data.launch_id = 2;

                }else if(comp_stat ==3){

                    run_data.launch_id = 3;

                }

                ///////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                comp_stat = int(temp);

                run_data.launch_freq = comp_stat;

                //////////////////////////////////////////////////////////////////

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_launch_speed = temp;

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available        //This section reads in 4 bytes and assigns them to one uint32 variable

                temp_launch_speed = temp_launch_speed | (temp<<8);
                // temp_launch_speed << 8;

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_launch_speed = temp_launch_speed | (temp<<8);
                // temp_launch_speed << 8;

                while(Serial2.readBytes(&temp, 1) != 1){ 
                }        //This waits till another byte of data is available

                temp_launch_speed = temp_launch_speed | (temp<<8);
                // temp_launch_speed << 8;

                /////////////////////////////////////////////////////////////////

                run_data.launch_speed = temp_launch_speed;
                
                }
        
        }

        }
        
  
  }
}

bool ref_sys::hid_buff_write(byte arr[]){

    *arr[0] = 3;
    *arr[1] = run_data.curr_stage;
    *arr[2] = run_data.rem_time;
    *arr[3] = run_data.ref_warning;
    *arr[4] = run_data.blue_sentry_hp;
    *arr[5] = run_data.red_sentry_hp;
    *arr[6] = run_data.foul_robot_id;
    *arr[7] = run_data.curr_robot_id;
    *arr[8] = run_data.curr_robot_level;
    *arr[9] = run_data.curr_robot_hp;
    *arr[10] = run_data.curr_robot_17_cool_val;
    *arr[11] = run_data.curr_robot_17_heat_lim;
    *arr[12] = run_data.second_wpn_cool_val;
    *arr[13] = run_data.second_wpn_heat_lim;
    *arr[14] = run_data.second_wpn_speed_lim;
    *arr[15] = run_data.chasis_volt;
    *arr[16] = run_data.chasis_current;
    *arr[17] = run_data.robot_power_lim;
    *arr[18] = run_data.chasis_power;
    *arr[19] = run_data.curr_robot_barr_heat;
    *arr[20] = run_data.curr_robot_second_barr_heat;
    *arr[21] = run_data.launch_id;
    *arr[22] = run_data.launch_speed;
    *arr[23] = run_data.launch_freq;

    return true;
}
