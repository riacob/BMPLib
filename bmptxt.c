#include "bmptxt.h"

static void printSymbolBitmap(bmp_file_t *f, bmp_font_t *font, bmp_fontsymbol_t *s, uint32_t startx, uint32_t starty, bmp_pixel_t color)
{
    assert(s);
    for (uint32_t y = starty; y < starty + font->sizey; y++)
    {
        for (uint32_t x = startx; x < startx + font->sizex; y++)
        {
            bmp_gfx_draw_pixel(f, x, y, color);
        }
    }
}

static void printSymbol(bmp_file_t *f, bmp_font_t *font, bmp_fontsymbol_t *cur, uint32_t startx, uint32_t starty, uint32_t *curx, uint32_t *cury, int wrap, uint32_t hpadding, uint32_t vpadding, bmp_pixel_t color)
{
    assert(f && font && cur);
    // If we can fit cur in the same line
    if (!(*curx + font->sizex) < hpadding)
    {
        printSymbolBitmap(f, font, cur, *curx, *cury, color);
        *curx += font->sizex;
    }
    else
    {
        // If word wrapping is enabled and we can fit cur in a new line
        if (wrap && ((*cury + font->sizey) < vpadding))
        {
            *curx = startx;
            cury += font->sizey;
        }
    }
}

static bmp_fontsymbol_t *getSymbolFromChar(bmp_font_t *fontData, char c)
{
    assert(fontData);
    // Scan thtough available symbols
    for (uint32_t i = 0; i < fontData->nSymbols; i++)
    {
        if (fontData->symbols[i].c == c)
        {
            return &fontData->symbols[i];
        }
    }
    return NULL;
}

void bmp_txt_print(bmp_file_t *f, bmp_font_t *font, char *text, uint32_t startx, uint32_t starty, int wrap, uint32_t hpadding, uint32_t vpadding, bmp_pixel_t color)
{
    assert(f && font && text);
    size_t len = strlen(text);
    uint32_t cx, cy;
    // Print characher by charachter
    for (size_t i = 0; i < len - 1; i++)
    {
        printSymbol(f, font, getSymbolFromChar(font, text[i]), startx, starty, &cx, &cy, wrap, hpadding, vpadding, color);
    }
}

bmp_font_t *bmp_txt_load_font(FILE *f)
{
    /**
     * File structure:
     * 0x0000 nSymbols
     * 0x0004 sizex
     * 0x0008 sizey
     * 0x000C symbols
     * 
     * symbols: 
     * symbols+0x0000*nSymbol = char
     * symbols+0x0001*nSymbol = bmp_pixel_t
     */
    assert(f);
    bmp_font_t *ptr = (bmp_font_t *)malloc(sizeof(bmp_font_t));
    bmp_fontsymbol_t *symbols;
    // Number of symbols
    fseek(f, 0, SEEK_SET);
    fread(&ptr->nSymbols, 4, 1, f);
    printf("%d\n", ptr->nSymbols);
    symbols = (bmp_fontsymbol_t *)malloc(sizeof(bmp_fontsymbol_t) * ptr->nSymbols);
    // Size x
    fseek(f, 4, SEEK_SET);
    fread(&ptr->sizex, 4, 1, f);
    // Size y
    fseek(f, 8, SEEK_SET);
    fread(&ptr->sizey, 4, 1, f);
    // (c & bmp_pixel_t) * i : i < number of symbols
    for(uint32_t i = 0; i < ptr->nSymbols; i+=4) {
        fseek(f,12+i,SEEK_SET);
        fread(&symbols->c, 1, 1, f);
        fread(symbols->bitmap, 3, 1, f);
    }
    return ptr;
}