
#include "Push_button.h"
#include "soft_serial.h"
#include "4067mux.h"


 
 
 MUX hc4067_mux_1(32, 33, 34, 35, 4, 27);   //(S0, S1, S2, S3, common_io, !E) 
 soft_serial soft_serial_port(16, 17, 18);  //(data, ck, rck)
 soft_serial mux_serial_port(25, 26, 27);  //(data, ck, rck)

 Push_button Button_1(1, hc4067_mux_1);      //create button objects with mux input
 Push_button Button_2(2, hc4067_mux_1);
 Push_button Button_3(3, hc4067_mux_1);
 Push_button Button_4(4, hc4067_mux_1);

 Push_button Button_5 (22);                   // button objects with uC pins
 Push_button Button_6 (23);                                             
 
 uint16_t toggler = 0;
 uint16_t data_transfer = 1;
 volatile int interruptCounter;
 int totalInterruptCounter;
 hw_timer_t * timer = NULL;
 portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
   portENTER_CRITICAL_ISR(&timerMux);
   hc4067_mux_1.poll_mux(); 
  
   interruptCounter++;
   portEXIT_CRITICAL_ISR(&timerMux);
 
}


void setup(){
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100000, true);
  timerAlarmEnable(timer);
 }



void loop(){
   
   Button_1.poll_input_pin();
   Button_2.poll_input_pin();
   Button_3.poll_input_pin();
   Button_4.poll_input_pin();
   Button_5.poll_input_pin();
   Button_6.poll_input_pin();
  
  
  
   
    if(Button_1.shortpress()){
      data_transfer *= 2;
      
      if(data_transfer >= 128)
        data_transfer = 128;
      
      soft_serial_port.send_serial_data(data_transfer);
    }
    if(Button_2.shortpress()){
      data_transfer /= 2;
      
      if(data_transfer <= 1)
        data_transfer = 1;
      
      soft_serial_port.send_serial_data(data_transfer);
    }
    if(Button_3.longpress()){
      toggler = 0xff;
      soft_serial_port.send_serial_data(toggler);
    }
   
    
    if(Button_4.longpress()){
      toggler = 0x00;
      soft_serial_port.send_serial_data(toggler);
    }
    
    if(Button_5 + Button_6){
      toggler = 0x77;
      soft_serial_port.send_serial_data(toggler);
      
    }
    if(Button_6.shortpress()){
      toggler = 0x0f;
      soft_serial_port.send_serial_data(toggler);
      
    }
   
}
