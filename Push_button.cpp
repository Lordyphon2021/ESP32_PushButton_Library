
#include <Arduino.h>
#include "Push_button.h"
#include "4067mux.h"


Push_button::Push_button(int pin)
  :pin_number(pin), pin_state(0), pin_last_state(0), longpress_ctr(0)
  ,debounce_ctr(0), csto_sel_flag(0), pMUX(nullptr)
{
     
     
     pinMode(pin_number, INPUT);
     pinMode(pin_number, INPUT_PULLUP);
}

 
Push_button::Push_button(int muxnum, MUX& hc4067_mux)
  : pin_number(muxnum), pin_state(0), pin_last_state(0), longpress_ctr(0)
  ,debounce_ctr(0), csto_sel_flag(1), pMUX(&hc4067_mux)
{}

void Push_button::poll_input_pin(void){
     if( csto_sel_flag == 0 ){
         pin_state = !(digitalRead(pin_number));
        
     }
     if( csto_sel_flag == 1 && pMUX){
         pin_state = pMUX -> buttonnumber_active[pin_number];
     }
}

void Push_button::button_debouncer(void){
  debounce_ctr = 0;
   if(pin_state){
     while(debounce_ctr < 1000){ 
        if(pin_state)
          debounce_ctr++;
        else
          debounce_ctr = 0;
      }
    }
}

bool Push_button::shortpress(void){
  bool pushed_flag{false};
  
  if(pin_state && !pin_last_state){
    //button_debouncer();
    longpress_ctr = 0; 
    pushed_flag = true;
  }
  pin_last_state = pin_state;

  return pushed_flag;
}

bool Push_button::longpress(void){

  bool pushed_flag = false;

  if(pin_state){
    longpress_ctr++;
  }
  if(longpress_ctr == 1000000){
    pushed_flag = true;
    longpress_ctr = 0;
  }
  return pushed_flag;

}


bool Push_button::operator+(Push_button& other)
{
  
   return (pin_state && other.pin_state);
  
}

  
  
  
