//
//  SuperApplication.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 14/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "SuperApplication.hpp"
#include "ClockApplication.hpp"
#include "MenuApplication.hpp"
#include "TimerApplication.hpp"

#define primaryIdentifier "com.matchstic.clock"
#define menuIdentifier "com.matchstic.menu"

using namespace std;

SuperApplication::SuperApplication(Size displaySize) :
    _displaySize(displaySize),
    _displayState(kPrimary) {
    this->_rootView = new View(rectMake(0, 0, displaySize.width, displaySize.height));
    this->_rootView->setBackgroundColor(BLACK);
    
    this->_buildApplicationList();
        
    // We now need to add the primary to our UI.
    this->_transitionToPrimaryFromBoot();
}

SuperApplication::~SuperApplication() {
    delete _rootView;
    
    // Delete the map of applications?
    // Null the currentApplication pointer
    
}

void SuperApplication::_buildApplicationList() {
    // TODO: Build a map of all applications, ideally alphabetically on applicationName().
    
    // Primary.
    ClockApplication *primary = new ClockApplication(_displaySize, this);
    primary->setApplicationState(kBackground);
    this->_allApplications.insert({primary->applicationIdentifier(), primary});
    
    // Timer
    TimerApplication *timer = new TimerApplication(_displaySize);
    timer->setApplicationState(kBackground);
    this->_allApplications.insert({timer->applicationIdentifier(), timer});
    
    // INSERT MORE APPLICATIONS HERE pls.
    
    // Menu - is last to capture all applications.
    MenuApplication *menu = new MenuApplication(_displaySize, this);
    menu->setApplicationState(kBackground);
    this->_allApplications.insert({menu->applicationIdentifier(), menu});
}

std::list<Application*> *SuperApplication::getAllMenuApplications() {
    list<Application *> *menuApplications = new list<Application*>();
    
    // Iterate over ALL applications. We only add ones that can be shown in the menu.
    for (map<string, Application*>::const_iterator iterator = this->_allApplications.begin(), end = this->_allApplications.end(); iterator != end; ++iterator) {
        Application *application = iterator->second;
        
        if (application->showInMenu()) {
            printf("Adding... %s\n", application->applicationName().c_str());
            menuApplications->push_back(application);
        } else {
            printf("Not adding... %s\n", application->applicationName().c_str());
        }
    }
    
    menuApplications->sort([](Application *a, Application *b) {
        return a->applicationName() < b->applicationName();
    });
    
    return menuApplications;
}

Application *SuperApplication::_getApplicationWithIdentifier(std::string identifier) {
    std::map<std::string, Application*>::iterator it = _allApplications.find(identifier);
    
    if (it != _allApplications.end()) {
        return it->second;
    } else {
        return NULL;
    }
}

void SuperApplication::draw() {
    // Quite simply, just draw the root view! That in turn will draw any applications being (co-)rendered.
    this->getRootView()->draw(pointMake(0, 0));
}

// Timestamp passed through to ALL applications
void SuperApplication::update(unsigned long timestamp) {
    for (std::map<std::string, Application*>::const_iterator iterator = this->_allApplications.begin(), end = this->_allApplications.end(); iterator != end; ++iterator) {
        Application *application = iterator->second;
        application->update(timestamp);
    }
}

void SuperApplication::onButtonPress(int button) {
    printf("Button: %d was pressed!\n", button);
    
    if (this->_currentApplication != NULL) {
        this->_currentApplication->onButtonPress(button);
    }
}

void SuperApplication::onButtonLongPress(int button) {
    printf("Button: %d was long-pressed!\n", button);
    
    if (button == kAction) {
        // Back to home we go!
        this->_transitionToPrimaryFromApplication();
    } else if (this->_currentApplication != NULL) {
        this->_currentApplication->onButtonLongPress(button);
    }
}

