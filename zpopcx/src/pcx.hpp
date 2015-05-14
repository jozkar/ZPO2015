/*  Projekt do predmetu ZPO - Nacitani a ukladani obrazu
 *  Autori: Zdenek Biberle (xbiber00), Josef Ridky(xridky00), Zdenek Tretter (xtrett00)
 *  Popis: Predpisy funkci pro nacitani a ukladani obrazu ve formatu PCX (8 bitu - grayscale, 24 bitu - RGB)
 *  Datum: 9. 5. 2015
 */   

#ifndef _PCX_HPP_
#define _PCX_HPP_

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <sstream>

// navratove hodnoty a typy bitmap
enum{
    OK = 0,
    ERROR = 1,
    GrayScale = 2,
    TrueColor = 3
};

// PCX hlavicka
struct PCXHeader{
		// ID, Verze, RLE kodovani, Bitu na pixel
		unsigned char id;
		unsigned char version;
		unsigned char rle;
		unsigned char  bpp;     
		
		// Pocatecni souradnice, Koncove souradnice, horizontalni DPI, vertikalni DPI 
		unsigned short xBegin;
		unsigned short yBegin;
		unsigned short xEnd;
		unsigned short yEnd;
		unsigned short hDPI;
		unsigned short vDPI;
	
		// Barevna paleta, Rezervovany bajt, Pocet bitovych hladin
	    unsigned char palette[3 * 16];
	    unsigned char reservedByte;
	    unsigned char numberBitPlanes;
	    
	    // Bajtu na radek, Typ barevne palety
	    unsigned short bytesPerLine;
	    unsigned short paletteType;
	    unsigned short hSize;
	    unsigned short vSize; 
		//rezervovane bajty
		unsigned char reserved2[54];
};	

// struktura bitmapy
class BMP{
    int type;
    unsigned int  width, height, bitsPerPixel, bytesPerLine, pixelCount, 
		  byteCount, color, cpx, cpy;
    unsigned char *pixels, R, G, B;
    bool loaded;    
    PCXHeader header;
    
	public:
	// vytvoreni bitmapy
	BMP(int type, unsigned int width, unsigned int height);
	
	void createGrayScale(unsigned int width, unsigned int height);
	void createTrueColor(unsigned int width, unsigned int height);
	
	// zruseni bitmapy
	~BMP();
	
	// naplneni bitmapy
	void fillPixels();
	
	// gettery
	std::string getInfo();
	int getType();
	unsigned int getWidth();
	unsigned int getHeight();
	unsigned int getBytesPerLine();
	unsigned int getBitsPerPixel();
	unsigned int getPixelCount();
	unsigned int getByteCount();
	unsigned int getColor();
	unsigned int getRLE();
	int getPixel(int x, int y);
	int getPixelGrayScale(int x, int y);
	int getPixelRGB(int x, int y);
	
	// settery
	void setType(int type);
	void setBitsPerPixel(unsigned int bpp);
	void setWidth(unsigned int width);
	void setHeight(unsigned int height);
	void setBytesPerLine(unsigned int bpl);
	void setPixelCount(unsigned int count);
	void setByteCount(unsigned int count);
	void setColor(int color);
	void setColorRGB(unsigned char r, unsigned char g, unsigned char b);
	
	// "vykreslovaci" funkce
	void putPixel(int x, int y);
	void putPixelGrayScale(int x, int y, unsigned char color);
	void putPixelInt(int x, int y, int color);
	void putPixelRGB(int x, int y, unsigned char r, unsigned char g, unsigned char b);
	void moveTo(int x, int y);
	void lineTo(int x, int y);
	void line(int x1, int y1, int x2, int y2);
	void clear();
	void fill();
	
	// funkce pro praci s PCX
	int write(unsigned char byt, unsigned int coding, FILE *out);
	int saveToPCX(char *out);
	int saveToGrayScalePCX(char *out);
	int saveToTrueColorPCX(char *out);
	int loadFromPCX(char *in);
	
};

#endif
