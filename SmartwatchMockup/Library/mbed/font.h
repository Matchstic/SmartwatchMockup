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

#include <stdint.h>

#ifndef __GUI_FONT_H__
#define __GUI_FONT_H__

#ifdef __cplusplus
extern "C" {
#endif
    
    /*----------------------------------------------------------------------
     * Type declarations
     *----------------------------------------------------------------------*/
    
    typedef struct glyph_t {
        int8_t       bearingX;  /* The horizontal distance from the
                                 * current pen position to the glyph's
                                 * left bbox edge. */
        int8_t       bearingY;  /* The vertical distance from the
                                 * baseline to the top of the glyph's
                                 * bbox. */
        unsigned int width       : 6; /* The glyph's horizontal extent. */
        unsigned int height      : 6; /* The glyph's vertical extent. */
        unsigned int advance     : 6; /* The horizontal distance the
                                       * pen position must be
                                       * incremented (for
                                       * left-to-right writing) or
                                       * decremented (for
                                       * right-to-left writing) by
                                       * after each glyph is rendered
                                       * when processing text. */
        unsigned int bitmapIndex : 14; /* byte-index of the start of
                                        * the glyph's bitmap as
                                        * contained in the font-face
                                        * bitmaps array */
    } __attribute__((__packed__)) glyph_t;
    
    typedef struct font_face_t {
        const char    *familyName;
        unsigned int   pointSize;
        const glyph_t *glyphs;
        const uint8_t *bitmaps;
    } font_face_t;
    
    
#define FONT_LOWEST_SUPPORTED_CHAR_ENCODING  ' '
#define FONT_HIGHEST_SUPPORTED_CHAR_ENCODING '~'
    
    
    typedef enum font_color_t {
        BLACK = 0,
        WHITE = 1
    } font_color_t;
    
    
    /*----------------------------------------------------------------------
     * External globals
     *----------------------------------------------------------------------*/
    
    extern const font_face_t fonts[];
    
    
    /*----------------------------------------------------------------------
     * Exported functions.
     *----------------------------------------------------------------------*/
    
    extern const font_face_t *lookupFontFace(const char   *familyName,
                                             unsigned int  pointSize);
    
#ifdef __cplusplus
}
#endif

#endif  /* __GUI_FONT_H__ */
