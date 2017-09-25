//
//  MenuApplication.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 21/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef MenuApplication_hpp
#define MenuApplication_hpp

#include <stdio.h>
#include "Application.hpp"
#include "SuperApplication.hpp"
#include <map>
#include <list>

#include "ListView.hpp"

class MenuApplication : public Application {
public:
    MenuApplication(Size displaySize, SuperApplication *superApplication);
    ~MenuApplication();
    
    void viewDidLoad();
    
    void update(unsigned long timestamp);
    void onButtonPress(int button);
    void onButtonLongPress(int button);
    
    boolean showInMenu() {
        return false;
    }
    
    const std::string applicationName() {
        return "Menu";
    }
    
    const std::string applicationIdentifier() {
        return "com.matchstic.menu";
    }
    
private:
    SuperApplication *_superApplication;
    
    std::list<Application*> *_applications;
    ListView *listView;
};

#endif /* MenuApplication_hpp */
