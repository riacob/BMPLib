#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bmplib.h"
#include "bmpgfx.h"
#include "bmptxt.h"

int main(int argc, char *argv[])
{
    FILE *f, *f1, *ff;
    bmp_file_t *bmpf, *bmpf1;
    bmp_font_t *font;
    bmp_pixel_t color = {
        .R = 255,
        .G = 133,
        .B = 255
    };
    f = fopen(argv[1], "r");
    assert(f);
    bmpf = bmp_read(f);
    assert(bmpf);
    fclose(f);
    //bmp_print_info(bmpf,stdout);
    f1 = fopen(argv[2], "w");
    ff = fopen(argv[3], "r");
    assert(f1 && ff);
    font = bmp_txt_load_font(ff);
    assert(font);
    bmp_gfx_draw_rect_fill(bmpf,200,130,197,200,color);
    bmp_gfx_draw_line(bmpf,0,50,100,55,bmp_gfx_conv_rgb(0,255,127));
    bmp_gfx_draw_rect(bmpf,10,10,70,150,bmp_gfx_conv_rgb(255,255,32));
    bmp_write(bmpf, f1);
    fclose(f1);
    bmp_free(bmpf);
    f1 = fopen(argv[2], "r");
    assert(f1);
    bmpf1 = bmp_read(f1);
    assert(bmpf1);
    fclose(f1);
    //bmp_print_info(bmpf1,stdout);
    bmp_free(bmpf1);
    /*bmp_file_t *bmpf = bmp_create_24bpp(500,500);
    FILE *f = fopen(argv[1], "w");
    assert(bmpf && f);
    bmp_write(bmpf,f);
    fclose(f);
    bmp_free(bmpf);*/

    return EXIT_SUCCESS;
}