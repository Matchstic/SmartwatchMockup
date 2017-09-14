//
//  Framebuffer.hpp
//  SmartwatchMockup
//
//  Created by Matt Clarke on 11/09/2017.
//  Copyright © 2017 Matt Clarke. All rights reserved.
//

#ifndef Framebuffer_hpp
#define Framebuffer_hpp

#include <stdio.h>
#include <cstdint>

#include "font.h"

/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

static const unsigned LCD_WIDTH  = 144;  ///< Constant defining the LCD's geometry.
static const unsigned LCD_HEIGHT = 168;  ///< Constant defining the LCD's geometry.
static const unsigned LCD_END_OF_DUMMY_SIZE = 2;
static const unsigned LCD_FRAMEBUFFER_SIZEOF_SCAN_LINE_METADATA =
(1 + /* mode byte in SPI update command */
 1   /* addr byte in SPI update command */);
static const unsigned LCD_FRAMEBUFFER_SIZEOF_SCAN_LINE =
(LCD_FRAMEBUFFER_SIZEOF_SCAN_LINE_METADATA + (LCD_WIDTH / 8));

static const unsigned SIZEOF_FRAMEBUFFER = (LCD_HEIGHT * LCD_FRAMEBUFFER_SIZEOF_SCAN_LINE);
static const unsigned SIZEOF_FRAMEBUFFER_FOR_ALLOC = SIZEOF_FRAMEBUFFER + LCD_END_OF_DUMMY_SIZE;

/* Constants for the LCD's command protocol */
static const uint8_t M0_FLAG = 0x80;
static const uint8_t M1_FLAG = 0x40;
static const uint8_t M2_FLAG = 0x20;
static const uint8_t DUMMY8  = 0x00;

class FrameBuffer {
public:
    /**
     * \brief initialize the hardware dependent component of a given
     * framebuffer; and set it up to show all-white.
     *
     * \note This does not update the LCD automatically; it only
     * initializes a framebuffer.
     *
     * \param[in] fb
     *     A memory buffer to initialize.
     */
    FrameBuffer(uint8_t *fb);
    
    /**
     * Clear the framebuffer to prepare for new drawing. The cleared buffer
     * still needs to be drawn on the LCD if clearning the LCD is desired.
     */
    void clear(void);
    
    /**
     * \brief Copy over a bitmap to a specified location into the framebuffer.
     *
     * This is the main work-horse for displaying bitmaps on the LCD. We
     * only support mono-chrome bitmaps with an encoding of 1 for white
     * and 0 for black. We have rendering tools to convert a bitmap into
     * the required encoding.
     
     * \note The placement of the target bitmap is limited to the LCD's
     * boundary--otherwise this routine fails.
     *
     * In case you are wondering, 'blit' stands for Block Image Transfer.
     *
     * Sample code:
     * <pre>
     fb_bitBlit(fb,
     (const uint8_t *)pixel_data,
     width,
     height,
     0, // posx
     0  // posy
     );
     lcd_drawFrameBuffer(fb);
     </pre>
     */
    void bitBlit(const uint8_t *bitmap,
                 unsigned int   width,  /*!< width of the bitmap */
                 unsigned int   height, /*!< height of the bitmap */
                 unsigned int   posx,   /*!< x-offset for the
                                         * placement of the top-left
                                         * corner of the bitmap
                                         * w.r.t. the top-left
                                         * corner of the screen */
                 unsigned int   posy    /*!< y-offset for the
                                         * placement of the top-left
                                         * corner of the bitmap
                                         * w.r.t. the top-left
                                         * corner of the screen */
    );
    
    /*
     * \brief Fetch a byte (8-bit pixel sequence) from a given scan-line
     * in the framebuffer.
     *
     * The scan-line is identified by the row; and pixels are grouped into
     * 8-bit bytes within a row.
     *
     * \note This function is declared inline for a faster implementation.
     *
     * \param[in] framebuffer
     *     The framebuffer to fetch the byte from.
     *
     * \param[in] row
     *     The row index of the scan line.
     *
     * \param[in] byteIndex
     *     The pixel-index expressed as a byte-index.
     */
    uint8_t
    getRowByte(unsigned int row, unsigned int byteIndex) {
        return buffer[rowColToIndex(row, byteIndex)];
    }
    
    /*
     * \brief Set a byte (8-bit pixel sequence) for a given scan-line in
     * the framebuffer.
     *
     * The scan-line is identified by the row; and pixels are grouped into
     * 8-bit bytes within a row.
     *
     * \note This function is declared inline for a faster implementation.
     *
     * \param[in] framebuffer
     *     The framebuffer to set the byte into.
     *
     * \param[in] row
     *     The row index of the scan line.
     *
     * \param[in] byteIndex
     *     The pixel-index expressed as a byte-index.
     *
     * \param[in] pixels
     *     The actual 8 pixels to set.
     */
    void
    setRowByte(unsigned int row, unsigned int byteIndex, uint8_t pixels) {
        buffer[rowColToIndex(row, byteIndex)] = pixels;
    }
    
    /**
     * \brief The printf function for the frameBuffer.
     *
     * This can be used to render strings in a given
     * font-face. Internally, it uses fb_bitBlit to bilt the glyphs onto a
     * framebuffer. Currently, since bitBlit doesn't handle the case where
     * a bitmap exceeds the framebuffer's boundary, you must be very
     * careful about the placement of the text string. Later, when
     * fb_bitBlit is able to handle bitmaps which fall outside the LCD's
     * boundary, the rendered text may be clipped if it doesn't fit the
     * frame.
     *
     * \param[in] face
     *     The font-face to be used for rendering the text.
     *
     * \param[in] baselineX
     *     The X-offset from the left corner of the screen of the starting
     *     pen position; this defines the X-coordinate of the baseline.
     *
     * \param[in] baselineY
     *     The Y-offset from the top corner of the screen of the starting
     *     pen position; this defines the Y-coordinate of the baseline.
     *
     * \param[in] fgColor
     *     The foreground colour.
     *
     * \param[in] string
     *     The text to be rendered.
     *
     * Sample code:
     * <pre>
     *      face = lookupFontFace("DejaVu Serif", 9);
     *      if (face == NULL) {
     *          TRACE_FATAL("failed to find face for DejaVu Serif; 10\n");
     *      }
     *      fb_printString(fb,
     *                     face,
     *                     90,      // baselineX
     *                     140,     // baselineY
     *                     BLACK,   // foregroundColor
     *                     "Hello Mr. Obama!");
     *      lcd_drawFrameBuffer(fb);
     * </pre>
     */
    void printString(const font_face_t *face,
                     unsigned short     baselineX,
                     unsigned short     baselineY,
                     font_color_t       fgColor,
                     const char        *string);
    
    const uint8_t *getBuffer(void) const {
        return (buffer);
    }
    
    uint8_t *getBuffer(void) {
        return (buffer);
    }
    
private:
    unsigned rowColToIndex(unsigned row, unsigned col) {
        return (row * LCD_FRAMEBUFFER_SIZEOF_SCAN_LINE) + LCD_FRAMEBUFFER_SIZEOF_SCAN_LINE_METADATA + col;
    }
    
private:
    uint8_t *const buffer;
};

#endif /* Framebuffer_hpp */
