#include <zpopcx.h>

int main(void)
{

    loadTrueColorPcx("sourceRGB.pcx", "outRGB.pcx");
    loadGrayScalePcx("sourceGS.pcx", "outGS.pcx");

    return 0;
}
