//
//  MenuApplication.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 21/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "MenuApplication.hpp"
#include "Label.hpp"
#include "FreeSansBold18pt7b.h"

MenuApplication::MenuApplication(Size displaySize, SuperApplication *superApplication) :
    Application(displaySize),
    _superApplication(superApplication) {
    viewDidLoad();
    
    // Generate list of applications to display.
    this->_applications = superApplication->getAllMenuApplications();
}

MenuApplication::~MenuApplication() {
    
}

void MenuApplication::viewDidLoad() {
    // Iterate over the application list, and make views.
    // Item height = displayheight/3.
    
    int itemHeight = getDisplaySize().height / 3;
    
    this->listView = new ListView(rectMake(0, 0, getDisplaySize().width, getDisplaySize().height));
    this->listView->setItemSize(itemHeight);
    this->listView->setDirection(kVertical);
    
    this->addSubview(this->listView);
    
    for (int i = 0; i < 10; i++) {
        View *baseView = new View(rectMake(0, 0, getDisplaySize().width, itemHeight));
        
        char buf[12];
        sprintf(buf, "%d", i);
        
        Label *applicationLabel = new Label(rectMake(10, 10, getDisplaySize().width - 20, itemHeight - 20));
        applicationLabel->setText(buf);
        applicationLabel->setColor(WHITE, BLACK);
        applicationLabel->setFont(&FreeSansBold18pt7b, 12, 7);
        
        baseView->addSubview(applicationLabel);
        
        listView->addNewItem(baseView);
    }
    
    // Make sure to run a first layout, and then we're rolling.
    this->listView->moveToIndex(0);
    
    /*for (std::list<Application*>::const_iterator iterator = this->_applications->begin(), end = this->_applications->end(); iterator != end; ++iterator) {
        Application *application = *iterator;
        
        View *baseView = new View(rectMake(0, 0, getDisplaySize().width, itemHeight));
        
        Label *applicationLabel = new Label(rectMake(10, 10, getDisplaySize().width - 20, itemHeight - 20));
        
    }*/
}

void MenuApplication::update(unsigned long timestamp) {
    // Call super for animations!
    Application::update(timestamp);
    
    
}

void MenuApplication::onButtonPress(int button) {
    // When the action button is clicked, we need to launch the currently selected application.
    // up == scroll up, and vice versa
    
    if (button == kTop) {
        listView->moveBackwardsByIndexCount(1);
    } else if (button == kBottom) {
        listView->moveForwardsByIndexCount(1);
    } else if (button == kAction) {
        // Launch the selected application!
        printf("Selected: %d\n", listView->getCurrentIndex());
    }
}

void MenuApplication::onButtonLongPress(int button) {

}
