//
//  TimerApplication.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 21/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef TimerApplication_hpp
#define TimerApplication_hpp

#include <stdio.h>
#include "Application.hpp"

class TimerApplication : public Application {
public:
    TimerApplication(Size displaySize);
    ~TimerApplication();
    
    void viewDidLoad();
    
    void update(unsigned long timestamp);
    void onButtonPress(int button);
    void onButtonLongPress(int button);
    
    boolean showInMenu() {
        return true;
    }
    
    const std::string applicationName() {
        return "Timer";
    }
    
    const std::string applicationIdentifier() {
        return "com.matchstic.timer";
    }
};

#endif /* TimerApplication_hpp */
