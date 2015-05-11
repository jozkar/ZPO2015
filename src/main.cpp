#include "pcx.hpp"


void createGrayScalePcx(int width, int height, const char *filename)
{
    BMP bmp(GrayScale, width, height);
    printf("creating grayscale PCX %dx%d pixels\n", width, height);
    bmp.setColor(0xff);
    bmp.line(0, 0, width-1, height-1);
    bmp.setColor(0x00);
    bmp.line(0, height-1, width-1, 0);
    bmp.saveToGrayScalePCX((char *)filename);
    printf("done file %s\n", filename);
}

void createTrueColorPcx(int width, int height, const char *filename)
{
    BMP bmp(TrueColor, width, height);
    printf("creating truecolor PCX %dx%d pixels\n", width, height);
    bmp.setColorRGB(0xff, 0x00, 0x00);
    bmp.line(0, 0, width-1, height-1);
    bmp.setColorRGB(0x00, 0x00, 0xff);
    bmp.line(0, height-1, width-1, 0);
    bmp.saveToTrueColorPCX((char *)filename);
    printf("done file %s\n", filename);
}

void loadTrueColorPcx(const char *filename, const char *out)
{
    BMP bmp(GrayScale, 1, 1);
    printf("loading truecolor PCX %s to %s\n", filename, out);
    bmp.loadFromPCX((char *)filename);
    bmp.saveToGrayScalePCX((char *)out);
    printf("done file %s\n", out);
}

int main(void)
{
    createGrayScalePcx(126, 126, "g126x126.pcx");
    createGrayScalePcx(127, 127, "g127x127.pcx");
    createGrayScalePcx(128, 128, "g128x128.pcx");
    createGrayScalePcx(129, 129, "g129x129.pcx");

    createTrueColorPcx(126, 126, "rgb126x126.pcx");
    createTrueColorPcx(127, 127, "rgb127x127.pcx");
    createTrueColorPcx(128, 128, "rgb128x128.pcx");
    createTrueColorPcx(129, 129, "rgb129x129.pcx");

    loadTrueColorPcx("g126x126.pcx", "out.pcx");

    return 0;
}
