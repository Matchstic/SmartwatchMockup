//
//  SharpLCD.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 11/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "SharpLCD.hpp"
#import "SWMLCDView.h"

void SharpLCD::enableDisplay(void) {
    
}
void SharpLCD::disableDisplay(void) {
    
}

void SharpLCD::clear(void) {
    [[SWMLCDView sharedInstance] clear];
}

void SharpLCD::drawFrameBuffer(const FrameBuffer *fb) {
    [[SWMLCDView sharedInstance] drawFrameBuffer:fb];
}
