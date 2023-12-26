#ifndef BMPGFX_H
#define BMPGFX_H

#include "bmplib.h"

/**
 * @brief Converts RGB values to bmp_pixel_t
 * 
 * @param r: R value (0-255)
 * @param g: G value (0-255)
 * @param b: B value (0-255)
 * @return bmp_pixel_t: single pixel
 */
bmp_pixel_t bmp_gfx_conv_rgb(uint8_t r, uint8_t g, uint8_t b);
/**
 * @brief Draws pixel on bitmap
 * 
 * @param f: Bitmap
 * @param x: X coordinate
 * @param y: Y coordinate
 * @param color: Pixel color
 */
void bmp_gfx_draw_pixel(bmp_file_t *f, uint32_t x, uint32_t y, bmp_pixel_t color);
/**
 * @brief Draws line on bitmap
 * 
 * @param f: Bitmap
 * @param startx: X start coordinate
 * @param starty: Y start coordinate
 * @param endx: X end coordinate 
 * @param enxy: Y end coordinate
 * @param color: Line color
 */
void bmp_gfx_draw_line(bmp_file_t *f, uint32_t startx, uint32_t starty, uint32_t endx, uint32_t endy, bmp_pixel_t color);
/**
 * @brief Draws filled rectangle on bitmap
 * 
 * @param f: Bitmap
 * @param startx: X start coordinate
 * @param starty: Y start coordinate
 * @param sizex: Width
 * @param sizey: Height
 * @param color: Rectangle color
 */
void bmp_gfx_draw_rect_fill(bmp_file_t *f, uint32_t startx, uint32_t starty, uint32_t sizex, uint32_t sizey, bmp_pixel_t color);
/**
 * @brief Draws rectangle on bitmap
 * 
 * @param f: Bitmap
 * @param startx: X start coordinate
 * @param starty: Y start coordinate 
 * @param sizex: Width 
 * @param sizey: Height 
 * @param color: Rectangle color 
 */
void bmp_gfx_draw_rect(bmp_file_t *f, uint32_t startx, uint32_t starty, uint32_t sizex, uint32_t sizey, bmp_pixel_t color);

#endif