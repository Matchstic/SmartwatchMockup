//
//  Smartwatch.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 11/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "Smartwatch.hpp"
#include "Adafruit_SharpMem.hpp"
#include "Arduino.h"
#include "SuperApplication.hpp"
#include "Button.h"

#define LCD_WIDTH 144
#define LCD_HEIGHT 168

#define LONG_PRESS 500 //We define a "long press" to be 500 milliseconds.

#define SCK 13
#define MOSI 11
#define SS 10

Adafruit_SharpMem display(SCK, MOSI, SS, LCD_WIDTH, LCD_HEIGHT);

SuperApplication *superApplication;

#define MBUT 3
#define UBUT 4
#define DBUT 5
Button buttonMid(MBUT, true, false, 20);
Button buttonUp(UBUT, true, false, 20);
Button buttonDown(DBUT, true, false, 20);

typedef enum : int {
    kButtonIdle,
    kButtonPressed,
    kButtonLongPressed
} ButtonState;

ButtonState midState = kButtonIdle;
ButtonState upState = kButtonIdle;
ButtonState downState = kButtonIdle;

void Smartwatch::setup() {
    
    // Setup the display.
    display.begin();
    display.setRotation(0);
    display.clearDisplay();
    
    // Give access to the display for drawing in views.
    View::initialise(&display);
    
    // Initialise our "SpringBoard"
    superApplication = new SuperApplication(sizeMake(LCD_WIDTH, LCD_HEIGHT));
    
    // Setup background colour.
    display.fillRect(0, 0, LCD_WIDTH, LCD_HEIGHT, BLACK);
    
    // Read button values.
    buttonMid.read();
    buttonUp.read();
    buttonDown.read();
}

void Smartwatch::loop() {
    // Start by clearing the last framebuffer.
    display.clearDisplay();
    
    // Read button values.
    buttonMid.read();
    buttonUp.read();
    buttonDown.read();
    
    // Update button states.
    if (buttonMid.isPressed()) {
        midState = buttonMid.pressedFor(LONG_PRESS) ? kButtonLongPressed : kButtonPressed;
    }
    if (buttonUp.isPressed()) {
        upState = buttonUp.pressedFor(LONG_PRESS) ? kButtonLongPressed : kButtonPressed;
    }
    if (buttonDown.isPressed()) {
        downState = buttonDown.pressedFor(LONG_PRESS) ? kButtonLongPressed : kButtonPressed;
    }
    
    // Forward end state through to all components.
    if (buttonMid.wasReleased()) {
        if (midState == kButtonLongPressed) {
            superApplication->onButtonLongPress(kAction);
        } else {
            superApplication->onButtonPress(kAction);
        }
        midState = kButtonIdle;
    }
    
    if (buttonUp.wasReleased()) {
        if (upState == kButtonLongPressed) {
            superApplication->onButtonLongPress(kTop);
        } else {
            superApplication->onButtonPress(kTop);
        }
        upState = kButtonIdle;
    }
    
    if (buttonDown.wasReleased()) {
        if (downState == kButtonLongPressed) {
            superApplication->onButtonLongPress(kBottom);
        } else {
            superApplication->onButtonPress(kBottom);
        }
        downState = kButtonIdle;
    }
    
    // Send the current time through to all components.
    superApplication->update(millis());
    
    // Do any drawing.
    superApplication->draw();

    // And, flush framebuffer to the LCD!
    display.refresh();
}
