//
//  View.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 12/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "View.hpp"

Rect rectMake(int x, int y, int width, int height) {
    Rect frame;
    frame.origin.x = x;
    frame.origin.y = y;
    frame.size.width = width;
    frame.size.height = height;
    
    return frame;
}

Point pointMake(int x, int y) {
    Point pt;
    pt.x = x;
    pt.y = y;
    
    return pt;
}

Size sizeMake(int width, int height) {
    Size sz;
    sz.width = width;
    sz.height = height;
    
    return sz;
}

Adafruit_GFX* View::_display = 0;

View::View(Rect frame) : _frame(frame), _hidden(false), _backgroundColor(TRANSPARENT) {
    
}

View::~View() {
    
}

void View::addSubview(View* subview) {
    _subviews.push_back(subview);
    subview->_superview = this;
}

void View::insertSubviewAtIndex(View *subview, int index) {
    if (index > _subviews.size() - 1) {
        index = _subviews.size() - 1;
    } else if (index < 0) {
        throw "InternalInconsistencyException :: Cannot insert at a negative index!";
    }
    
    bool present = (std::find(_subviews.begin(), _subviews.end(), subview) != _subviews.end());
    if (present) {
        #warning Un-implemented logic flow
    } else {
        std::list<View*>::iterator itr = _subviews.begin();
        std::advance(itr, index);
        _subviews.insert(itr, subview);
    }
    
    subview->_superview = this;
}

void View::removeFromSuperview() {
    this->_superview->_removeSubview(this);
}

void View::_removeSubview(View *subview) {
    bool present = (std::find(_subviews.begin(), _subviews.end(), subview) != _subviews.end());
    if (present) {
        _subviews.remove(subview);
    }
}

void View::draw(Point parentOffset) {
    // If we are hidden, no need to draw ourselves or any subviews.
    if (this->getHidden()) {
        return;
    }
    
    // These offsets MUST be utilised to move things around based upon the parent's positioning system.
    int xOffset = parentOffset.x + this->getFrame().origin.x;
    int yOffset = parentOffset.y + this->getFrame().origin.y;
    
    // Handle background colouration.
    if (_backgroundColor != TRANSPARENT) {
        View::_display->fillRect(xOffset, yOffset, this->getFrame().size.width, this->getFrame().size.height, _backgroundColor);
    }
    
    // Allow for customising drawing on this view for subclasses.    
    this->drawRect(rectMake(xOffset, yOffset, this->getFrame().size.width, this->getFrame().size.height));
    
    // Handle subviews.
    this->_drawSubviews(pointMake(xOffset, yOffset));
}

void View::_drawSubviews(Point currentOffset) {
    for (std::list<View*>::const_iterator iterator = this->_subviews.begin(), end = this->_subviews.end(); iterator != end; ++iterator) {
        View *view = *iterator;
        view->draw(currentOffset);
    }
}

void View::update(unsigned long timestamp) {
    
    // Iterate over the animations list and update them.
    if (this->_animations.size() > 0) {
        for (std::list<Animation*>::const_iterator iterator = this->_animations.begin(), end = this->_animations.end(); iterator != end; ++iterator) {
            Animation *animation = *iterator;
            if (animation != NULL) {
                animation->update(timestamp);
                
                if (animation->getState() == kEnded) {
                    iterator = this->_animations.erase(iterator);
                    delete animation;
                }
            }
        }
    }
    
    // Update all the subviews too!
    for (std::list<View*>::const_iterator iterator = this->_subviews.begin(), end = this->_subviews.end(); iterator != end; ++iterator) {
        View *view = *iterator;
        view->update(timestamp);
    }
}
