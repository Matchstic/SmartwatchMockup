//
//  ClockApplication.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 14/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef ClockApplication_hpp
#define ClockApplication_hpp

#include <stdio.h>
#include "Application.hpp"
#include <string>
#include "Label.hpp"
#include "Circle.hpp"
#include "SuperApplication.hpp"

class ClockApplication : public Application {
public:
    ClockApplication(Size displaySize, SuperApplication *superApplication);
    ~ClockApplication();
    
    void viewDidLoad();
    
    void update(unsigned long timestamp);
    void onButtonPress(int button);
    void onButtonLongPress(int button);
    
    boolean showInMenu() {
        return false;
    }
    
    const std::string applicationName() {
        return "Clock";
    }
    
    const std::string applicationIdentifier() {
        return "com.matchstic.clock";
    }
    
private:
    Label *hourLabel;
    Label *minuteLabel;
    Circle *secondsDot;
    
    SuperApplication *_superApplication;
};

#endif /* ClockApplication_hpp */
