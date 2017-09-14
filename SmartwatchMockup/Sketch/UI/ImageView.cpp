//
//  ImageView.cpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 12/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include "ImageView.hpp"

ImageView::ImageView(Rect frame, const uint8_t *bitmap, Size imageSize, uint16_t color) :
    View(frame),
    _bitmap(bitmap),
    _imageSize(imageSize),
    _color(color) {

    this->setBackgroundColor(TRANSPARENT);
}

ImageView::~ImageView() {
    
}

void ImageView::drawRect(Rect rect) {
    View::_display->drawBitmap(rect.origin.x, rect.origin.y, this->_bitmap, this->_imageSize.width, this->_imageSize.height, this->_color);
}

void ImageView::setImage(const uint8_t *bitmap, Size imageSize, uint16_t color) {
    this->_bitmap = bitmap;
    this->_imageSize = imageSize;
    this->_color = color;
}
