//
//  Animation.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 24/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "View.hpp"
#include "Animation.hpp"

AnimationState *animationStateMake(View *view) {
    AnimationState *state = new AnimationState();
    
    // Set properties.
    state->frame = view->getFrame();
    
    return state;
}

Animation::Animation(View *toAnimateOn, AnimationState *toState, float duration) :
    _toAnimateOn(toAnimateOn),
    _toState(toState),
    _progressState(kInitialised),
    _durationSeconds(duration) {
    
    _fromState = animationStateMake(toAnimateOn);
}

Animation::~Animation() {
    delete _fromState;
    delete _toState;
}

void Animation::begin() {
    _progressState = kRunning;
    _startTimestamp = millis();
}

void Animation::update(unsigned long timestamp) {
    if (_progressState != kRunning) return;
    
    float percent = this->_percentageFromTimestamp(timestamp);
    if (percent > 1.0) {
        percent = 1.0;
    }
    
    // Update properties.
    this->_updateFrameWithProgress(percent);
    
    if (percent >= 1.0) {
        _progressState = kEnded;
        
        // Let the original caller know we're done!
        _completion();
    }
}

// Note: this is currently just linear. We want nice easing via a cubic Bézier curve.
float Animation::_percentageFromTimestamp(unsigned long timestamp) {
    // First, calculate the difference between the end timestamp and start timestamp.
    unsigned long difference = _durationSeconds * 1000.0f;
    
    // Minus the start from the new timestamp to normalise it.
    unsigned long normalised = timestamp - _startTimestamp;
    
    // Now, divide the normalised by the difference to get the percentage.
    return (float)normalised / (float)difference;
}

void Animation::_updateFrameWithProgress(float percent) {
    Rect newFrame = _fromState->frame;
    
    // Handle origin....
    // x
    int xdiff = _toState->frame.origin.x - _fromState->frame.origin.x;
    newFrame.origin.x = newFrame.origin.x + (percent * xdiff);
    
    // y
    int ydiff = _toState->frame.origin.y - _fromState->frame.origin.y;
    newFrame.origin.y = newFrame.origin.y + (percent * ydiff);
    
    // Handle size....
    // width
    int widthdiff = _toState->frame.size.width - _fromState->frame.size.width;
    newFrame.size.width = newFrame.size.width + (percent * widthdiff);
    
    // height
    int heightdiff = _toState->frame.size.height - _fromState->frame.size.height;
    newFrame.size.height = newFrame.size.height + (percent * heightdiff);
    
    // Set new frame to view.
    _toAnimateOn->setFrame(newFrame);
}
