# ESP32_CPP_I-O

Experimental ESP32 Library for 16x Pushbutton Input with 74HC6067 Multiplexer, direct Pin Input
and 74HC595 shift register serial output.

Push_button(int pin, int debounce_ms)          //constructor for button objects wired directly to ESP32 
Push_button(int mux_input, MUX& hc4067_mux, int debounce_ms)   //constructor for buttons connected to multiplexer 
