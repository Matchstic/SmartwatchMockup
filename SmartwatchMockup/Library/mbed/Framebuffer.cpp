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

#include "Framebuffer.hpp"

static uint8_t bitReverse8(uint8_t byte) {
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

FrameBuffer::FrameBuffer(uint8_t *fb) : buffer(fb)
{
    clear();
    
    /* Make sure the dummy bytes are clear */
    ((uint8_t *)buffer)[SIZEOF_FRAMEBUFFER]     = DUMMY8;
    ((uint8_t *)buffer)[SIZEOF_FRAMEBUFFER + 1] = DUMMY8;
}

void FrameBuffer::clear(void) {
    uint8_t     *pfb;
    unsigned int row;
    unsigned int index;
    
    /* initialize the frame buffer */
    pfb = buffer;
    for (row = 0; row < LCD_HEIGHT; row++) {
        for (index = 0; index < LCD_FRAMEBUFFER_SIZEOF_SCAN_LINE; index++) {
            switch (index) {
                case 0:
                    *pfb++ = M0_FLAG; /* update line command */
                    break;
                case 1:
                    *pfb++ = bitReverse8(row + 1);
                    break;
                default:
                    *pfb++ = 0x00; /* all white */
                    break;
            }
        }
    }
}

void FrameBuffer::bitBlit(const uint8_t *bitmap,
                               unsigned int   width,  /* width of the bitmap */
                               unsigned int   height, /* height of the bitmap */
                               unsigned int   posx,   /* x-offset of the top-left
                                                       * corner of the bitmap
                                                       * w.r.t. the top-left
                                                       * corner of the screen */
                               unsigned int   posy    /* y-offset of the top-left
                                                       * corner of the bitmap
                                                       * w.r.t. the top-left
                                                       * corner of the screen */)
{
    unsigned int row;
    unsigned int col;
    unsigned int bitsToBlitInRow;
    unsigned int destByteIndex; /* within the current row */
    unsigned int destBitIndexMod8; /* bits to the right of this index in
                                    * 'destByte' need to be blitted next;
                                    * MSB has index 0. */
    unsigned int srcByteIndex;  /* byte index within the source bitmap */
    unsigned int srcBitIndexMod8; /* bits to the right of this index in
                                   * source byte (at index srcByteIndex)
                                   * need to be blitted to the dest; MSB
                                   * has index 0. */
    const unsigned int endRow = posy + height;
    uint8_t      destByte;
    uint8_t      srcBits;
    
#if 0
    unsigned long current_cycle_count = Get_system_register(AVR32_COUNT);
#endif
    
    // ASSERT(bitmap != NULL, "%s: passed in a NULL bitmap", __FUNCTION__);
    // ASSERT(width > 0, "%s: passed in an invalid width", __FUNCTION__);
    // ASSERT(height > 0, "%s: passed in an invalid height", __FUNCTION__);
    // ASSERT(posx + width <= LCD_WIDTH,
    //        "%s: bitmap will exceed the screen width", __FUNCTION__);
    // ASSERT(posy + height <= LCD_HEIGHT,
    //        "%s: bitmap will exceed the screen height", __FUNCTION__);
    
    /*
     * A couple of helpful macros to maintain a source bitstream.
     */
#define SHIFT_INTO_SRC_BITS_FROM_SINGLE_SRC_BYTE(M) do {                \
uint8_t mask;                                                   \
\
mask      = ((uint8_t)((uint8_t)1 << (M)) - 1);                 \
srcBits <<= (M);                                                \
srcBits  |= (bitmap[srcByteIndex] >> (8 - (srcBitIndexMod8 + (M)))) & mask; \
\
/* update the indices */                                        \
srcBitIndexMod8 += (M);                                         \
if (srcBitIndexMod8 == 8) {                                     \
srcBitIndexMod8 = 0;                                        \
srcByteIndex++;                                             \
}                                                               \
} while (0)
    
    /* Left-shift N bits into srcBits; fetching them from the source
     * bitmap. */
#define SHIFT_INTO_SRC_BITS(N) do {                                          \
uint8_t bitsToShift;                                                 \
\
bitsToShift = (N);                                                   \
\
if ((srcBitIndexMod8 + (N)) > 8) {                                   \
bitsToShift = 8 - srcBitIndexMod8;                               \
SHIFT_INTO_SRC_BITS_FROM_SINGLE_SRC_BYTE(bitsToShift);           \
bitsToShift = (N) - bitsToShift;                                 \
}                                                                    \
\
SHIFT_INTO_SRC_BITS_FROM_SINGLE_SRC_BYTE(bitsToShift);               \
} while (0)
    
    srcByteIndex    = 0;
    srcBitIndexMod8 = 0;
    for (row = posy; row < endRow; row++) {
        col              = posx;
        bitsToBlitInRow  = width;
        destBitIndexMod8 = col & 0x7  /* col % 8 */;
        destByteIndex    = col >> 3   /* col / 8 */;
        srcBits          = 0;
        
        /* While there are bits in this row remaining to be blitted to
         * the destination, ... */
        while (bitsToBlitInRow) {
            if ((destBitIndexMod8 == 0) && (bitsToBlitInRow >= 8)) {
                /* We know that destBitIndexMod8 == 0, which means that
                 * the destination is byte aligned and we can simply
                 * do the equivalent of a memcpy. */
                while (bitsToBlitInRow >= 8) {
                    SHIFT_INTO_SRC_BITS(8);
                    setRowByte(row, destByteIndex, srcBits);
                    
                    bitsToBlitInRow -= 8;
                    destByteIndex++;
                }
            } else {
                uint8_t blit; /* number of bits to blit in this iteration */
                uint8_t mask;
                uint8_t leftShift;
                
                /* This will be a read-modify-write operation, so we
                 * need to fetch the destination byte. */
                destByte = getRowByte(row, destByteIndex);
                
                if ((destBitIndexMod8 + bitsToBlitInRow) >= 8) {
                    blit      = 8 - destBitIndexMod8;
                    leftShift = 0;
                } else {
                    blit      = bitsToBlitInRow;
                    leftShift = (8 - (destBitIndexMod8 + bitsToBlitInRow));
                }
                
                SHIFT_INTO_SRC_BITS(blit);
                mask = ((uint8_t)((uint8_t)1 << blit) - 1) << leftShift;
                
                /* update destByte */
                destByte &= ~mask;
                destByte |= ((uint8_t)(srcBits << leftShift) & mask);
                
                setRowByte(row, destByteIndex, destByte);
                
                /* update dest indices */
                bitsToBlitInRow  -= blit;
                destBitIndexMod8 += blit;
                if (destBitIndexMod8 == 8) {
                    destBitIndexMod8 = 0;
                    destByteIndex++;
                }
            }
        }
        
        /* potentially update srcByteIndex */
        if (srcBitIndexMod8 != 0) {
            srcBitIndexMod8 = 0;
            srcByteIndex++;
        }
    }
}

