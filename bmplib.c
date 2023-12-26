#include "bmplib.h"

static int isBitmap(FILE *f)
{
    assert(f);
    uint16_t signature;
    fseek(f, BMP_ADDR_SIGNATURE, SEEK_SET);
    fread(&signature, 2, 1, f);
    return ((signature == BMP_SIGNATURE) ? 1 : 0);
}

static uint32_t getNColors(uint16_t bpp)
{
    switch (bpp)
    {
    case 1:
        return 1;
    case 4:
        return 16;
    case 8:
        return 256;
    case 16:
        return 65535;
    case 24:
        return 16777216;
    }
    return 0;
}

static uint32_t getDataSize(FILE *f)
{
    assert(f);
    uint32_t size = 0;
    fseek(f, BMP_ADDR_IMAGESIZE, SEEK_SET);
    fread(&size, 4, 1, f);
    return size;
}

static uint32_t getDataOffset(FILE *f)
{
    assert(f);
    uint32_t dOfst;
    fseek(f, BMP_ADDR_DATAOFFSET, SEEK_SET);
    fread(&dOfst, 4, 1, f);
    return dOfst;
}

static uint16_t getBPP(FILE *f)
{
    assert(f);
    uint16_t bpp;
    fseek(f,BMP_ADDR_BPP, SEEK_SET);
    fread(&bpp, 2, 1, f);
    return bpp;
}

static bmp_header_t *read_header(FILE *f)
{
    bmp_header_t *ptr = (bmp_header_t *)malloc(sizeof(bmp_header_t));
    assert(ptr && f);
    // 2 signature
    fseek(f, BMP_ADDR_SIGNATURE, SEEK_SET);
    fread(&ptr->signature, 2, 1, f);
    // 4 file size
    fseek(f, BMP_ADDR_FILESIZE, SEEK_SET);
    fread(&ptr->fileSize, 4, 1, f);
    // 4 reserved
    fseek(f, BMP_ADDR_RESERVED, SEEK_SET);
    fread(&ptr->reserved, 4, 1, f);
    // 4 data offset
    fseek(f, BMP_ADDR_DATAOFFSET, SEEK_SET);
    fread(&ptr->dataOffset, 4, 1, f);
    return ptr;
}

static bmp_infoheader_t *read_info_header(FILE *f)
{
    bmp_infoheader_t *ptr = (bmp_infoheader_t *)malloc(sizeof(bmp_infoheader_t));
    assert(ptr && f);
    // 4 info header size
    fseek(f, BMP_ADDR_INFOHEADERSIZE, SEEK_SET);
    fread(&ptr->infoHeaderSize, 4, 1, f);
    // 4 width
    fseek(f, BMP_ADDR_WIDTH, SEEK_SET);
    fread(&ptr->width, 4, 1, f);
    // 4 height
    fseek(f, BMP_ADDR_HEIGHT, SEEK_SET);
    fread(&ptr->height, 4, 1, f);
    // 2 planes
    fseek(f, BMP_ADDR_PLANES, SEEK_SET);
    fread(&ptr->planes, 2, 1, f);
    // 2 bits per pixel
    fseek(f, BMP_ADDR_BPP, SEEK_SET);
    fread(&ptr->bpp, 2, 1, f);
    // 4 compression
    fseek(f, BMP_ADDR_COMPRESSION, SEEK_SET);
    fread(&ptr->compression, 4, 1, f);
    // 4 image size
    fseek(f, BMP_ADDR_IMAGESIZE, SEEK_SET);
    fread(&ptr->imageSize, 4, 1, f);
    // 4 xppm
    fseek(f, BMP_ADDR_XPPM, SEEK_SET);
    fread(&ptr->xPixelsPerM, 4, 1, f);
    // 4 yppm
    fseek(f, BMP_ADDR_YPPM, SEEK_SET);
    fread(&ptr->yPixelsPerM, 4, 1, f);
    // 4 colors used
    fseek(f, BMP_ADDR_COLORSUSED, SEEK_SET);
    fread(&ptr->colorsUsed, 4, 1, f);
    // 4 important colors
    fseek(f, BMP_ADDR_IMPORTANTCOLORS, SEEK_SET);
    fread(&ptr->importantColors, 4, 1, f);
    return ptr;
}

static bmp_colortable_t *read_color_table(FILE *f)
{
    bmp_colortable_t *ptr;
    assert(ptr && f);
    uint16_t bpp;
    uint32_t nColors;
    // Get bpp
    fseek(f, BMP_ADDR_BPP, SEEK_SET);
    fread(&bpp, 2, 1, f);
    // Get nColors
    nColors = getNColors(bpp);
    nColors = 16;
    ptr = (bmp_colortable_t *)malloc(sizeof(bmp_colortable_t) * nColors);
    // Get color table
    fseek(f, BMP_ADDR_COLORTABLE, SEEK_SET);
    fread(ptr, sizeof(bmp_colortable_t), nColors, f);
    return ptr;
}

