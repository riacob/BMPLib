#include "bmpgfx.h"

static uint32_t cordsToIdx(bmp_file_t *f, uint32_t x, uint32_t y)
{
    assert(f);
    // Literal nonsense, but fixes all drawing issues
    x -= y + 112;
    y = f->infoHeader->height - y;
    return x + (f->infoHeader->width * y);
}

static void drawVerticalLine(bmp_file_t *f, uint32_t x, uint32_t starty, uint32_t endy, bmp_pixel_t color)
{
    assert(f);
    if (starty < endy)
    {
        for (uint32_t y = starty; y < endy + starty; y++)
        {
            bmp_gfx_draw_pixel(f, x, y, color);
        }
    }
    else
    {
        for (uint32_t y = endy; y > starty; y--)
        {
            bmp_gfx_draw_pixel(f, x, y, color);
        }
    }
}

static void drawHorizontalLine(bmp_file_t *f, uint32_t y, uint32_t startx, uint32_t endx, bmp_pixel_t color)
{
    assert(f);
    if (startx < endx)
    {
        for (uint32_t x = startx; x < endx + startx; x++)
        {
            bmp_gfx_draw_pixel(f, x, y, color);
        }
    }
    else
    {
        for (uint32_t x = endx; x > startx; x--)
        {
            bmp_gfx_draw_pixel(f, x, y, color);
        }
    }
}

static void drawLineLow(bmp_file_t *f, uint32_t startx, uint32_t starty, uint32_t endx, uint32_t endy, bmp_pixel_t color)
{
    int32_t dx = endx - startx;
    int32_t dy = endy - starty;
    int32_t yi = 1;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }
    int32_t D = (2 * dy) - dx;
    int32_t y = starty;

    for (int32_t x = startx; x < startx + endx; x++)
    {
        bmp_gfx_draw_pixel(f, x, y, color);
        if (D > 0)
        {
            y = y + yi;
            D = D + (2 * (dy - dx));
        }
        else
        {
            D = D + 2 * dy;
        }
    }
}

static void drawLineHigh(bmp_file_t *f, uint32_t startx, uint32_t starty, uint32_t endx, uint32_t endy, bmp_pixel_t color)
{
    int32_t dx = endx - startx;
    int32_t dy = endy - starty;
    int32_t xi = 1;
    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }
    int32_t D = (2 * dy) - dx;
    int32_t x = startx;

    for (int32_t y = starty; y < starty + endy; y++)
    {
        bmp_gfx_draw_pixel(f, x, y, color);
        if (D > 0)
        {
            x = x + xi;
            D = D + (2 * (dx - dy));
        }
        else
        {
            D = D + 2 * dx;
        }
    }
}

bmp_pixel_t bmp_gfx_conv_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    bmp_pixel_t pix;
    pix.R = r;
    pix.G = g;
    pix.B = b;
    return pix;
}

void bmp_gfx_draw_square_fill(bmp_file_t *f, uint32_t startx, uint32_t starty, uint32_t sizex, uint32_t sizey, bmp_pixel_t color)
{
    assert(f);
    // Crop drawing if it doesn't fit in the image
    if (startx + sizex > f->infoHeader->width)
    {
        sizex -= (f->infoHeader->width - sizex);
    }
    if (starty + sizey > f->infoHeader->height)
    {
        sizey -= (f->infoHeader->width - sizey);
    }
    // Draw the square line by line
    for (uint32_t y = starty; y < sizey + starty; y++)
    {
        uint32_t x;
        for (x = startx; x < sizex + startx; x++)
        {
            bmp_gfx_draw_pixel(f, x, y, color);
        }
    }
}

void bmp_gfx_draw_pixel(bmp_file_t *f, uint32_t x, uint32_t y, bmp_pixel_t color)
{
    f->pixels[cordsToIdx(f, x, y)] = color;
}

void bmp_gfx_draw_line(bmp_file_t *f, uint32_t startx, uint32_t starty, uint32_t endx, uint32_t endy, bmp_pixel_t color)
{
    assert(f);
    uint32_t sizex = endx - startx;
    uint32_t sizey = endy - starty;
    // Crop drawing if it doesn't fit in the image
    if (startx + sizex > f->infoHeader->width)
    {
        sizex -= (f->infoHeader->width - sizex);
    }
    if (starty + sizey > f->infoHeader->height)
    {
        sizey -= (f->infoHeader->width - sizey);
    }
    if (startx == endx)
    {
        drawVerticalLine(f, startx, starty, endy, color);
    }
    else if (starty == endy)
    {
        drawHorizontalLine(f, starty, startx, endx, color);
    }
    else
    {
        if (abs(endy - starty) < abs(endx - startx))
        {
            if (startx > endx)
            {
                drawLineLow(f, startx, starty, endx, endy, color);
            }
            else
            {
                drawLineHigh(f, startx, starty, endx, endy, color);
            }
        }
        else
        {
            if (starty > endy)
            {
                drawLineHigh(f, startx, starty, endx, endy, color);
            }
            else
            {
                drawLineLow(f, startx, starty, endx, endy, color);
            }
        }
    }
}

void bmp_gfx_draw_square(bmp_file_t *f, uint32_t startx, uint32_t starty, uint32_t sizex, uint32_t sizey, bmp_pixel_t color)
{
    assert(f);
    // Crop drawing if it doesn't fit in the image
    if (startx + sizex > f->infoHeader->width)
    {
        sizex -= (f->infoHeader->width - sizex);
    }
    if (starty + sizey > f->infoHeader->height)
    {
        sizey -= (f->infoHeader->width - sizey);
    }
    // From startx,starty to endx,starty
    bmp_gfx_draw_line(f, startx, starty, startx + sizex, starty, color);
    // From startx,endy to endx,endy
    bmp_gfx_draw_line(f, startx, starty + starty + sizey, startx + sizex, starty + starty + sizey, color);
    // From startx,starty to startx,endy
    bmp_gfx_draw_line(f, startx, starty, startx, starty + sizey, color);
    // From endx,starty to endx, endy
    bmp_gfx_draw_line(f, startx + startx + sizex, starty, startx + startx + sizex, starty + sizey, color);
}