//
//  SWMLCDView.m
//  SmartwatchMockup
//
//  Created by Matt Clarke on 11/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#import "SWMLCDView.h"
#include "Framebuffer.hpp"

@implementation SWMLCDView

+ (instancetype)sharedInstance {
    static dispatch_once_t queueCreationGuard;
    static SWMLCDView *sharedInstance;
    dispatch_once(&queueCreationGuard, ^{
        sharedInstance = [[SWMLCDView alloc] init];
    });
    return sharedInstance;
}

-(void)clear {
    [self _initialiseImageBuffer];
}

-(void)_initialiseImageBuffer {
    if (!self.imageRep) {
        self.imageRep = [[NSBitmapImageRep alloc]
                     initWithBitmapDataPlanes:NULL
                     pixelsWide:LCD_WIDTH
                     pixelsHigh:LCD_HEIGHT
                     bitsPerSample:1
                     samplesPerPixel:1
                     hasAlpha:NO
                     isPlanar:NO
                     colorSpaceName:@"NSDeviceWhiteColorSpace"
                     bytesPerRow:0
                     bitsPerPixel:0];
    }
    
    // Intialise to black.
    for (int y = 0; y < LCD_HEIGHT; y++) {
        for (int x = 0; x < LCD_WIDTH; x++) {
            NSUInteger zColourAry[1] = {1};
            [self.imageRep setPixel:zColourAry atX:x y:y];
        }
    }
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Drawing code here.
    [self.imageRep drawInRect:dirtyRect];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Adafruit GFX entry point
///////////////////////////////////////////////////////////////////////////////////////////////////////

-(void)drawPixelAtX:(int16_t)x y:(int16_t)y color:(uint16_t) color {
    NSUInteger zColourAry[1] = {color};
    [self.imageRep setPixel:zColourAry atX:x y:y];
}

- (uint8_t)getPixelAtX:(int16_t)x y:(int16_t)y {
    NSUInteger pixel;
    [self.imageRep getPixel:&pixel atX:x y:y];
    
    return pixel;
}

- (void)refresh {
    // Request re-draw
    [self display];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// ARM mbed Sharp Memory LCD entry point
///////////////////////////////////////////////////////////////////////////////////////////////////////

- (uint8_t)bitReverse8:(uint8_t)byte {
#if (__CORTEX_M >= 0x03)
    return (uint8_t)(__RBIT(byte) >> 24);
#else /* #if (__CORTEX_M < 0x03) */
    uint8_t rev8 = 0;
    
    for (unsigned i = 0; i < 8; i++) {
        if (byte & (1 << i)) {
            rev8 |= 1 << (7 - i);
        }
    }
    
    return rev8;
#endif /* #if (__CORTEX_M >= 0x03) */
}

// basically, we're emulating the display!
-(void)drawFrameBuffer:(const void *)fb {
    if (!self.imageRep) {
        [self _initialiseImageBuffer];
    }
    
    // Iterate over the framebuffer pointer, and draw into the image bitmap ref.
    
    const uint8_t* buffer = ((FrameBuffer*)fb)->getBuffer();
    
    int xLine = 0;
    int x = 0;
    int y = 0;
    int lineLength = LCD_FRAMEBUFFER_SIZEOF_SCAN_LINE;
    
    for (int i = 0; i < SIZEOF_FRAMEBUFFER_FOR_ALLOC; i++) {
        if (x < 2) {
            x++;
           continue;
        }
        
        uint8_t byte = [self bitReverse8:buffer[i]];
        
        // Each byte contains 8 pixel values.
        for (int j = 0; j < 8; j++) {
            uint8_t bit = ((byte >> j) & 0x01);
            
            NSUInteger pix[1] = {bit};
            [self.imageRep setPixel:pix atX:xLine y:y];
            
            xLine++;
        }
        
        // Increments
        if ((i+1) % lineLength == 0) {
            y++;
            x = 0;
            xLine = 0;
        } else {
            x++;
        }
    }
    
    // Request re-draw
    [self display];
}

@end