static bmp_pixel_t *read_data(FILE *f)
{
    assert(f);
    bmp_pixel_t *ptr;
    uint16_t bpp = getBPP(f);
    uint32_t size = getDataSize(f);
    uint32_t dataOffset = getDataOffset(f);
    if (bpp < 24)
    {
        NOT_IMPL("parsing < 24 bpp");
    }
    ptr = (bmp_pixel_t *)malloc(size);
    assert(ptr);
    fseek(f, dataOffset, SEEK_SET);
    fread(ptr, 3, size/3, f);
    return ptr;
}

bmp_file_t *bmp_read(FILE *f)
{
    bmp_file_t *bmpf = (bmp_file_t *)malloc(sizeof(bmp_file_t));
    assert(bmpf && f);
    assert(isBitmap(f));
    bmpf->header = read_header(f);
    bmpf->infoHeader = read_info_header(f);
    // Color table is present only if we are working with less than 8 bpp
    if (bmpf->infoHeader->bpp < 8)
    {
        bmpf->colorTable = read_color_table(f);
    }
    else
    {
        bmpf->colorTable = NULL;
    }
    // If image is compressed we need to run decompression
    if (bmpf->infoHeader->compression)
    {
        NOT_IMPL("Decompression");
    }
    bmpf->pixels = read_data(f);
    return bmpf;
}

void bmp_print_info(bmp_file_t *bmpf, FILE *stream)
{
    assert(bmpf && stream);
    fprintf(stream, "BMP file information: \n");
    fprintf(stream, "* HEADER\n");
    fprintf(stream, "Signature: %d\n", bmpf->header->signature);
    fprintf(stream, "FileSize: %d\n", bmpf->header->fileSize);
    fprintf(stream, "Reserved: %d\n", bmpf->header->reserved);
    fprintf(stream, "DataOffset: %d\n", bmpf->header->dataOffset);
    fprintf(stream, "* INFOHEADER\n");
    fprintf(stream, "InfoHeaderSize: %d\n", bmpf->infoHeader->infoHeaderSize);
    fprintf(stream, "Width: %d\n", bmpf->infoHeader->width);
    fprintf(stream, "Height: %d\n", bmpf->infoHeader->height);
    fprintf(stream, "Planes: %d\n", bmpf->infoHeader->planes);
    fprintf(stream, "BitsPerPixel: %d ", bmpf->infoHeader->bpp);
    switch (bmpf->infoHeader->bpp)
    {
    case 1:
        fprintf(stream, "(monochrome)\n");
        break;
    case 4:
        fprintf(stream, "(16 colors)\n");
        break;
    case 8:
        fprintf(stream, "(256 colors)\n");
        break;
    case 16:
        fprintf(stream, "(65536 colors)\n");
        break;
    case 24:
        fprintf(stream, "(16777216 colors)\n");
        break;
    }
    fprintf(stream, "Compression: ");
    switch (bmpf->infoHeader->compression)
    {
    case 0:
        fprintf(stream, "BI_RGB (none)\n");
        break;
    case 1:
        fprintf(stream, "BI_RLE8 (8 bit RLE)\n");
        break;
    case 2:
        fprintf(stream, "BI_RLE4 (4 bit RLE)\n");
        break;
    }
    fprintf(stream, "CompressedImageSize: %d\n", bmpf->infoHeader->imageSize);
    fprintf(stream, "xPixelsPerM: %d\n", bmpf->infoHeader->xPixelsPerM);
    fprintf(stream, "yPixelsPerM: %d\n", bmpf->infoHeader->yPixelsPerM);
    fprintf(stream, "ColorsUsed: %d\n", bmpf->infoHeader->colorsUsed);
    fprintf(stream, "ImportantColors: %d\n", bmpf->infoHeader->importantColors);
    fprintf(stream, "* COLORTABLE\n");
    if (!bmpf->colorTable)
    {
        fprintf(stream, "Not present (bpp > 8)\n\n");
    }
    else
    {
        fprintf(stream, "Present (bpp < 8)\n\n");
    }
}

void bmp_free(bmp_file_t *bmpf)
{
    free(bmpf->header);
    free(bmpf->infoHeader);
    free(bmpf->colorTable);
    free(bmpf->pixels);
}

