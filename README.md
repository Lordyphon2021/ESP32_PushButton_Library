# ESP32_CPP_I-O

Experimental ESP32 Library for 16x Pushbutton Input with 74HC6067 Multiplexer or direct GPIO Input
and otional 74HC595 shift register serial output.

Buttons can be assigned either directly to an ESP32 Pin or to 74HC4067 Multiplexer Inputs ( 0 - 15 )
Overloaded constructors convert pin state logic accordingly.
    
    
 public button methods  : 
 
 shortpress() : 

     returns true if pin state changed (uC Pin: negative logic, internal pullup on, true if state change from 1 to 0, 
     MUX logic : true if state change from 0 to 1, external pull-downs required (10k))

 longpress(int presstime)  : 

     increments counter, returns true when counter reaches threshold, resets counter. presstime roughly ms.

 overloaded + operator: if(Button_1 + Button_2) returns true if both pressed. 

     debouncing via private button class method. debounce value can be omitted, default will deactivate method.

 Multiplexer Class: 
 
     "void poll_mux(void)" updates all 16 pin-states in Interrupt Service Routine, "bool get_pin_state(int mux_in)" forwards states to button class

     Data Output(soft serial class) to 595 shift register IC(s), bitbanged via serial-data pin, ck pin(clock) and rck pin(updates latched 595 output). 

     TODO: implement SPI-bus version...
 
 shift register constructor is overloaded: sending uint8_t or uint16_t data(second 595 IC can be daisychained) will select the corresponding send method.
