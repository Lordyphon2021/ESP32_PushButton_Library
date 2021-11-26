
#include <Arduino.h>
#include "Push_button.h"

int pin_number;
    MUX* pMUX;
    bool pin_state;
    bool pin_last_state;
    long longpress_ctr;
    
    long debounce_ctr;
    int debounce_time;
    bool csto_sel_flag;

Push_button::Push_button(int pin, int debounce_ms)          //constructor for buttons wired to ESP32 (csto_flag = 0)
  :pin_number(pin), pMUX(nullptr), pin_state(0), pin_last_state(0), longpress_ctr(0)
  ,debounce_ctr(0), debounce_time(debounce_ms), csto_sel_flag(0)
{
     pinMode(pin_number, INPUT);          //ESP32 setup
     pinMode(pin_number, INPUT_PULLUP);
}

 
Push_button::Push_button(int mux_input, MUX& hc4067_mux, int debounce_ms)              //constructor for buttons connected to multiplexer (csto_flag = 1)
  : pin_number(mux_input), pMUX(&hc4067_mux), pin_state(0), pin_last_state(0), longpress_ctr(0)
  ,debounce_ctr(0), debounce_time(debounce_ms), csto_sel_flag(1)
{}




void Push_button::poll_input_pin(void){                  
     if( csto_sel_flag == 0 ){
         pin_state = !(digitalRead(pin_number));          //pin state via ESP32- pin 
        
     }
     else if( csto_sel_flag == 1 && pMUX){
         pin_state = pMUX -> get_pin_state(pin_number);   //pin state via multiplexer class
        
     }
}

void Push_button::button_debouncer(void){                 //private debouncing method
  
  debounce_ctr = 0;
  
  if(pin_state && debounce_time != 0){
     while(debounce_ctr < debounce_time * 1000){             
        if(pin_state)
          debounce_ctr++;
        else
          debounce_ctr = 0;
      }
    }
}

bool Push_button::shortpress(void){                       //public method
  bool pushed_flag{false};
  
  if(pin_state && !pin_last_state){
    button_debouncer();                                   //debounce
    longpress_ctr = 0; 
    pushed_flag = true;
  }
  pin_last_state = pin_state;                             //update pin state

  return pushed_flag;
}

bool Push_button::longpress(int presstime){               //public method

  bool pushed_flag = false;

  if(pin_state){
    longpress_ctr++;
  }
  if(longpress_ctr == presstime * 1000){                          
    pushed_flag = true;
    longpress_ctr = 0;
  }
  return pushed_flag;

}


bool Push_button::operator+(Push_button& other)         //overloaded "+" operator ( if(Button_1 + Button_2) {do something...} )
{
  
   return (pin_state && other.pin_state);
  
}

  
  
  