void SuperApplication::launchApplication(std::string applicationIdentifier) {
    switch (_displayState) {
        case kPrimary:
        case kMenu:
            this->_transitionToApplication(applicationIdentifier);
            break;
        case kApplication:
        default:
            printf("ApplicationLaunchError :: Cannot launch another application when one is already active!");
            break;
    }
}

void SuperApplication::closeCurrentApplication() {
    this->_transitionToPrimaryFromApplication();
}

void SuperApplication::_transitionToPrimaryFromBoot() {
    this->_currentApplication = this->_getApplicationWithIdentifier(primaryIdentifier);
    this->_currentApplication->setApplicationState(kForeground);
    
    // Setup incoming frame for animations.
    this->_currentApplication->getRootView()->setFrame(rectMake(0, _displaySize.height, _displaySize.width, _displaySize.height));
    
    this->_rootView->addSubview(this->_currentApplication->getRootView());
    
    // Animate the primary into view
    AnimationState *newApplicationState = animationStateMake(this->_currentApplication->getRootView());
    newApplicationState->frame.origin.y = 0;
    
    this->_currentApplication->getRootView()->animateWithState(newApplicationState, 0.15, [this]() {
        // nop.
    });
}

void SuperApplication::_transitionToPrimaryFromApplication() {
    
    if (this->_displayState == kPrimary) {
        // No need to return again!
        return;
    }
    
    Application *previousApplication = this->_currentApplication;
    previousApplication->setApplicationState(kClosing);
    
    this->_currentApplication = this->_getApplicationWithIdentifier(primaryIdentifier);
    this->_currentApplication->setApplicationState(kOpening);
    this->_displayState = kPrimary;
    
    // Setup incoming frame for animations.
    this->_currentApplication->getRootView()->setFrame(rectMake(-_displaySize.width, 0, _displaySize.width, _displaySize.height));
    
    this->_rootView->addSubview(this->_currentApplication->getRootView());
    
    // Animations.
    
    AnimationState *previousApplicationState = animationStateMake(previousApplication->getRootView());
    previousApplicationState->frame.origin.x = _displaySize.width;
    
    AnimationState *newApplicationState = animationStateMake(this->_currentApplication->getRootView());
    newApplicationState->frame.origin.x = 0;
    
    previousApplication->getRootView()->animateWithState(previousApplicationState, 0.15, [previousApplication](){
        previousApplication->getRootView()->removeFromSuperview();
        previousApplication->setApplicationState(kBackground);
    });
    
    this->_currentApplication->getRootView()->animateWithState(newApplicationState, 0.15, [this]() {
        this->_currentApplication->setApplicationState(kForeground);
    });
}

void SuperApplication::_transitionToApplication(std::string applicationIdentifier) {
    
    Application *previousApplication = this->_currentApplication;
    previousApplication->setApplicationState(kClosing);
    
    this->_currentApplication = this->_getApplicationWithIdentifier(applicationIdentifier);
    this->_currentApplication->setApplicationState(kOpening);
    this->_displayState = !strcmp(applicationIdentifier.c_str(), menuIdentifier) ? kMenu : kApplication;
    
    // Setup incoming frame for animations.
    this->_currentApplication->getRootView()->setFrame(rectMake(_displaySize.width, 0, _displaySize.width, _displaySize.height));
    
    this->_rootView->addSubview(this->_currentApplication->getRootView());
    
    // Animations
    
    AnimationState *previousApplicationState = animationStateMake(previousApplication->getRootView());
    previousApplicationState->frame.origin.x = -_displaySize.width;
    
    AnimationState *newApplicationState = animationStateMake(this->_currentApplication->getRootView());
    newApplicationState->frame.origin.x = 0;
    
    previousApplication->getRootView()->animateWithState(previousApplicationState, 0.15, [previousApplication](){
        previousApplication->getRootView()->removeFromSuperview();
        previousApplication->setApplicationState(kBackground);
    });
    
    this->_currentApplication->getRootView()->animateWithState(newApplicationState, 0.15, [this]() {
        this->_currentApplication->setApplicationState(kForeground);
    });
}

