//
//  Button.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 21/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "Button.h"
#include "Arduino.h"
#import <CoreGraphics/CoreGraphics.h>

uint8_t digitalRead(uint8_t pin) {
    // here, we'll read from the keyboard instead.
    
    // Pin # + 11 corresponds to keyboard number.
    pin += 0x11;
    //printf("Reading for: %x\n", pin);
    
    return CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState, pin);
}

Button::Button(uint8_t pin, uint8_t puEnable, uint8_t invert, uint32_t dbTime) {
    _pin = pin;
    _puEnable = puEnable;
    _invert = invert;
    _dbTime = dbTime;
    
    _state = digitalRead(_pin);
    
    if (_invert != 0) _state = !_state;
    _time = (uint32_t)millis();
    _lastState = _state;
    _changed = 0;
    _lastChange = _time;
}

uint8_t Button::read() {
    static uint32_t ms;
    static uint8_t pinVal;
    
    ms = (uint32_t)millis();
    pinVal = digitalRead(_pin);
    if (_invert != 0) pinVal = !pinVal;
    if (ms - _lastChange < _dbTime) {
        _time = ms;
        _changed = 0;
        return _state;
    }
    else {
        _lastState = _state;
        _state = pinVal;
        _time = ms;
        if (_state != _lastState)   {
            _lastChange = ms;
            _changed = 1;
        }
        else {
            _changed = 0;
        }
        return _state;
    }
}

uint8_t Button::isPressed() {
    return _state == 0 ? 0 : 1;
}

uint8_t Button::isReleased() {
    return _state == 0 ? 1 : 0;
}

uint8_t Button::wasPressed(void) {
    return _state && _changed;
}

uint8_t Button::wasReleased(void) {
    return !_state && _changed;
}

/*----------------------------------------------------------------------*
 * pressedFor(ms) and releasedFor(ms) check to see if the button is     *
 * pressed (or released), and has been in that state for the specified  *
 * time in milliseconds. Returns false (0) or true (1) accordingly.     *
 * These functions do not cause the button to be read.                  *
 *----------------------------------------------------------------------*/
uint8_t Button::pressedFor(uint32_t ms) {
    return (_state == 1 && _time - _lastChange >= ms) ? 1 : 0;
}

uint8_t Button::releasedFor(uint32_t ms) {
    return (_state == 0 && _time - _lastChange >= ms) ? 1 : 0;
}

/*----------------------------------------------------------------------*
 * lastChange() returns the time the button last changed state,         *
 * in milliseconds.                                                     *
 *----------------------------------------------------------------------*/
uint32_t Button::lastChange(void) {
    return _lastChange;
}
