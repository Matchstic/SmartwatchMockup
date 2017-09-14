//
//  View.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 12/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef View_hpp
#define View_hpp

#include <stdio.h>
#include <list>
#include "Structures.h"
#include "Adafruit_GFX.h"

#define BLACK 0
#define WHITE 1
#define TRANSPARENT -1

Rect rectMake(int x, int y, int width, int height);
Point pointMake(int x, int y);
Size sizeMake(int width, int height);

class View {
public:
    static void initialise(Adafruit_GFX* display) {
        View::_display = display;
    }
    
    View() {};
    View(Rect frame);
    ~View();
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Handle rendering
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    /**
     * Called to start drawing.
     */
    void draw(Point parentOffset);
    
    /**
     * Override to provide custom drawing.
     */
    virtual void drawRect(Rect rect) {};
    
    /**
     * This is called on every event loop iteration, to allow for doing any updates on a timely basis.
     * @param timestamp The current timestamp in millis
     */
    virtual void update(unsigned long timestamp);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Subview management
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    void addSubview(View* subview);
    
    void insertSubviewAtIndex(View *subview, int index);
    
    void removeFromSuperview();
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Frames
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    Rect getFrame() {
        return this->_frame;
    }
    
    void setFrame(Rect newFrame) {
        this->_frame = newFrame;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Background colouration
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    void setBackgroundColor(int color) {
        this->_backgroundColor = color;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Misc. state management
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    void setHidden(boolean hidden) {
        this->_hidden = hidden;
    }
    
    boolean getHidden() {
        return this->_hidden;
    }
    
protected:
    static Adafruit_GFX* _display;
    
private:
    Rect _frame;
    
    std::list<View*> _subviews;
    View* _superview;
    
    int _backgroundColor;
    boolean _hidden;
    
    void _drawSubviews(Point currentOffset);
    void _removeSubview(View* subview);
};

#endif /* View_hpp */
