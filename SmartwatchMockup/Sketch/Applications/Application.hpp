//
//  Application.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 12/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include "Structures.h"
#include "View.hpp"
#include "Label.hpp"
#include <list>
#include <string>

// Allows for defining the current state of execution for the application
typedef enum : int {
    kForeground,
    kBackground,
    kClosing,
    kOpening
} ExecutionState;

typedef enum : int {
    kTop,
    kAction,
    kBottom
} HardwareButton;

/**
 * An Application hosts all logic for a given application; it is treated as a view controller under MVC.
 */
class Application {
public:
    Application(/*NotificationManager*/ Size displaySize);
    ~Application();
    
    void draw();
    
    /**
     * This is called on every event loop iteration, to allow for doing any updates on a timely basis.
     * @param timestamp The current timestamp in millis
     */
    virtual void update(unsigned long timestamp); // Timestamp passed through to view heirarchy for animations
    
    /**
     * These two are called on hardware button events, and should be overriden.
     */
    virtual void onButtonPress(int button) {}
    virtual void onButtonLongPress(int button) {}
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Root view stuff
    //////////////////////////////////////////////////////////////////////////////////////////
    
    // To allow for moving the entire application's view etc.
    View* getRootView() {
        return _rootView;
    }
    
    void addSubview(View* subview) {
        _rootView->addSubview(subview);
    }
    
    // Override to customise this application.
    virtual void viewDidLoad() {}
    
    Size getDisplaySize() {
        return _displaySize;
    }
    
    /* NotificationManager getNotificationManager(); */
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Application state management
    //////////////////////////////////////////////////////////////////////////////////////////
    
    void setApplicationState(ExecutionState state) {
        _execState = state;
    }
    
    ExecutionState getApplicationState() {
        return _execState;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Wakelocks to avoid the system sleeping at important times.
    //////////////////////////////////////////////////////////////////////////////////////////
    
    bool hasWakelockSet() {
        return _wakelockQueue.size() > 0;
    }
    
    void addWakelockRequest(std::string identifier) {
        bool found = (std::find(_wakelockQueue.begin(), _wakelockQueue.end(), identifier) != _wakelockQueue.end());
        
        if (!found) {
            _wakelockQueue.push_back(identifier);
        }
    }
    
    void removeWakelockRequest(std::string identifier) {
        bool present = (std::find(_wakelockQueue.begin(), _wakelockQueue.end(), identifier) != _wakelockQueue.end());
        if (present) {
            _wakelockQueue.remove(identifier);
        }
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Main menu data points
    //////////////////////////////////////////////////////////////////////////////////////////
    
    // Override to hide in the main menu if needed.
    virtual boolean showInMenu() {
        return true;
    }
    
    const virtual std::string applicationName() {
        return "Application";
    }
    
    const virtual std::string applicationIdentifier() {
        return "com.company.name";
    }
    
protected:
    
    //void drawStatusBar(unsigned long timestamp);
    
private:
    ExecutionState _execState;
    
    Size _displaySize;
    
    // NotificationManager notificationManager;
    View* _rootView;
    
    std::list<std::string> _wakelockQueue;
    
    //Label *_statusBar;
};

#endif /* Application_hpp */
