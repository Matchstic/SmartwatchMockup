//
//  Arduino.c
//  SmartwatchMockup
//
//  Created by Matt Clarke on 12/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#include <stdio.h>
#include "Arduino.h"

#include <time.h>

unsigned long millis(void) {
    struct timeval  tv;
    gettimeofday(&tv, NULL);
    
    double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
    
    return time_in_mill;
}
