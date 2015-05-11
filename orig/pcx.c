#include <stdio.h>
#include <stdlib.h>
#include "pcx_lib.h"

void createBWPcx(int width, int height, const char *filename)
{
    Bitmap *bmp;
    printf("creating black & white PCX %dx%d pixels\n", width, height);
    bmp=bitmapCreateTwoColors(width, height);
    if (bmp) {
        bitmapSetColorBit(bmp, 1);
        bitmapLine(bmp, 0, 0, width-1, height-1);
        bitmapLine(bmp, 0, height-1, width-1, 0);
        bitmapSave2TwoColorPCX(bmp, (char *)filename);
        bitmapDestroy(bmp);
        printf("done file %s\n", filename);
    }
    else {
        puts("creation failed!");
    }
}

void createGrayScalePcx(int width, int height, const char *filename)
{
    Bitmap *bmp;
    printf("creating grayscale PCX %dx%d pixels\n", width, height);
    bmp=bitmapCreateGrayScale(width, height);
    if (bmp) {
        bitmapSetColorInt(bmp, 0xff);
        bitmapLine(bmp, 0, 0, width-1, height-1);
        bitmapSetColorInt(bmp, 0x00);
        bitmapLine(bmp, 0, height-1, width-1, 0);
        bitmapSave2GrayScalePCX(bmp, (char *)filename);
        bitmapDestroy(bmp);
        printf("done file %s\n", filename);
    }
    else {
        puts("creation failed!");
    }
}

void createTrueColorPcx(int width, int height, const char *filename)
{
    Bitmap *bmp;
    printf("creating truecolor PCX %dx%d pixels\n", width, height);
    bmp=bitmapCreateTrueColor(width, height);
    if (bmp) {
        bitmapSetColorRGB(bmp, 0xff, 0x00, 0x00);
        bitmapLine(bmp, 0, 0, width-1, height-1);
        bitmapSetColorRGB(bmp, 0x00, 0x00, 0xff);
        bitmapLine(bmp, 0, height-1, width-1, 0);
        bitmapSave2TrueColorPCX(bmp, (char *)filename);
        bitmapDestroy(bmp);
        printf("done file %s\n", filename);
    }
    else {
        puts("creation failed!");
    }
}

int main(void)
{
    createBWPcx(126, 126, "bw126x126.pcx");
    createBWPcx(127, 127, "bw127x127.pcx");
    createBWPcx(128, 128, "bw128x128.pcx");
    createBWPcx(129, 129, "bw129x129.pcx");

    createGrayScalePcx(126, 126, "g126x126.pcx");
    createGrayScalePcx(127, 127, "g127x127.pcx");
    createGrayScalePcx(128, 128, "g128x128.pcx");
    createGrayScalePcx(129, 129, "g129x129.pcx");

    createTrueColorPcx(126, 126, "rgb126x126.pcx");
    createTrueColorPcx(127, 127, "rgb127x127.pcx");
    createTrueColorPcx(128, 128, "rgb128x128.pcx");
    createTrueColorPcx(129, 129, "rgb129x129.pcx");

    return 0;
}

