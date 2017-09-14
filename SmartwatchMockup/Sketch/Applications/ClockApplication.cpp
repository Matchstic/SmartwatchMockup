//
//  ClockApplication.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 14/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "ClockApplication.hpp"
#include "ImageView.hpp"
#include "Label.hpp"
#include "FreeSans24pt7b.h"
#include "FreeSansBold24pt7b.h"
#include <ctime>

// try drawing a thing.
const uint8_t aClogoBitmaps[] =
{
    0x00, 0x07, 0xFC, 0x00, 0x00, //              #########
    0x00, 0x3F, 0xFF, 0x80, 0x00, //           ###############
    0x00, 0x7C, 0x03, 0xE0, 0x00, //          #####        #####
    0x01, 0xE0, 0x00, 0xF0, 0x00, //        ####             ####
    0x03, 0x80, 0x00, 0x38, 0x00, //       ###                 ###
    0x07, 0x00, 0x00, 0x0E, 0x00, //      ###                    ###
    0x0E, 0x00, 0x00, 0x06, 0x00, //     ###                      ##
    0x1C, 0x00, 0x00, 0x03, 0x00, //    ###                        ##
    0x38, 0x00, 0x00, 0x03, 0x80, //   ###                         ###
    0x30, 0x00, 0x00, 0x01, 0x80, //   ##                           ##
    0x70, 0x00, 0x00, 0x00, 0xC0, //  ###                            ##
    0x60, 0x38, 0x00, 0x00, 0xC0, //  ##       ###                   ##
    0x60, 0xFE, 0x00, 0x00, 0x60, //  ##     #######                  ##
    0xC1, 0xC7, 0x00, 0x00, 0x60, // ##     ###   ###                 ##
    0xC1, 0x83, 0x00, 0x00, 0x60, // ##     ##     ##                 ##
    0xC3, 0x01, 0x80, 0x00, 0x60, // ##    ##       ##                ##
    0xC7, 0x01, 0xC0, 0x00, 0x60, // ##   ###       ###               ##
    0xC6, 0x00, 0xE0, 0x00, 0x30, // ##   ##         ###               ##
    0xC0, 0x00, 0x60, 0x0C, 0x30, // ##               ##         ##    ##
    0xC0, 0x00, 0x30, 0x0C, 0x60, // ##                ##        ##   ##
    0xC0, 0x00, 0x38, 0x18, 0x60, // ##                ###      ##    ##
    0xC0, 0x00, 0x1C, 0x38, 0x60, // ##                 ###    ###    ##
    0xC0, 0x00, 0x0E, 0x70, 0x60, // ##                  ###  ###     ##
    0x60, 0x00, 0x07, 0xE0, 0x60, //  ##                  ######      ##
    0x60, 0x00, 0x01, 0x80, 0xC0, //  ##                    ##       ##
    0x70, 0x00, 0x00, 0x00, 0xC0, //  ###                            ##
    0x30, 0x00, 0x00, 0x01, 0x80, //   ##                           ##
    0x18, 0x00, 0x00, 0x03, 0x80, //    ##                         ###
    0x1C, 0x00, 0x00, 0x07, 0x00, //    ###                       ###
    0x0E, 0x00, 0x00, 0x0E, 0x00, //     ###                     ###
    0x07, 0x00, 0x00, 0x1C, 0x00, //      ###                   ###
    0x03, 0x80, 0x00, 0x38, 0x00, //       ###                 ###
    0x01, 0xE0, 0x00, 0xF0, 0x00, //        ####             ####
    0x00, 0x7C, 0x07, 0xC0, 0x00, //          #####       #####
    0x00, 0x1F, 0xFF, 0x00, 0x00, //            #############
    0x00, 0x03, 0xF8, 0x00, 0x00, //               #######
};

ClockApplication::ClockApplication(Size displaySize) : Application(displaySize) {
    viewDidLoad();
}

ClockApplication::~ClockApplication() {
    delete minuteLabel;
    delete hourLabel;
}


void ClockApplication::viewDidLoad() {
    // TODO: Build UI.
    printf("View did load\n");
    
    hourLabel = new Label(rectMake(47, 58, getDisplaySize().width, 24));
    hourLabel->setText("HOUR");
    hourLabel->setColor(WHITE, BLACK);
    hourLabel->setFont(&FreeSansBold24pt7b, 12, 7);
    
    getRootView()->addSubview(hourLabel);
    
    minuteLabel = new Label(rectMake(47, 98, getDisplaySize().width, 24));
    minuteLabel->setText("MINUTE");
    minuteLabel->setColor(WHITE, BLACK);
    minuteLabel->setFont(&FreeSans24pt7b, 12, 7);
    
    getRootView()->addSubview(minuteLabel);
    
    secondsDot = new Circle(rectMake(0, 0, 12, 12));
    secondsDot->setColor(WHITE);
    
    getRootView()->addSubview(secondsDot);
}

void ClockApplication::update(unsigned long timestamp) {
    // Call super for animations!
    Application::update(timestamp);
    
    // Ask the current watchface to update it's UI.
    
    /*static int x = 0;
    static int y = 0;
    static int xdirection = 1;
    static int ydirection = 1;
    
    static unsigned long lastTime = timestamp;
    
    unsigned long elapsed = timestamp - lastTime;
    int diff = (int)elapsed / 10;
    if (diff > 0)
        lastTime = timestamp;
    
    // Handle direction flag.
    if (x <= 0 && !xdirection) {
        xdirection = 1;
        x = 0;
    } else if (x >= getDisplaySize().width - 100 && xdirection) {
        xdirection = 0;
        x = getDisplaySize().width - 100;
    }
    
    if (y <= 0 && !ydirection) {
        ydirection = 1;
        y = 0;
    } else if (y >= getDisplaySize().height - 100 && ydirection) {
        ydirection = 0;
        y = getDisplaySize().height - 100;
    }
    
    x = xdirection == 1 ? x + diff : x - diff;
    y = ydirection == 1 ? y + diff : y - diff;
    
    // Set the frame for our root view.
    testingView->setFrame(rectMake(x, y, 100, 100));*/
    
    // Convert timestamp to hours and minutes.
    time_t rawtime = (time_t)timestamp / 1000;
    struct tm *dt = localtime(&rawtime);
    
    int hour = dt->tm_hour;
    int minute = dt->tm_min;
    int secs = dt->tm_sec;
    
    char str[3];
    sprintf(str, hour < 10 ? "0%d" : "%d", hour);
    
    hourLabel->setText(str);
    
    char str2[3];
    sprintf(str2, minute < 10 ? "0%d" : "%d", minute);
    
    minuteLabel->setText(str2);
    
    // Work out where the seconds dot should go.
    
    // 2Pi radians in a circle, easy, right?
    float a = (6.2832 / 60.0) * secs;
    a -= 1.57079632679; // Bump back by 1/2 Pi, so we hit 12 o'clock when a new minute starts.
    
    int xr = (getDisplaySize().width - 12) / 2;
    int yr = (getDisplaySize().height - 12) / 2;
    int x = xr + xr * cos(a);
    int y = xr + yr * sin(a);
    int yOffset = (getDisplaySize().height - getDisplaySize().width) / 2;
    
    secondsDot->setFrame(rectMake(x, y + yOffset, 12, 12));
    
    if (secs == 0) {
        
    }
}

void ClockApplication::onButtonPress(int button) {
    // As we are the primary application, we need to trigger the moving to either:
    // - Menu
    // - Application from a notification.
    // That's all on the action button.
    
    
}

void ClockApplication::onButtonLongPress(int button) {
    
}
