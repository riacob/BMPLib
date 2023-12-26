#ifndef BMPTXT_H
#define BMPTXT_H

#include "bmplib.h"
#include "bmpgfx.h"
#include <string.h>

typedef struct
{
    char c;
    bmp_pixel_t *bitmap;
} bmp_fontsymbol_t;

typedef struct
{
    bmp_fontsymbol_t *symbols;
    uint32_t nSymbols;
    uint32_t sizex;
    uint32_t sizey;
} bmp_font_t;

/**
 * @brief Prints text on bitmap
 *
 * @param f: Bitmap
 * @param font: Font
 * @param text: Text to be printed
 * @param startx: X start coordinate
 * @param starty: Y start coordinate
 * @param wrap: Word wrapping, 1 = enabled 0 = disabled (truncate)
 * @param hpadding: Number of pixels from the left of the screen where string gets split
 * @param vpadding: Number of pixels from the top of the screen where string gets truncated
 * @param color: Color of the text
 */
void bmp_txt_print(bmp_file_t *f, bmp_font_t *font, char *text, uint32_t startx, uint32_t starty, int wrap, uint32_t hpadding, uint32_t vpadding, bmp_pixel_t color);
/**
 * @brief Loads font from file
 * 
 * @param f: File pointer
 * @return bmp_font_t*: Pointer to font allocation
 */
bmp_font_t *bmp_txt_load_font(FILE *f);

#endif