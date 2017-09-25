//
//  SuperApplication.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 14/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef SuperApplication_hpp
#define SuperApplication_hpp

#include <stdio.h>
#include "Application.hpp"
#include <map>

typedef enum : int {
    kPrimary,
    kMenu,
    kApplication,
} DisplayState;

class SuperApplication {
public:
    SuperApplication(Size displaySize);
    ~SuperApplication();
    
    void draw();
    void update(unsigned long timestamp); // Timestamp passed through to applications
    
    void onButtonPress(int button);
    void onButtonLongPress(int button);
    
    void launchApplication(std::string applicationIdentifier);
    void closeCurrentApplication();
    
    View* getRootView() {
        return _rootView;
    }
    
    bool hasWakelockSet() {
        return _currentApplication != NULL ? _currentApplication->hasWakelockSet() : false;
    }
    
    std::list<Application*> *getAllMenuApplications();
    
private:
    Application *_currentApplication;
    std::map<std::string, Application*> _allApplications;
    
    void _buildApplicationList();
    Application *_getApplicationWithIdentifier(std::string identifier);
    
    DisplayState _displayState;
    
    View *_rootView;
    Size _displaySize;
    
    void _transitionToPrimaryFromBoot();
    void _transitionToPrimaryFromApplication();
    
    void _transitionToApplication(std::string applicationIdentifier);
};

#endif /* SuperApplication_hpp */
