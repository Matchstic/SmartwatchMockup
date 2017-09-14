//
//  SWMMainViewController.m
//  SmartwatchMockup
//
//  Created by Matt Clarke on 11/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#import "SWMMainViewController.h"
#include "Framebuffer.hpp"
#include "Smartwatch.hpp"

@interface SWMMainViewController ()

@end

Smartwatch smartwatch;

@implementation SWMMainViewController

dispatch_queue_t backgroundQueue() {
    static dispatch_once_t queueCreationGuard;
    static dispatch_queue_t queue;
    dispatch_once(&queueCreationGuard, ^{
        queue = dispatch_queue_create("com.matchstic.smartwatchmockup.backgroundQueue", 0);
    });
    return queue;
}

- (void)viewDidAppear {
    [super viewDidAppear];
    
    // Start running main off the UI thread.
    dispatch_async(backgroundQueue(), ^(void){
        [self main];
    });
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.layer.backgroundColor = [NSColor blackColor].CGColor;
    
    // Create LCD chrome.
    self.chromeImageView = [[NSImageView alloc] initWithFrame:self.view.frame];
    self.chromeImageView.image = [NSImage imageNamed:@"breakout.png"];
    
    [self.view addSubview:self.chromeImageView];
    
    // LCD view
    self.lcdView = [SWMLCDView sharedInstance];
    self.lcdView.frame = NSMakeRect(70, 57, LCD_WIDTH, LCD_HEIGHT);
    
    [self.view addSubview:self.lcdView];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Begin lifecycle "emulation"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

-(void)main {
    [self setup];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        self.looper = [NSTimer scheduledTimerWithTimeInterval:0.005 target:self selector:@selector(loop:) userInfo:nil repeats:YES];
    });
}

-(void)setup {
    smartwatch.setup();
}

-(void)loop:(id)sender {
    smartwatch.loop();
}

@end
