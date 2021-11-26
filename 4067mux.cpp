#include<arduino.h>
#include "4067mux.h"
#include "soft_serial.h"


MUX::MUX(int s0, int s1, int s2, int s3, int cio, int oe) //constructor
  :mux_pin_S0(s0),mux_pin_S1(s1),mux_pin_S2(s2),mux_pin_S3(s3), mux_common_io(cio),mux_pin_oe(oe)
{
    
    
    pinMode(mux_pin_S0, OUTPUT);                      // ESP32 hardware setup for multiplexer pins
    pinMode(mux_pin_S1, OUTPUT);
    pinMode(mux_pin_S2, OUTPUT);
    pinMode(mux_pin_S3, OUTPUT);
    pinMode(mux_common_io, INPUT);
    pinMode(mux_pin_oe, OUTPUT);
    
}
bool MUX::set_4bit_parallel_port(uint8_t data_out){  //creates 4-Bit parallel output port for ESP32, 
                                                     //used with poll_mux method in interrupt service routine

    digitalWrite(mux_pin_oe, HIGH);                  // disable mux output
    delayMicroseconds(1);
    
    digitalWrite(mux_pin_S0, data_out & 0x01);      // bit mask to output binary values 0 - 15 on selected pins
    digitalWrite(mux_pin_S1, data_out>>1 & 0x01);   
    digitalWrite(mux_pin_S2, data_out>>2 & 0x01);
    digitalWrite(mux_pin_S3, data_out>>3 & 0x01);
    
    digitalWrite(mux_pin_oe, LOW);                  // enable mux output 
    delayMicroseconds(1);                           // allow for settling time (bypass this and the whole thing will NOT work)
    
    return true;                                    // return true after new value appears on port.
  }

bool MUX::get_pin_state(int mux_in)                 // forwards multiplexer pin state to button class
{
    return buttonnumber_active[mux_in];
}

void MUX::poll_mux(void){                           // used in Interrupt Service Routine
  for(uint8_t i = 0x00; i < 0x10 ; ++i){            // i corresponds to all 16 button adresses on the multiplexer
    if(set_4bit_parallel_port(i) && digitalRead(mux_common_io))   //if(updated parallel port(values 0-15)== true && logical high on common-io pin)
        buttonnumber_active[i] = 1;                               //mux pin nr. (i) = 1
    else
        buttonnumber_active[i] = 0; 
  }
 
}