static void bmp_write_header(bmp_file_t *bmpf, FILE *f)
{
    assert(bmpf && f);
    fseek(f, BMP_ADDR_SIGNATURE, SEEK_SET);
    fwrite(&bmpf->header->signature, 2, 1, f);
    fseek(f, BMP_ADDR_FILESIZE, SEEK_SET);
    fwrite(&bmpf->header->fileSize, 4, 1, f);
    fseek(f, BMP_ADDR_RESERVED, SEEK_SET);
    fwrite(&bmpf->header->reserved, 4, 1, f);
    fseek(f, BMP_ADDR_DATAOFFSET, SEEK_SET);
    fwrite(&bmpf->header->dataOffset, 4, 1, f);
}

static void bmp_write_infoheader(bmp_file_t *bmpf, FILE *f)
{
    assert(bmpf && f);
    fseek(f, BMP_ADDR_INFOHEADERSIZE, SEEK_SET);
    fwrite(&bmpf->infoHeader->infoHeaderSize, 4, 1, f);
    fseek(f, BMP_ADDR_WIDTH, SEEK_SET);
    fwrite(&bmpf->infoHeader->width, 4, 1, f);
    fseek(f, BMP_ADDR_HEIGHT, SEEK_SET);
    fwrite(&bmpf->infoHeader->height, 4, 1, f);
    fseek(f, BMP_ADDR_PLANES, SEEK_SET);
    fwrite(&bmpf->infoHeader->planes, 2, 1, f);
    fseek(f, BMP_ADDR_BPP, SEEK_SET);
    fwrite(&bmpf->infoHeader->bpp, 2, 1, f);
    fseek(f, BMP_ADDR_COMPRESSION, SEEK_SET);
    fwrite(&bmpf->infoHeader->compression, 4, 1, f);
    fseek(f, BMP_ADDR_IMAGESIZE, SEEK_SET);
    fwrite(&bmpf->infoHeader->imageSize, 4, 1, f);
    fseek(f, BMP_ADDR_XPPM, SEEK_SET);
    fwrite(&bmpf->infoHeader->xPixelsPerM, 4, 1, f);
    fseek(f, BMP_ADDR_YPPM, SEEK_SET);
    fwrite(&bmpf->infoHeader->yPixelsPerM, 4, 1, f);
    fseek(f, BMP_ADDR_COLORSUSED, SEEK_SET);
    fwrite(&bmpf->infoHeader->colorsUsed, 4, 1, f);
    fseek(f, BMP_ADDR_IMPORTANTCOLORS, SEEK_SET);
    fwrite(&bmpf->infoHeader->importantColors, 4, 1, f);
}

static void bmp_write_colortable(bmp_file_t *bmpf, FILE *f)
{
    assert(f);
    if (bmpf->colorTable)
    {
        fseek(f, BMP_ADDR_COLORTABLE, SEEK_SET);
        fwrite(bmpf->colorTable, 4, getNColors(bmpf->infoHeader->bpp), f);
    }
}

void bmp_write(bmp_file_t *bmpf, FILE *f)
{
    assert(bmpf && f);
    bmp_write_header(bmpf, f);
    bmp_write_infoheader(bmpf, f);
    bmp_write_colortable(bmpf, f);
    bmp_write_data(bmpf, f);
}

void bmp_raw_drawSquare(uint32_t startx, uint32_t starty, uint32_t sizex, uint32_t sizey, bmp_pixel_t color)
{

}

static void bmp_write_data(bmp_file_t *bmpf, FILE *f)
{
    assert(bmpf && f);
    fseek(f, bmpf->header->dataOffset, SEEK_SET);
    fwrite(bmpf->pixels, 3, bmpf->infoHeader->imageSize/3, f);
}

bmp_file_t* bmp_create_24bpp(uint32_t xSize, uint32_t ySize)
{
    NOT_IMPL("BMP creation");
    assert(xSize&&ySize);
    bmp_file_t *ptr = (bmp_file_t*)malloc(sizeof(bmp_file_t));
    assert(ptr);
    bmp_pixel_t *pix = (bmp_pixel_t*)malloc(xSize*ySize);
    assert(pix);
    bmp_header_t *head = (bmp_header_t*)malloc(sizeof(bmp_header_t));
    assert(head);
    head->dataOffset = 54;
    head->fileSize = 54+xSize*ySize;
    head->reserved = 0;
    head->signature = BMP_SIGNATURE;
    bmp_infoheader_t *info = (bmp_infoheader_t*)malloc(sizeof(bmp_infoheader_t));
    assert(info);
    info->bpp = 24;
    info->colorsUsed = 16777216;
    info->compression = 0;
    info->height = ySize;
    info->width = xSize;
    info->imageSize = xSize*ySize;
    info->importantColors = 0;
    info->planes = 1;
    info->xPixelsPerM = 0;
    info->yPixelsPerM = 0;

    ptr->header = head;
    ptr->infoHeader = info;
    ptr->colorTable = NULL;
    ptr->pixels = pix;
    return ptr;
}
