//
//  Label.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 12/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "Label.hpp"

Label::Label(Rect frame) :
    View(frame) {
        this->setBackgroundColor(TRANSPARENT);
}

Label::~Label() {
    
}

void Label::drawRect(Rect rect) {
    View::_display->setCursor(rect.origin.x, rect.origin.y + _fontsize + _baseline);
    View::_display->setTextColor(this->_foregroundColor, this->_backgroundColor);
    View::_display->setFont(this->_font);
    View::_display->setTextWrap(false);
    View::_display->print(this->getText().c_str());
}
