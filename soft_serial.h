#pragma once


class soft_serial{

  private:
    
    uint8_t send_data8;
    uint16_t send_data16;
    int data_pin{0};
    int ck_pin{0};
    int rck_pin{0};
    
    
  public:  
  
    soft_serial(int,int,int);
    void send_serial_data(uint8_t);  
    void send_serial_data(uint16_t);

};
