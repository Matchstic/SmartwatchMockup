//
//  SuperApplication.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 14/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "SuperApplication.hpp"
#include "ClockApplication.hpp"

#define primaryIdentifier "com.matchstic.clock"
#define menuIdentifier "com.matchstic.menu"

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
    ClockApplication *primary = new ClockApplication(_displaySize);
    this->_allApplications.insert({primary->applicationIdentifier(), primary});
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
    if (this->_currentApplication != NULL) {
        this->_currentApplication->onButtonPress(button);
    }
}

void SuperApplication::onButtonLongPress(int button) {
    if (this->_currentApplication != NULL) {
        this->_currentApplication->onButtonLongPress(button);
    }
}

void SuperApplication::launchApplication(std::string applicationIdentifier) {
    switch (_displayState) {
        case kPrimary:
            this->_transitionToApplicationFromPrimary(applicationIdentifier);
            break;
        case kMenu:
            this->_transitionToApplicationFromMenu(applicationIdentifier);
            break;
        case kApplication:
        default:
            printf("ApplicationLaunchError :: Cannot launch another application when one is already active!");
            break;
    }
}

void SuperApplication::closeCurrentApplication() {
    
}

void SuperApplication::_transitionToPrimaryFromBoot() {
    this->_currentApplication = this->_getApplicationWithIdentifier(primaryIdentifier);
    this->_currentApplication->setApplicationState(kForeground);
    
    this->_rootView->addSubview(this->_currentApplication->getRootView());
    
    // TODO: Animate the primary into view?
}

void SuperApplication::_transitionToMenuFromPrimary() {
    // TODO
}

void SuperApplication::_transitionToPrimaryFromMenu() {
    // TODO
}

void SuperApplication::_transitionToPrimaryFromApplication(std::string applicationIdentifier) {
    // TODO
}

void SuperApplication::_transitionToApplicationFromMenu(std::string applicationIdentifier) {
    // TODO
}

void SuperApplication::_transitionToApplicationFromPrimary(std::string applicationIdentifier) {
    // TODO
}

