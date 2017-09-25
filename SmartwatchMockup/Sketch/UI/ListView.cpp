//
//  ListView.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 24/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "ListView.hpp"

#define SCROLL_DURATION 0.15

ListView::ListView(Rect frame) : View(frame),
    _currentIndex(0) {
    this->setBackgroundColor(TRANSPARENT);
}

ListView::~ListView() {
    
}

void ListView::addNewItem(View *item) {
    this->_items.push_back(item);
    this->addSubview(item);
}

void ListView::insertNewItemAtIndex(View *item, int index) {
    if (index > _items.size() - 1) {
        index = (int)_items.size() - 1;
    } else if (index < 0) {
        throw "InternalInconsistencyException :: Cannot insert at a negative index!";
    }
    
    std::vector<View*>::iterator itr = _items.begin();
    std::advance(itr, index);
    _items.insert(itr, item);
    
    this->addSubview(item);
}

void ListView::removeItemAtIndex(int index) {
    // Check item exists at this index!
    if (index < 0 || index > _items.size() - 1) {
        return; // Not in bounds!
    }
    
    View *toRemove = _items[index];
    
    _items.erase(_items.begin() + index);
    
    toRemove->removeFromSuperview();
    
    // Move items to the new places.
    // TODO: Animations are jerky as hell when doing this!
    this->moveToIndex(_currentIndex);
}

int ListView::getCurrentIndex() {
    return _currentIndex;
}

void ListView::moveToIndex(int index) {
    // Note: the duration of the animation depends on the _itemSize multiples travelled.
    // If 1, SCROLL_DURATION. If 2, SCROLL_DURATION*2. And so on.
    
    if (index < 0 || index > _items.size() - 1) {
        return; // No need to move around, can maybe do something to show at ends?
    }
    
    int goingForwards = index > _currentIndex;
    int itemsOnDisplay = ceilf(_direction == kVertical ? getFrame().size.height / _itemSize : getFrame().size.width / _itemSize);
    
    /*
     * Quite frankly, item sizing is a royal pain.
     * I'm going to go on record and say it will just be easier if we just centralise the current item
     * onscreen, and be done with it.
     * Therefore, we can assume that any given time, there will be (x + !(x%2)) items on-screen.
     */
    
    itemsOnDisplay += !(itemsOnDisplay%2);
    
    /*
     * We can now procede to calculate which views are now going to be on-screen during this upcoming move.
     * There will always be (itemsOnDisplay + 1) views to animate. This also happens to be an even number...
     * As a result, we can state that of (itemsOnDisplay + 1) views, half will be to, and half-1 will be from.
     */
    
    int animateFrom = 0, animateTo = 0;
    animateFrom = goingForwards ? ((itemsOnDisplay + 1) / 2) - 1 : (itemsOnDisplay + 1) / 2;
    animateTo = goingForwards ? (itemsOnDisplay + 1) / 2 : ((itemsOnDisplay + 1) / 2) - 1;
    
    animateFrom = _currentIndex - animateFrom;
    animateTo = _currentIndex + animateTo;
    
    int centermostPointValue = (_direction == kVertical) ? (getFrame().size.height/2) - (_itemSize/2) : (getFrame().size.width/2) - (_itemSize/2);
    
    int counter = 0;
    for (std::vector<View*>::const_iterator iterator = this->_items.begin(), end = this->_items.end(); iterator != end; ++iterator, ++counter) {
        
        View *view = *iterator;
        
        // Move this view's position based on it's relative closeness to the requested index.
        int currentViewPointValue = centermostPointValue - ((index - counter) * _itemSize);
        
        if (counter >= animateFrom && counter <= animateTo) {
            AnimationState *newViewState = animationStateMake(view);
            
            if (_direction == kVertical)
                newViewState->frame.origin.y = currentViewPointValue;
            else
                newViewState->frame.origin.x = currentViewPointValue;
            
            view->animateWithState(newViewState, SCROLL_DURATION * abs(_currentIndex - index), [](){});
        } else {
            Rect newFrame = view->getFrame();
            
            if (_direction == kVertical)
                newFrame.origin.y = currentViewPointValue;
            else
                newFrame.origin.x = currentViewPointValue;
            
            view->setFrame(newFrame);
        }
    }
    
    // And now, we update the index.
    _currentIndex = index;
}

void ListView::moveForwardsByIndexCount(int indexCount) {
    int newIndex = _currentIndex + indexCount;
    
    this->moveToIndex(newIndex);
}

void ListView::moveBackwardsByIndexCount(int indexCount) {
    int newIndex = _currentIndex - indexCount;
    
    this->moveToIndex(newIndex);
}
