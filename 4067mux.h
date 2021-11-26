#pragma once


class MUX{

  public:
    
    MUX(int s0, int s1, int s2, int s3, int cio, int oe);
    
    void poll_mux(void);             // rotate through s0-s3 (values 0-15) during interrupt, store corresponding common-io result in button_number_active array
    bool get_pin_state(int mux_in);  //return input status from buttonnumber_active[16] for single pin number
 
  private:
       
    int mux_pin_S0;
    int mux_pin_S1;
    int mux_pin_S2;
    int mux_pin_S3;
    int mux_common_io;
    int mux_pin_oe;
 
    bool buttonnumber_active[16]={0}; 
    bool set_4bit_parallel_port(uint8_t);
    
   
};
