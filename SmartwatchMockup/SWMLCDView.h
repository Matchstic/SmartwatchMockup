//
//  SWMLCDView.h
//  SmartwatchMockup
//
//  Created by Matt Clarke on 11/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface SWMLCDView : NSView

@property (nonatomic, strong) NSBitmapImageRep *imageRep;

+ (instancetype)sharedInstance;

- (void)clear;
- (void)drawFrameBuffer:(const void *)fb;
- (void)drawPixelAtX:(int16_t)x y:(int16_t)y color:(uint16_t) color;
- (uint8_t)getPixelAtX:(int16_t)x y:(int16_t)y;
- (void)refresh;

@end
