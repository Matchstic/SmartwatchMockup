//
//  Label.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 12/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef Label_hpp
#define Label_hpp

#include <stdio.h>
#include <string>
#include "View.hpp"

class Label : public View {
public:
    Label(Rect frame);
    ~Label();
    
    void drawRect(Rect rect);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Setting text etc.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    void setText(std::string text) {
        _text = text;
    }
    
    std::string getText() {
        return _text;
    }
    
    void setFont(const GFXfont* font, int fontsize, int baseline) {
        _font = font;
        _fontsize = fontsize;
        _baseline = baseline;
    }
    
    void setColor(uint16_t foregroundColor, uint16_t backgroundColor) {
        _foregroundColor = foregroundColor;
        _backgroundColor = backgroundColor;
    }
    
private:
    const GFXfont* _font;
    int _fontsize;
    int _baseline;
    uint16_t _foregroundColor;
    uint16_t _backgroundColor;
    std::string _text;
};

#endif /* Label_hpp */
