/*********************************************************************
 This is an Arduino library for our Monochrome SHARP Memory Displays
 
 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/products/1393
 
 These displays use SPI to communicate, 3 pins are required to
 interface
 
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!
 
 Written by Limor Fried/Ladyada  for Adafruit Industries.
 BSD license, check license.txt for more information
 All text above, and the splash screen must be included in any redistribution
 *********************************************************************/

#include "Adafruit_SharpMem.hpp"
#import "SWMLCDView.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif
#ifndef _swap_uint16_t
#define _swap_uint16_t(a, b) { uint16_t t = a; a = b; b = t; }
#endif

#define PROGMEM
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

/**************************************************************************
 Sharp Memory Display Connector
 -----------------------------------------------------------------------
 Pin   Function        Notes
 ===   ==============  ===============================
 1   VIN             3.3-5.0V (into LDO supply)
 2   3V3             3.3V out
 3   GND
 4   SCLK            Serial Clock
 5   MOSI            Serial Data Input
 6   CS              Serial Chip Select
 9   EXTMODE         COM Inversion Select (Low = SW clock/serial)
 7   EXTCOMIN        External COM Inversion Signal
 8   DISP            Display On(High)/Off(Low)
 
 **************************************************************************/

#define SHARPMEM_BIT_WRITECMD   (0x80)
#define SHARPMEM_BIT_VCOM       (0x40)
#define SHARPMEM_BIT_CLEAR      (0x20)
#define TOGGLE_VCOM             do { _sharpmem_vcom = _sharpmem_vcom ? 0x00 : SHARPMEM_BIT_VCOM; } while(0);

byte *sharpmem_buffer;

/* ************* */
/* CONSTRUCTORS  */
/* ************* */
Adafruit_SharpMem::Adafruit_SharpMem(uint8_t clk, uint8_t mosi, uint8_t ss, uint16_t width, uint16_t height) :
Adafruit_GFX(width, height) {
    _clk  = clk;
    _mosi = mosi;
    _ss   = ss;
}

boolean Adafruit_SharpMem::begin(void) {
    setRotation(0);
    
    return true;
}


/* ************** */
/* PUBLIC METHODS */
/* ************** */

/**************************************************************************/
/*!
 @brief Draws a single pixel in image buffer
 
 @param[in]  x
 The x position (0 based)
 @param[in]  y
 The y position (0 based)
 */
/**************************************************************************/
void Adafruit_SharpMem::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;
    
    switch(rotation) {
        case 1:
            _swap_int16_t(x, y);
            x = WIDTH  - 1 - x;
            break;
        case 2:
            x = WIDTH  - 1 - x;
            y = HEIGHT - 1 - y;
            break;
        case 3:
            _swap_int16_t(x, y);
            y = HEIGHT - 1 - y;
            break;
    }
    
    [[SWMLCDView sharedInstance] drawPixelAtX:x y:y color:color];
}

/**************************************************************************/
/*!
 @brief Gets the value (1 or 0) of the specified pixel from the buffer
 
 @param[in]  x
 The x position (0 based)
 @param[in]  y
 The y position (0 based)
 
 @return     1 if the pixel is enabled, 0 if disabled
 */
/**************************************************************************/
uint8_t Adafruit_SharpMem::getPixel(uint16_t x, uint16_t y)
{
    if((x >= _width) || (y >= _height)) return 0; // <0 test not needed, unsigned
    
    switch(rotation) {
        case 1:
            _swap_uint16_t(x, y);
            x = WIDTH  - 1 - x;
            break;
        case 2:
            x = WIDTH  - 1 - x;
            y = HEIGHT - 1 - y;
            break;
        case 3:
            _swap_uint16_t(x, y);
            y = HEIGHT - 1 - y;
            break;
    }
    
    return [[SWMLCDView sharedInstance] getPixelAtX:x y:y];
}

/**************************************************************************/
/*!
 @brief Clears the screen
 */
/**************************************************************************/
void Adafruit_SharpMem::clearDisplay()
{
    [[SWMLCDView sharedInstance] clear];
}

/**************************************************************************/
/*!
 @brief Renders the contents of the pixel buffer on the LCD
 */
/**************************************************************************/
void Adafruit_SharpMem::refresh(void)
{
    [[SWMLCDView sharedInstance] refresh];
}
