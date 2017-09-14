//
//  Application.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 12/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "Application.hpp"

Application::Application(/*NotificationManager*/ Size displaySize) {
    _displaySize = displaySize;
    _rootView = new View(rectMake(0, 0, displaySize.width, displaySize.height));
    
    viewDidLoad();
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
