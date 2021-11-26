#pragma once
#include "4067mux.h"




class Push_button{

  private:
    int pin_number;
    MUX* pMUX;
    bool pin_state;
    bool pin_last_state;
    long longpress_ctr;
    long debounce_ctr;
    int debounce_time;
    bool csto_sel_flag;
   
    
  private:  
    
    void button_debouncer(void);
    
    
  public:  
    
  
    Push_button(int pin, int debounce_ms = 0); //constructor ESP-Pin
    Push_button(int mux_input, MUX& hc4067_mux, int debounce_ms = 0); //constructor Multiplexer Input
    
    //main function interface:
    void poll_input_pin(void);
    bool longpress(int presstime);
    bool shortpress(void);
    bool operator+(Push_button& other);
    
  
}; 


     
   
