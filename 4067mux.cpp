#include<arduino.h>
#include "4067mux.h"
#include "soft_serial.h"


MUX::MUX(int s0, int s1, int s2, int s3, int cio, int oe)
  :mux_pin_S0(s0),mux_pin_S1(s1),mux_pin_S2(s2),mux_pin_S3(s3), mux_common_io(cio),mux_pin_oe(oe)
{
    
    
    pinMode(mux_pin_S0, OUTPUT);
    pinMode(mux_pin_S1, OUTPUT);
    pinMode(mux_pin_S2, OUTPUT);
    pinMode(mux_pin_S3, OUTPUT);
    pinMode(mux_common_io, INPUT);
    pinMode(mux_pin_oe, OUTPUT);
    
}
bool MUX::set_4bit_parallel_port(uint8_t data_out){

    digitalWrite(mux_pin_oe, HIGH);
    //delayMicroseconds(1);
    
    digitalWrite(mux_pin_S0,  data_out & 0x01);
    digitalWrite(mux_pin_S1, (data_out & 0x02 >> 1));
    digitalWrite(mux_pin_S2, (data_out & 0x04 >> 2));
    digitalWrite(mux_pin_S3, (data_out & 0x08 >> 3));
    
    digitalWrite(mux_pin_oe, LOW);
    delayMicroseconds(2);
    
    return true;
  }

bool MUX::get_pin_state(int pinnum)
{
    return buttonnumber_active[pinnum];
}




void MUX::poll_mux(void){
  for(int i = 0; i < 16 ; ++i){
    
    if(set_4bit_parallel_port(i) && digitalRead(mux_common_io))
        buttonnumber_active[i] = 1;
    else
        buttonnumber_active[i] = 0; 
  }
 
}
