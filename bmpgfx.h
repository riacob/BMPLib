#ifndef BMPGFX_H
#define BMPGFX_H

#include "bmplib.h"

bmp_pixel_t bmp_gfx_conv_rgb(uint8_t r, uint8_t g, uint8_t b);
void bmp_gfx_draw_pixel(bmp_file_t *f, uint32_t x, uint32_t y, bmp_pixel_t color);
void bmp_gfx_draw_line(bmp_file_t *f, uint32_t startx, uint32_t starty, uint32_t sizex, uint32_t sizey, bmp_pixel_t color);
void bmp_gfx_draw_square_fill(bmp_file_t *f, uint32_t startx, uint32_t starty, uint32_t endx, uint32_t endy, bmp_pixel_t color);
void bmp_gfx_draw_square(bmp_file_t *f, uint32_t startx, uint32_t starty, uint32_t sizex, uint32_t sizey, bmp_pixel_t color);

#endif