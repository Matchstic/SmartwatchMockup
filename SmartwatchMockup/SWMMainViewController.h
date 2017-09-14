//
//  SWMMainViewController.h
//  SmartwatchMockup
//
//  Created by Matt Clarke on 11/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SWMLCDView.h"

@interface SWMMainViewController : NSViewController {
    
}

@property (nonatomic, strong) NSImageView *chromeImageView;
@property (nonatomic, strong) SWMLCDView *lcdView;
@property (nonatomic, strong) NSTimer *looper;

@end
