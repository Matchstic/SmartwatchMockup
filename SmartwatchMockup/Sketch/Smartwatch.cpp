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

#define LCD_WIDTH 144
#define LCD_HEIGHT 168

#define SCK 13
#define MOSI 11
#define SS 10

Adafruit_SharpMem display(SCK, MOSI, SS, LCD_WIDTH, LCD_HEIGHT);

SuperApplication *superApplication;

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
}

void Smartwatch::loop() {
    // Start by clearing the last framebuffer.
    display.clearDisplay();
    
    // TODO: Forward button events through to all components.
    
    // Send the current time through to all components.
    superApplication->update(millis());
    
    // Do any drawing.
    superApplication->draw();

    // And, flush framebuffer to the LCD!
    display.refresh();
}
