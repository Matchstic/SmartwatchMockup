//
//  SharpLCD.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 11/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef SharpLCD_hpp
#define SharpLCD_hpp

#include <stdio.h>
#include "Framebuffer.hpp"

class SharpLCD {
public:
    SharpLCD() {}
    
    void enableDisplay(void);
    void disableDisplay(void);
    
    void clear(void);
    
    void drawFrameBuffer(const FrameBuffer *fb);
};

#endif /* SharpLCD_hpp */
