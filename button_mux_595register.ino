
/*REUSABLE CODEBASE FOR ESP32 PUSH-BUTTON INPUT 
 * -VIA uC-PIN OR 74HC4067 16 to 4 MULTIPLEXERS 
 * -8 or 16 Bit 595 SHIFT REGISTER OUTPUT 
 * 
 * Author: Stefan Plattner-Deisenberger, 2021
   * 
   * Buttons can be assigned either directly to an ESP32 Pin or to 74HC4067 Multiplexer Inputs ( 0 - 15 )
   * Overloaded constructors convert pin state logic accordingly.
   * 
   * 
   * public button methods  : -shortpress() : returns true if pin state changed (uC Pin: negative logic, internal pullup on, true if state change from 1 to 0, 
   *                                          MUX logic : true if state change from 0 to 1, external pull-downs required (10k))
   *                          -longpress(int presstime)  : increments counter, returns true when counter reaches threshold,
   *                                          resets counter. presstime roughly ms.
   *                          -overloaded + operator: if(Button_1 + Button_2) returns true if both pressed. 
   *                          -debouncing via private button class method. debounce value can be omitted, default will deactivate method.
   *
   *Multiplexer Class: "void poll_mux(void)" updates all 16 pin-states in Interrupt Service Routine, "bool get_pin_state(int mux_in)" forwards states to button class
   *
   *Data Output(soft serial class) to 595 shift register IC(s), bitbanged via serial-data pin, ck pin(clock) and rck pin(updates latched 595 output). 
   *      TODO: implement SPI-bus version...
   *-shift register constructor is overloaded: sending uint8_t or uint16_t data(second 595 IC can be daisychained) will select the corresponding send method.
   *
   *
   */
//(main function)

#include "Push_button.h"
#include "soft_serial.h"
#include "4067mux.h"

 MUX hc4067_mux_1(15, 16, 17, 18, 2, 4);     //create multiplexer object (S0, S1, S2, S3, common_io, !OutputEnable) , push buttons connected to mux inputs 0 - 15
 soft_serial soft_serial_port(19, 21, 22);   //create shift register object (data, ck, rck)
 
 //create button objects via multiplexer input pins (0 - 15)
 //format: Push_button Name (Multiplexer input pin, Multiplexer object, debounce_time(roughly ms)) // (int, MUX&, int=0)
 Push_button Button_1(0, hc4067_mux_1, 100);     
 Push_button Button_2(1, hc4067_mux_1, 100);
 Push_button Button_3(2, hc4067_mux_1, 100);
 Push_button Button_4(3, hc4067_mux_1);     // debounce time can be omitted


// create button objects via ESP32 pins (pin_number, debounce_time)
 Push_button Button_5 (23, 100);                  
 Push_button Button_6 (13);                 // debounce time can be omitted                          
 
 uint16_t toggler = 0;                       // 595 send data 
 uint16_t data_transfer = 0;                 // ---"---
 
 volatile int interruptCounter;
 int totalInterruptCounter;
 hw_timer_t * timer = NULL;                 
 portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {                  // Interrupt Service Routine     
   portENTER_CRITICAL_ISR(&timerMux);
   
   hc4067_mux_1.poll_mux();                 // scans mux inputs 0-15 and stores button states in C-style array (bool buttonnumber_active[16])
  
   interruptCounter++;
   portEXIT_CRITICAL_ISR(&timerMux);
 
}


void setup(){                                             // Timer Setup
  
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100000, true);                   // sets interrupt rate
  timerAlarmEnable(timer);
 
 }



void loop(){
   
   Button_1.poll_input_pin();                             //update button states in loop
   Button_2.poll_input_pin();                             //will check either ESP32 pin or MUX pin state array
   Button_3.poll_input_pin();
   Button_4.poll_input_pin();
   Button_5.poll_input_pin();
   Button_6.poll_input_pin();
  
  
  // test code
   
    if(Button_1.shortpress()){                            //if pressed
      data_transfer *= 2;                                 //do something....
      
      if(data_transfer >= 128)
        data_transfer = 128;
      
      soft_serial_port.send_serial_data(data_transfer);   // send data to 595 shift register
    }
    if(Button_2.shortpress()){
      data_transfer /= 2;
      
      if(data_transfer <= 1)
        data_transfer = 1;
      
      soft_serial_port.send_serial_data(data_transfer);
    }
    if(Button_3.longpress(1000)){
      toggler = 0xff;
      soft_serial_port.send_serial_data(toggler);
    }
   
    
    if(Button_4.longpress(1000)){
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
