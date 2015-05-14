#include "zpopcx.h"
#include "pcx.hpp"

extern "C" 
{
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
