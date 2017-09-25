//
//  Application.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 12/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "Application.hpp"
#include "FreeSans9pt7b.h"

Application::Application(/*NotificationManager*/ Size displaySize) {
    _displaySize = displaySize;
    _rootView = new View(rectMake(0, 0, displaySize.width, displaySize.height));
    
    this->viewDidLoad();
}

Application::~Application() {
    delete _rootView;
}

void Application::draw() {
    // We want to draw the root view, but ONLY if we are not backgrounded.
    printf("draw?");
    
    if (getApplicationState() != kBackground) {
        printf("draw!");
        getRootView()->draw(pointMake(0, 0));
    }
}

void Application::update(unsigned long timestamp) {
    // We want to pass this timestamp through to the root view, such that we can propgate it to any view that
    // is doing an animation between states.
    
    if (getApplicationState() != kBackground) {
        getRootView()->update(timestamp);
    }
}

/*void Application::drawStatusBar(unsigned long timestamp) {
    if (this->_statusBar == nullptr) {
        this->_statusBar = new Label(rectMake(50, 0, getDisplaySize().width, 24));
        this->_statusBar->setColor(WHITE, BLACK);
        this->_statusBar->setFont(&FreeSans9pt7b, 12, 7);
        
        this->getRootView()->addSubview(_statusBar);
    }
    
    time_t rawtime = (time_t)timestamp / 1000;
    struct tm *dt = localtime(&rawtime);
    
    int hour = dt->tm_hour;
    int minute = dt->tm_min;
    
    char str[6];
    sprintf(str, "%0d:%0d", hour, minute);

    this->_statusBar->setText(str);
}*/
