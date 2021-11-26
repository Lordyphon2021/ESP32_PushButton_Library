#include <Arduino.h>
#include "soft_serial.h"



soft_serial::soft_serial(int dat, int ck, int rck)       //constructor
      :data_pin(dat),ck_pin(ck),rck_pin(rck)
{ 
   pinMode(data_pin, OUTPUT);                            //ESP32 pin setup
   pinMode(ck_pin, OUTPUT);
   pinMode(rck_pin ,OUTPUT);
}

void soft_serial::send_serial_data(uint8_t send_data8){  //send 8 bit word
    for(int i = 0; i < 8; i++){
      if((send_data8 & 0x80) > 0 )                        //get most significant bit
        digitalWrite(data_pin, HIGH);
      else                                                //output bit on serial data pin
        digitalWrite(data_pin, LOW);
    
      digitalWrite(ck_pin, HIGH);                         //clock pulse
      digitalWrite(ck_pin, LOW);       
      send_data8 = send_data8 << 1;                       //shift next bit to send position
    }
    
    digitalWrite(rck_pin, HIGH);                          //update 595 parallel output with one clock pulse on rck pin
    digitalWrite(rck_pin, LOW);
  }


void soft_serial::send_serial_data(uint16_t send_data16){ //send 16 bit word, same logic
    for(int i = 0; i < 16; i++){
      if((send_data16 & 0x8000) > 0 )
        digitalWrite(data_pin, HIGH);
      else
        digitalWrite(data_pin, LOW);
    
      digitalWrite(ck_pin, HIGH);
      digitalWrite(ck_pin, LOW);       
      send_data16 = send_data16 << 1;
    }
    digitalWrite(rck_pin, HIGH);                          
    digitalWrite(rck_pin, LOW);
}  
