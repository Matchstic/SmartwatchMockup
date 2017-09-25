//
//  Animation.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 24/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef Animation_hpp
#define Animation_hpp

#include <stdio.h>

#include <functional>

class View;

// To extend animations, add view property here.
struct AnimationState {
    Rect frame;
};

typedef enum : int {
    kInitialised,
    kRunning,
    kEnded
} AnimationProgressState;

// Creates an animation state from the provided view.
AnimationState *animationStateMake(View *view);

class Animation {
public:
    Animation(View *toAnimateOn, AnimationState *toState, float duration);
    ~Animation();
    
    // Called by system.
    // TODO: Make friend of View only.
    void update(unsigned long timestamp);
    void begin();
    void setCompletion(std::function<void()> completion) {
        _completion = completion;
    }
    
    AnimationProgressState getState() {
        return _progressState;
    }
    
private:
    unsigned long _startTimestamp;
    float _durationSeconds;
    
    View *_toAnimateOn;
    AnimationState *_fromState;
    AnimationState *_toState;
    AnimationProgressState _progressState = kInitialised;
    
    std::function<void()> _completion;
    
    float _percentageFromTimestamp(unsigned long timestamp);
    void _updateFrameWithProgress(float percent);
    // Add new _update... here.
};

#endif /* Animation_hpp */
