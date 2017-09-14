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

class ClockApplication : public Application {
public:
    ClockApplication(Size displaySize);
    ~ClockApplication();
    
    void viewDidLoad();
    
    void update(unsigned long timestamp);
    void onButtonPress(int button);
    void onButtonLongPress(int button);
    
    boolean showInMenu() {
        return false;
    }
    
    std::string applicationName() {
        return "Clock";
    }
    
    std::string applicationIdentifier() {
        return "com.matchstic.clock";
    }
    
private:
    Label *hourLabel;
    Label *minuteLabel;
    Circle *secondsDot;
};

#endif /* ClockApplication_hpp */
