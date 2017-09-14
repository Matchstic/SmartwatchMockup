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

#include <string.h>
#include <stddef.h>

#include "font.h"

/* Externs private to the font subsystem. */
/* extern const glyph_t glyphs_DejaVu_Serif_10[]; */
/* extern const uint8_t bitmaps_DejaVu_Serif_10[]; */
extern const glyph_t glyphs_DejaVu_Serif_9[];
extern const uint8_t bitmaps_DejaVu_Serif_9[];
extern const glyph_t glyphs_DejaVu_Serif_8[];
extern const uint8_t bitmaps_DejaVu_Serif_8[];

/* Accumulation of all avaialble fonts */
const font_face_t fonts[] = {
    /* { */
    /*     "DejaVu Serif", */
    /*     10,   /\* pointSize *\/ */
    /*     glyphs_DejaVu_Serif_10, */
    /*     bitmaps_DejaVu_Serif_10 */
    /* }, */
    {
        "DejaVu Serif",
        9,   /* pointSize */
        glyphs_DejaVu_Serif_9,
        bitmaps_DejaVu_Serif_9
    },
    {
        "DejaVu Serif",
        8,   /* pointSize */
        glyphs_DejaVu_Serif_8,
        bitmaps_DejaVu_Serif_8
    },
    
    /* sentinel value */
    {
        NULL,
        0,
        NULL,
        NULL
    }
};


const font_face_t *
lookupFontFace(const char  *familyName,
               unsigned int pointSize)
{
    unsigned fontIndex;
    
    for (fontIndex = 0; fonts[fontIndex].familyName != NULL; fontIndex++) {
        if ((strcmp(fonts[fontIndex].familyName, familyName) == 0) &&
            (fonts[fontIndex].pointSize == pointSize)) {
            /* found it! */
            return (&fonts[fontIndex]);
        }
    }
    
    return (NULL);
}
