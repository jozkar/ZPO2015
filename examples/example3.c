#include <zpopcx.h>

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

    loadGrayScalePcx("g126x126.pcx", "out1.pcx");
    loadTrueColorPcx("rgb126x126.pcx", "out2.pcx");
    loadGrayScalePcx("g129x129.pcx", "out3.pcx");
    loadTrueColorPcx("rgb129x129.pcx", "out4.pcx");

    return 0;
}
