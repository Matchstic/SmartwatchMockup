//
//  Button.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 21/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef Button_hpp
#define Button_hpp

#include <stdio.h>
#include <cstdint>

class Button {
public:
    Button(uint8_t pin, uint8_t puEnable, uint8_t invert, uint32_t dbTime);
    uint8_t read();
    uint8_t isPressed();
    uint8_t isReleased();
    uint8_t wasPressed();
    uint8_t wasReleased();
    uint8_t pressedFor(uint32_t ms);
    uint8_t releasedFor(uint32_t ms);
    uint32_t lastChange();
    
private:
    uint8_t _pin;           //arduino pin number
    uint8_t _puEnable;      //internal pullup resistor enabled
    uint8_t _invert;        //if 0, interpret high state as pressed, else interpret low state as pressed
    uint8_t _state;         //current button state
    uint8_t _lastState;     //previous button state
    uint8_t _changed;       //state changed since last read
    uint32_t _time;         //time of current state (all times are in ms)
    uint32_t _lastChange;   //time of last state change
    uint32_t _dbTime;       //debounce time
};

#endif /* Button_h */
