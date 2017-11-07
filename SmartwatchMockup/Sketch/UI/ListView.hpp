//
//  ListView.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 24/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef ListView_hpp
#define ListView_hpp

#include <stdio.h>
#include "View.hpp"
#include <vector>

typedef enum : int {
    kVertical,
    kHorizontal
} ListViewDirection;

class ListView : public View {
public:
    ListView(Rect frame);
    ~ListView();
    
    void drawRect(Rect rect);
    
    // Direction to scroll in.
    void setDirection(ListViewDirection dir) {
        _direction = dir;
    }
    
    // Set the size each view is to be.
    void setItemSize(int size) {
        _itemSize = size;
    }
    
    void setHighlightSelection(bool highlightSelection) {
        
    }
    
    // Item management.
    void addNewItem(View *item);
    void insertNewItemAtIndex(View *item, int index);
    void removeItemAtIndex(int index);
    
    // Current state information.
    int getCurrentIndex();
    
    // Movement.
    void moveToIndex(int index);
    void moveForwardsByIndexCount(int indexCount);
    void moveBackwardsByIndexCount(int indexCount);
    
private:
    int _itemSize;
    ListViewDirection _direction;
    bool _highlightSelection;
    
    // Scrollbar.
    View *_scrollBarOuterView;
    View *_scrollBarInnerView;
    
    std::vector<View*> _items;
    int _currentIndex;
};

#endif /* ListView_hpp */
