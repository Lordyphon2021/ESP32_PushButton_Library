#pragma once


class MUX{

  public:
    
    MUX(int s0, int s1, int s2, int s3, int cio, int oe);
    bool buttonnumber_active[16]={0};
    
    void poll_mux(void);
  
  private:
     
    int mux_pin_S0;
    int mux_pin_S1;
    int mux_pin_S2;
    int mux_pin_S3;
    int mux_common_io;
    int mux_pin_oe;
 
    
    bool set_4bit_parallel_port(uint8_t);
    bool get_pin_state(int pinnum);
};
