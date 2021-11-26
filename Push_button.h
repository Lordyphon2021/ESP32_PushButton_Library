#pragma once

class MUX;


class Push_button{

  private:
    int pin_number;
    bool pin_state;
    bool pin_last_state;
    long longpress_ctr;
    long debounce_ctr;
    bool csto_sel_flag;
    MUX* pMUX;
    
  private:  
    
    void button_debouncer(void);
    
    
  public:  
    
  
    Push_button(int pin);
    Push_button(int muxnum, MUX& hc4067_mux);
    
    void poll_input_pin(void);
    bool pressed(void);
    bool longpress(void);
    bool shortpress(void);
    bool both_pressed(Push_button& other);
    bool operator+(Push_button& other);
  
    
    
    
 }; 


     
   
