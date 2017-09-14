//
//  Circle.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 14/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "Circle.hpp"

Circle::Circle(Rect frame) :
    View(frame) {
    this->setBackgroundColor(TRANSPARENT);
}

Circle::~Circle() {
    
}

void Circle::drawRect(Rect rect) {
    View::_display->fillCircle(
                               rect.origin.x + rect.size.width/2,
                               rect.origin.y + rect.size.height/2,
                               rect.size.height/2,
                               _color);
}