void FrameBuffer::printString(const font_face_t *face,
                                   unsigned short     baselineX,
                                   unsigned short     baselineY,
                                   font_color_t       fgColor,
                                   const char        *string)
{
    unsigned char ch;
    while ((ch = *string++) != '\0') {
        if ((ch < FONT_LOWEST_SUPPORTED_CHAR_ENCODING) ||
            (ch > FONT_HIGHEST_SUPPORTED_CHAR_ENCODING)) {
            // TRACE_WARNING("printString: attempt to render unsupported char (enc: 0x%x)\n", ch);
            continue;
        }
        
        const glyph_t *glyph = &face->glyphs[ch - FONT_LOWEST_SUPPORTED_CHAR_ENCODING];
        
        if ((glyph->width > 0) && (glyph->height > 0)) {
            bitBlit(&face->bitmaps[glyph->bitmapIndex], /* bitmap */
                    glyph->width,  /* width of the bitmap */
                    glyph->height, /* height of the bitmap */
                    baselineX + glyph->bearingX, /* x-offset for the
                                                  * placement of the top-left
                                                  * corner of the bitmap. */
                    baselineY - glyph->bearingY  /* y-offset for the
                                                  * placement of the top-left
                                                  * corner of the bitmap. */
                    );
        }
        
        baselineX += glyph->advance;
    }
}
