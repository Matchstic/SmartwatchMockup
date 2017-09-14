//
//  ImageView.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 12/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef ImageView_hpp
#define ImageView_hpp

#include <stdio.h>
#include "View.hpp"

class ImageView : public View {
public:
    ImageView(Rect frame, const uint8_t *bitmap, Size imageSize, uint16_t color);
    ~ImageView();
    
    void drawRect(Rect rect);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Setting images etc.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    void setImage(const uint8_t *bitmap, Size imageSize, uint16_t color);
    
private:
    const uint8_t *_bitmap;
    Size _imageSize;
    uint16_t _color;
};

#endif /* ImageView_hpp */
