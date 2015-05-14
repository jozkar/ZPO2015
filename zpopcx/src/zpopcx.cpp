#include "zpopcx.h"
#include "pcx.hpp"

extern "C" 
{

	void loadGrayScalePcx(const char *filename, const char *out)
	{
		BMP bmp(GrayScale, 1, 1);
		printf("loading truecolor PCX %s to %s\n", filename, out);
		bmp.loadFromPCX((char *)filename);
		bmp.saveToGrayScalePCX((char *)out);
		printf("done file %s\n", out);
	}

	void loadTrueColorPcx(const char *filename, const char *out)
	{
		BMP bmp(TrueColor, 1, 1);
		printf("loading truecolor PCX %s to %s\n", filename, out);
		bmp.loadFromPCX((char *)filename);
		bmp.saveToTrueColorPCX((char *)out);
		printf("done file %s\n", out);
	}
}
