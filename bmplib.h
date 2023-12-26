#ifndef BMPLIB_H
#define BMPLIB_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#define NOT_IMPL(x) printf("\n\nBMP_NOT_IMPL: %s\n\n", x); exit(EXIT_FAILURE);
#define BMP_SIGNATURE 19778

enum {
    BMP_ADDR_SIGNATURE = 0x0000,
    BMP_ADDR_FILESIZE = 0x0002,
    BMP_ADDR_RESERVED = 0x0006,
    BMP_ADDR_DATAOFFSET = 0x000A,
    BMP_ADDR_INFOHEADERSIZE = 0x000E,
    BMP_ADDR_WIDTH = 0x0012,
    BMP_ADDR_HEIGHT = 0x0016,
    BMP_ADDR_PLANES = 0x001A,
    BMP_ADDR_BPP = 0x001C,
    BMP_ADDR_COMPRESSION = 0x001E,
    BMP_ADDR_IMAGESIZE = 0x0022,
    BMP_ADDR_XPPM = 0x0026,
    BMP_ADDR_YPPM = 0x002A,
    BMP_ADDR_COLORSUSED = 0x002E,
    BMP_ADDR_IMPORTANTCOLORS = 0x0032,
    BMP_ADDR_COLORTABLE = 0x0036
} bmp_addr_e;

typedef struct
{
    uint8_t B;
    uint8_t G;
    uint8_t R;
} bmp_pixel_t;

typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t reserved;
} bmp_colortable_t;

typedef struct
{
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
} bmp_header_t;

typedef struct
{
    uint32_t infoHeaderSize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t xPixelsPerM;
    uint32_t yPixelsPerM;
    uint32_t colorsUsed;
    uint32_t importantColors;
} bmp_infoheader_t;

typedef struct
{
    bmp_header_t *header;
    bmp_infoheader_t *infoHeader;
    bmp_colortable_t *colorTable;
    bmp_pixel_t *pixels;
} bmp_file_t;

/**
 * @brief Reads a file as a bitmap
 * 
 * @param f: Pointer to file
 * @return bmp_file_t*: Pointer to bitmap allocation
 */
bmp_file_t *bmp_read(FILE *f);
/**
 * @brief Writes bitmap to file
 * 
 * @param bmpf: Bitmap
 * @param f: Pointer to file
 */
void bmp_write(bmp_file_t *bmpf, FILE *f);
/**
 * @brief Prints header & infoHeader
 * 
 * @param bmpf: Bitmap 
 * @param stream: Output stream
 */
void bmp_print_info(bmp_file_t *bmpf, FILE *stream);
/**
 * @brief Frees the memory occupied by the bitmap
 * 
 * @param bmpf: Bitmap 
 */
void bmp_free(bmp_file_t *bmpf);
/**
 * @brief TODO Creates a 24bpp uncompressed bitmap
 * 
 * @param xSize: Width of bitmap
 * @param ySize: Height of bitmap
 * @return bmp_file_t*: Pointer to bitmap allocation
 */
bmp_file_t* bmp_create_24bpp(uint32_t xSize, uint32_t ySize);

#endif