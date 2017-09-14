//
//  Circle.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 14/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef Circle_hpp
#define Circle_hpp

#include <stdio.h>
#include "View.hpp"

class Circle : public View {
public:
    Circle(Rect frame);
    ~Circle();
    
    void drawRect(Rect rect);
    
    void setColor(uint16_t color) {
        _color = color;
    }
    
private:
    uint16_t _color;
};

#endif /* Circle_hpp */
