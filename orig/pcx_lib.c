// ----------------------------------------------------------------------------
// application: PCX bitmaps manipulation library
// author:      Pavel Tisnovsky
// file name:   pcx_lib.c
// description: base bitmaps manipulation library, common bitmap functions,
//              functions for reading and writing bitmaps to/from PCX format
// ----------------------------------------------------------------------------



#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pcx_lib.h"

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif



// bitmapCreate() {{{



//-----------------------------------------------------------------------------
// This function creates new bitmap of any type (two colors, grayscale or true
// colors)
//-----------------------------------------------------------------------------
Bitmap * bitmapCreate(int type, unsigned int width, unsigned int height)
{
    Bitmap *bmp;

    if (width<=0) return NULL;                       // check bitmap resolution
    if (height<=0) return NULL;
    switch (type) {
        case BitmapTypeTwoColors:                   // create black and white bitmap
            bmp=bitmapCreateTwoColors(width, height);
            break;
        case BitmapTypeGrayScale:                   // create grayscale pixmap
            bmp=bitmapCreateGrayScale(width, height);
            break;
        case BitmapTypeTrueColor:                   // create true color pixmap
            bmp=bitmapCreateTrueColor(width, height);
            break;
        default:                                    // unknown bitmap/pixmap type
            bmp=NULL;
            break;
    };
    return bmp;                                     // return pointer to new bitmap/pixmap
}



// }}}
// bitmapCreateTwoColors() {{{



//-----------------------------------------------------------------------------
// This function creates new two colors (black and white) bitmap and allocate
// memory for pixel's data
//-----------------------------------------------------------------------------
Bitmap * bitmapCreateTwoColors(unsigned int width, unsigned int height)
{
    Bitmap *bmp;
    unsigned int size;

    if (width<=0) return NULL;                       // check bitmap resolution
    if (height<=0) return NULL;
    bmp=(Bitmap*)malloc(sizeof(Bitmap));
    if (!bmp) return NULL;                           // check malloc
    bmp->type=BitmapTypeTwoColors;                  // fill in information
    bmp->bpp=1;
    bmp->width=width;
    bmp->height=height;
    bmp->bytesPerLine=((width+0x07) & 0xfffffff8)>>3;
    size=bmp->bytesPerLine*height;
    bmp->totalPixels=width*height;
    bmp->totalBytes=size;
    bmp->pixels=(unsigned char *)malloc(sizeof(unsigned char)*size);

    if (!bmp->pixels) {                             // check malloc
        free(bmp);
        return NULL;
    }
    bitmapFillPixels(bmp);                          // create initial pattern
    return bmp;
}



// }}}
// bitmapCreateGrayScale() {{{



//-----------------------------------------------------------------------------
// This function creates new gray scale bitmap and allocate memory for pixel's
// data
//-----------------------------------------------------------------------------
Bitmap * bitmapCreateGrayScale(unsigned int width, unsigned int height)
{
    Bitmap *bmp;
    unsigned int size;

    if (width<=0) return NULL;                       // check bitmap resolution
    if (height<=0) return NULL;
    bmp=(Bitmap*)malloc(sizeof(Bitmap));
    if (!bmp) return NULL;
    bmp->type=BitmapTypeGrayScale;                  // fill in information
    bmp->bpp=8;
    bmp->width=width;
    bmp->height=height;
    bmp->bytesPerLine=width;
    size=bmp->bytesPerLine*height;
    bmp->totalPixels=width*height;
    bmp->totalBytes=size;
    bmp->pixels=(unsigned char *)malloc(sizeof(unsigned char)*size);

    if (!bmp->pixels) {                             // check malloc
        free(bmp);
        return NULL;
    }
    bitmapFillPixels(bmp);                          // create initial pattern
    return bmp;
}



// }}}
// bitmapCreateTrueColor() {{{



//-----------------------------------------------------------------------------
// This function creates new true color bitmap and allocate memory for pixel's
// data
//-----------------------------------------------------------------------------
Bitmap * bitmapCreateTrueColor(unsigned int width, unsigned int height)
{
    Bitmap *bmp;
    unsigned int size;

    if (width<=0) return NULL;                       // check bitmap resolution
    if (height<=0) return NULL;
    bmp=(Bitmap*)malloc(sizeof(Bitmap));
    if (!bmp) return NULL;
    bmp->type=BitmapTypeTrueColor;                  // fill in information
    bmp->bpp=24;
    bmp->width=width;
    bmp->height=height;
    bmp->bytesPerLine=width*3;
    size=bmp->bytesPerLine*height;
    bmp->totalPixels=width*height;
    bmp->totalBytes=size;
    bmp->pixels=(unsigned char *)malloc(sizeof(unsigned char)*size);

    if (!bmp->pixels) {                             // check malloc
        free(bmp);
        return NULL;
    }
    bitmapFillPixels(bmp);                          // create initial pattern
    return bmp;
}



// }}}
// bitmapDestroy() {{{



//-----------------------------------------------------------------------------
// This function destroys bitmap and frees memory
//-----------------------------------------------------------------------------
int bitmapDestroy(Bitmap *bitmap)
{
    if (!bitmap) return false;
    free(bitmap->pixels);                           // free pixel' memory
    free(bitmap);
#ifndef __BORLANDC__
    bitmap=NULL;
#endif
    return true;
}



// }}}

// bitmapFillPixels() {{{



//-----------------------------------------------------------------------------
// This function fills whole bitmap with a color pattern
//-----------------------------------------------------------------------------
void bitmapFillPixels(Bitmap *bitmap)
{
    unsigned int    x,y;
    unsigned int    count;
    unsigned char  *p=bitmap->pixels;
    unsigned char  *porig=p;

    switch (bitmap->bpp) {
        case 1:                                     // black and white bitmap
            count=bitmap->bytesPerLine*bitmap->height;
            for (y=0; y<count; y++) {
                *p++=0x00;
            }
            break;
        case 8:                                     // grayscale bitmap
            for (y=0; y<bitmap->height; y++) {
                for (x=0; x<bitmap->width; x++) {
                    *p++=(unsigned char)(x+y);
                }
                porig+=bitmap->bytesPerLine;
                p=porig;
            }
            break;
        case 24:                                    // truecolor bitmap
            for (y=0; y<bitmap->height; y++) {
                for (x=0; x<bitmap->width; x++) {
                    *p++=(unsigned char)x;
                    *p++=(unsigned char)y;
                    *p++=(unsigned char)(x+y);
                }
                porig+=bitmap->bytesPerLine;
                p=porig;
            }
            break;
        default:
            break;
    }
}



// }}}

// bitmapGetInfo() {{{



//-----------------------------------------------------------------------------
// This function prints informations about bitmap to string
//-----------------------------------------------------------------------------
int bitmapGetInfo(Bitmap *bitmap, char *string, int strlen)
{
    //_snprintf(string, strlen, "width: %d   height: %d  bpp: %d", bitmap->width, bitmap->height, bitmap->bpp);
    return true;
}



// }}}
// bitmapGetWidth() {{{



//-----------------------------------------------------------------------------
// This function returns bitmap width
//-----------------------------------------------------------------------------
int bitmapGetWidth(Bitmap *bitmap)
{
    if (!bitmap) return 0;
    return bitmap->width;
}



// }}}
// bitmapGetHeight() {{{



//-----------------------------------------------------------------------------
// This function returns bitmap height
//-----------------------------------------------------------------------------
int bitmapGetHeight(Bitmap *bitmap)
{
    if (!bitmap) return 0;
    return bitmap->height;
}



// }}}
// bitmapGetSize() {{{



//-----------------------------------------------------------------------------
// This function returns bitmap size (width x height)
//-----------------------------------------------------------------------------
int bitmapGetSize(Bitmap *bitmap, int *width, int *height)
{
    if (!bitmap) return 0;
    *width=bitmap->width;
    *height=bitmap->height;
    return 1;
}



// }}}

// bitmapSetColorBit() {{{



//-----------------------------------------------------------------------------
// This function sets the current black or white color
//-----------------------------------------------------------------------------
int bitmapSetColorBit(Bitmap *bitmap, int color)
{
    if (!bitmap) return false;
    bitmap->currentColorBW=color;
    return true;
}



// }}}
// bitmapSetColorInt() {{{



//-----------------------------------------------------------------------------
// This function sets the current color
//-----------------------------------------------------------------------------
int bitmapSetColorInt(Bitmap *bitmap, int color)
{
    if (!bitmap) return false;
    if (color>0x00ffffff) return false;
    bitmap->currentColor=color;
    bitmap->currentColorR=(unsigned char)(color & 0x000000ff);
    bitmap->currentColorG=(unsigned char)((color & 0x0000ff00) >> 8);
    bitmap->currentColorB=(unsigned char)((color & 0x00ff0000) >>16);
    return true;
}



// }}}
// bitmapSetColorRGB() {{{



//-----------------------------------------------------------------------------
// This function sets the current RGB color
//-----------------------------------------------------------------------------
int bitmapSetColorRGB(Bitmap *bitmap, unsigned char r, unsigned char g, unsigned char b)
{
    if (!bitmap) return false;
    bitmap->currentColor=(r)|(g<<8)|(b<<16);
    bitmap->currentColorR=r;
    bitmap->currentColorG=g;
    bitmap->currentColorB=b;
    return true;
}



// }}}

// bitmapGetPixel() {{{



//-----------------------------------------------------------------------------
// This function returns pixel value from position [x,y]
//-----------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma option -w-rch
#endif
int bitmapGetPixel(Bitmap *bitmap, int x, int y)
{
    if (!bitmap) return 0;
    if (x<0 || x>=(int)bitmap->width) return 0;
    if (y<0 || y>=(int)bitmap->height) return 0;
    switch (bitmap->type) {
        case BitmapTypeTwoColors:
            return bitmapGetPixelBit(bitmap, x, y);
            break;
        case BitmapTypeGrayScale:
            return bitmapGetPixelGrayScale(bitmap, x, y);
            break;
        case BitmapTypeTrueColor:
            return bitmapGetPixelInt(bitmap, x, y);
            break;
        default:
            return 0;
            break;
    }
    return 0;
}
#ifdef __BORLANDC__
#pragma option -w+rch
#endif



// }}}

// bitmapPutPixel() {{{



//-----------------------------------------------------------------------------
// This function puts pixel at position [x,y]
//-----------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma option -w-rch
#endif
int bitmapPutPixel(Bitmap *bitmap, int x, int y)
{
    if (!bitmap) return false;
    if (x<0 || x>=(int)bitmap->width) return false;
    if (y<0 || y>=(int)bitmap->height) return false;
    switch (bitmap->type) {
        case BitmapTypeTwoColors:
            return bitmapPutPixelBit(bitmap, x, y, bitmap->currentColorBW);
            break;
        case BitmapTypeGrayScale:
            return bitmapPutPixelGrayScale(bitmap, x, y, (unsigned char)bitmap->currentColor);
            break;
        case BitmapTypeTrueColor:
            return bitmapPutPixelInt(bitmap, x, y, bitmap->currentColor);
            break;
        default:
            return false;
            break;
    }
    return false;
}
#ifdef __BORLANDC__
#pragma option -w+rch
#endif



// }}}
// bitmapPutPixelBit() {{{



//-----------------------------------------------------------------------------
// This function sets pixel at position [x,y] in black and white bitmap
//-----------------------------------------------------------------------------
int bitmapPutPixelBit(Bitmap *bitmap, int x, int y, int color)
{
    unsigned char *p;
    unsigned int maska;
    if (!bitmap) return false;
    if (x<0 || x>=(int)bitmap->width) return false;
    if (y<0 || y>=(int)bitmap->height) return false;
    if (bitmap->type==BitmapTypeTwoColors) {
        p=bitmap->pixels+(x>>3)+y*bitmap->bytesPerLine;
        maska=128 >> (x%8);
        if (color) *p|=maska;
        else       *p&=~maska;
        return true;
    }
    else {
        return false;
    }
}



// }}}
// bitmapPutPixelGrayScale() {{{



//-----------------------------------------------------------------------------
// This function sets pixel at position [x,y] to the specified color
//-----------------------------------------------------------------------------
int bitmapPutPixelGrayScale(Bitmap *bitmap, int x, int y, unsigned char color)
{
    unsigned char *p;
    if (!bitmap) return false;
    if (bitmap->type==BitmapTypeGrayScale) {
        p=bitmap->pixels+x+y*bitmap->bytesPerLine;
        *p=color;
        return true;
    }
    else {
        return false;
    }
}



// }}}
// bitmapPutPixelInt() {{{



//-----------------------------------------------------------------------------
// This function puts pixel at position [x,y] to RGB color
//-----------------------------------------------------------------------------
int bitmapPutPixelInt(Bitmap *bitmap, int x, int y, int color)
{
    unsigned char *p;

    if (!bitmap) return false;
    if (bitmap->type==BitmapTypeTrueColor) {
        p=bitmap->pixels+3*x+y*bitmap->bytesPerLine;
        *p=(unsigned char)(color & 0x000000ff); p++;
        *p=(unsigned char)((color & 0x0000ff00) >> 8); p++;
        *p=(unsigned char)((color & 0x00ff0000) >>16);
        return true;
    }
    else {
        return false;
    }
}



// }}}
// bitmapPutPixelRGB() {{{



//-----------------------------------------------------------------------------
// This function puts pixel at position [x,y] to RGB color
//-----------------------------------------------------------------------------
int bitmapPutPixelRGB(Bitmap *bitmap, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char *p;

    if (!bitmap) return false;
    if (bitmap->type==BitmapTypeTrueColor) {
        p=bitmap->pixels+3*x+y*bitmap->bytesPerLine;
        *p++=r;
        *p++=g;
        *p=b;
        return true;
    }
    else {
        return false;
    }
}



// }}}
// bitmapGetPixelBit() {{{



//-----------------------------------------------------------------------------
// This function gets pixel value from specified screen position, works for
// two-color bitmaps
//-----------------------------------------------------------------------------
int bitmapGetPixelBit(Bitmap *bitmap, int x, int y)
{
    unsigned char *p;
    unsigned int maska;
    if (!bitmap) return false;
    if (bitmap->type==BitmapTypeTwoColors) {
        p=bitmap->pixels+(x>>3)+y*bitmap->bytesPerLine;
        maska=128 >> (x%8);
        return (maska & *p) ? 1: 0;
    }
    else {
        return 0;
    }
}



// }}}
// bitmapGetPixelGrayScale() {{{



//-----------------------------------------------------------------------------
// This function gets pixel value from specified screen position, works for
// grayscale bitmaps
//-----------------------------------------------------------------------------
int bitmapGetPixelGrayScale(Bitmap *bitmap, int x, int y)
{
    unsigned char *p;
    if (!bitmap) return false;
    if (bitmap->type==BitmapTypeGrayScale) {
        p=bitmap->pixels+x+y*bitmap->bytesPerLine;
        return *p;
    }
    else {
        return 0;
    }
}



// }}}
// bitmapGetPixelInt() {{{



//-----------------------------------------------------------------------------
// This function gets pixel value from specified screen position, works for
// true-color bitmaps
//-----------------------------------------------------------------------------
int bitmapGetPixelInt(Bitmap *bitmap, int x, int y)
{
    unsigned char *p;
    int result;
    if (!bitmap) return false;
    if (bitmap->type==BitmapTypeTrueColor) {
        p=bitmap->pixels+3*x+y*bitmap->bytesPerLine;
        result=*p++;
        result|=(*p++)<<8;
        result|=(*p)<<16;
        return result;
    }
    else {
        return 0;
    }
}



// }}}

// bitmapMoveTo() {{{



//-----------------------------------------------------------------------------
// This function moves cursor pointer to position [x,y]
//-----------------------------------------------------------------------------
int bitmapMoveTo(Bitmap *bitmap, int x, int y)
{
    if (!bitmap) return false;
    if (x>(int)bitmap->width) return false;
    if (y>(int)bitmap->height) return false;
    bitmap->cpx=x;
    bitmap->cpy=y;
    return true;
}



// }}}
// bitmapLineTo() {{{



//-----------------------------------------------------------------------------
// This function draws line from old cursor pointer to new cursor pointer
// position [x,y]
//-----------------------------------------------------------------------------
int bitmapLineTo(Bitmap *bitmap, int x, int y)
{
    unsigned char *p;
    int i, deltax, deltay, numpixels;
    int d, dinc1, dinc2, xinc1, xinc2, yinc1, yinc2;
    int px=bitmap->cpx, py=bitmap->cpy;

    if (!bitmap) return false;
    if (x>=(int)bitmap->width) return false;
    if (y>=(int)bitmap->height) return false;

    switch (bitmap->type) {
        case BitmapTypeTwoColors:
            deltax=abs(x-bitmap->cpx);
            deltay=abs(y-bitmap->cpy);
            if (deltax>=deltay) {                   // angle is less then +-45 degrees
                numpixels=deltax;
                d=(deltay<<1)-deltax;
                dinc1=deltay<<1;
                dinc2=(deltay-deltax)<<1;
                xinc1=xinc2=1;
                yinc1=0;
                yinc2=1;
            }
            else {                                  // angle is greater then +-45 degrees
                numpixels=deltay;
                d=(deltax<<1)-deltay;
                dinc1=deltax<<1;
                dinc2=(deltax-deltay)<<1;
                xinc1=0;
                xinc2=1;
                yinc1=yinc2=1;
            }
            if ((int)bitmap->cpx>x) {               // reversed x-coordinates
                xinc1=-xinc1;
                xinc2=-xinc2;
            }
            if ((int)bitmap->cpy>y) {               // reversed y-coordinates
                yinc1=-yinc1;
                yinc2=-yinc2;
            }
            for (i=0; i<=numpixels; i++) {
                bitmapPutPixelBit(bitmap, px, py, bitmap->currentColorBW);
                if (d<0) {
                    d+=dinc1;
                    px+=xinc1;
                    py+=yinc1;
                }
                else {
                    d+=dinc2;
                    px+=xinc2;
                    py+=yinc2;
                }
            }
            break;

        case BitmapTypeGrayScale:
            p=bitmap->pixels+bitmap->cpx+bitmap->cpy*bitmap->width;
            deltax=abs(x-bitmap->cpx);
            deltay=abs(y-bitmap->cpy);
            if (deltax>=deltay) {                   // angle is less then +-45 degrees
                numpixels=deltax;
                d=(deltay<<1)-deltax;
                dinc1=deltay<<1;
                dinc2=(deltay-deltax)<<1;
                xinc1=xinc2=1;
                yinc1=0;
                yinc2=bitmap->width;
            }
            else {
                numpixels=deltay;
                d=(deltax<<1)-deltay;
                dinc1=deltax<<1;
                dinc2=(deltax-deltay)<<1;
                xinc1=0;
                yinc1=yinc2=bitmap->width;
                xinc2=1;
            }
            if ((int)bitmap->cpx>x) {               // reversed x-coordinates
                xinc1=-xinc1;
                xinc2=-xinc2;
            }
            if ((int)bitmap->cpy>y) {               // reversed y-coordinates
                yinc1=-yinc1;
                yinc2=-yinc2;
            }
            for (i=0; i<=numpixels; i++) {
                *p=(unsigned char)bitmap->currentColor;
                if (d<0) {
                    d+=dinc1;
                    p+=xinc1+yinc1;
                }
                else {
                    d+=dinc2;
                    p+=xinc2+yinc2;
                }
            }
            break;

        case BitmapTypeTrueColor:
            p=bitmap->pixels+3*(bitmap->cpx+bitmap->cpy*bitmap->width);
            deltax=abs(x-bitmap->cpx);
            deltay=abs(y-bitmap->cpy);
            if (deltax>=deltay) {                   // angle is less then +-45 degrees
                numpixels=deltax;
                d=(deltay<<1)-deltax;
                dinc1=deltay<<1;
                dinc2=(deltay-deltax)<<1;
                xinc1=xinc2=3;
                yinc1=0;
                yinc2=3*bitmap->width;
            }
            else {                                   // angle is greater then +-45 degrees
                numpixels=deltay;
                d=(deltax<<1)-deltay;
                dinc1=deltax<<1;
                dinc2=(deltax-deltay)<<1;
                xinc1=0;
                yinc1=yinc2=3*bitmap->width;
                xinc2=3;
            }
            if ((int)bitmap->cpx>x) {               // reversed x-coordinates
                xinc1=-xinc1;
                xinc2=-xinc2;
            }
            if ((int)bitmap->cpy>y) {               // reversed y-coordinates
                yinc1=-yinc1;
                yinc2=-yinc2;
            }
            for (i=0; i<=numpixels; i++) {
                *p=(unsigned char)bitmap->currentColorR;
                *(p+1)=(unsigned char)bitmap->currentColorG;
                *(p+2)=(unsigned char)bitmap->currentColorB;
                if (d<0) {
                    d+=dinc1;
                    p+=xinc1+yinc1;
                }
                else {
                    d+=dinc2;
                    p+=xinc2+yinc2;
                }
            }
            break;

        default:
            break;
    }
    bitmap->cpx=x;
    bitmap->cpy=y;
    return true;
}



// }}}
// bitmapLine() {{{



//-----------------------------------------------------------------------------
// This function draws line from [x0, y0] to new cursor position [x1,y1]
//-----------------------------------------------------------------------------
int bitmapLine(Bitmap *bitmap, int x1, int y1, int x2, int y2)
{
    bitmapMoveTo(bitmap, x1, y1);
    bitmapLineTo(bitmap, x2, y2);
    return true;
}



// }}}
// bitmapClear() {{{



//-----------------------------------------------------------------------------
// This function clears entire bitmap
//-----------------------------------------------------------------------------
int bitmapClear(Bitmap *bitmap)
{
    if (!bitmap) return false;
    memset(bitmap->pixels, 0, bitmap->totalBytes);
    return true;
}



// }}}
// bitmapFill() {{{



//-----------------------------------------------------------------------------
// This function fills entire bitmap
//-----------------------------------------------------------------------------
int bitmapFill(Bitmap *bitmap)
{
    int i, j;
    unsigned char *p;

    if (!bitmap) return false;
    switch (bitmap->type) {
        case BitmapTypeTwoColors:
            memset(bitmap->pixels, (bitmap->currentColorBW) ? 0xff:0x00, bitmap->totalBytes);
            break;
        case BitmapTypeGrayScale:
            memset(bitmap->pixels, bitmap->currentColor, bitmap->totalBytes);
            break;
        case BitmapTypeTrueColor:
            p=bitmap->pixels;
            for (j=0; j<(int)bitmap->height; j++) {
                for (i=0; i<(int)bitmap->width; i++) {
                    *p++=bitmap->currentColorR;
                    *p++=bitmap->currentColorG;
                    *p++=bitmap->currentColorB;
                }
            }
            break;
        default:
            break;
    }
    return true;
}



// }}}

// pcxEncPut() {{{



//-----------------------------------------------------------------------------
// Subroutine for writing an encoded byte pair (or single byte if it doesn't
// encode) to a file. It returns the count of bytes written, 0 if error.
//-----------------------------------------------------------------------------
int pcxEncPut(unsigned char byt, unsigned int cnt, FILE *file)
{
    if (cnt) {
        if ((cnt == 1) && (0xc0 != (0xc0 & byt))) {
            if (putc((int)byt, file)==EOF)          // disk write error (probably disk full)
                return 0;                           // write failed
            return 1;                               // write ok
        }
        else {
            if (putc((int)0xC0 | cnt, file)==EOF)   // disk write error
                return 0;                           // write failed
            if (putc((int)byt, file)==EOF)          // disk write error
                return 0;                           // write failed
            return 2;                               // write ok
        }
    }
    return 0;                                       // write failed
}



// }}}
// bitmapSave2PCX() {{{



//-----------------------------------------------------------------------------
// This function saves raster data to fileformat PCX (ZSoft PixMap)
//-----------------------------------------------------------------------------
int bitmapSave2PCX(Bitmap *bitmap, char *filename)
{
    int result;
    assert(bitmap);                                 // assert that bitmap is not NULL
    assert(filename);                               // assert that filename is not NULL
    if (!bitmap) return false;                      // bitmap does not exists
    if (!filename) return false;                    // empty filename
    switch (bitmap->type) {                         // which type is bitmap?
        case BitmapTypeTwoColors:                   // black and white bitmap
            result=bitmapSave2TwoColorPCX(bitmap, filename); // try to save bitmap to PCX file
            break;
        case BitmapTypeGrayScale:                   // grayscale bitmap/pixmap
            result=bitmapSave2GrayScalePCX(bitmap, filename); // try to save bitmap to PCX file
            break;
        case BitmapTypeTrueColor:                   // true color bitmap/pixmap
            result=bitmapSave2TrueColorPCX(bitmap, filename); // try to save bitmap to PCX file
            break;
        default:                                    // unknown bitmap type
            result=false;                           // function failed
            break;
    }
    return result;                                  // function returns
}



// }}}
// bitmapSave2TwoColorPCX() {{{



//-----------------------------------------------------------------------------
// This function saves raster data in black and white only pixel-format to
// fileformat PCX
//-----------------------------------------------------------------------------
int bitmapSave2TwoColorPCX(Bitmap *bitmap, char *filename)
{
    FILE  *fout;                                    // output file
    int    size;
    int    i, j;                                    // loop counters
    int    cnt;                                     // character count
    int    result;
    unsigned char *p;
    typedef struct tagPCXHeader {                   // pcx header
        unsigned char   id;                         // file id
        unsigned char   version;                    // PCX version
        unsigned char   rle;                        // compression
        unsigned char   bpp;                        // bits per pixel
        unsigned short  xstart;                     // starting coordinates
        unsigned short  ystart;
        unsigned short  xend;                       // ending coordinates
        unsigned short  yend;
        unsigned short  hres;                       // horizontal resolution
        unsigned short  vres;                       // vertical resolution
        unsigned char   pal[48];                    // palette (up to 16 colors)
        unsigned char   reserved;                   // reserved byte
        unsigned char   nbitp;                      // number of bitplanes
        unsigned short  bytesPerLine;               // bytes per line
        unsigned short  palType;                    // palette type
        unsigned short  horizontalSize;             // horizontal size
        unsigned short  verticalSize;               // vertical size
        unsigned char   reserved2[54];              // reserved bytes
    } PCXHeader;
    PCXHeader pcxHeader;

    assert(bitmap);                                 // check bitmap structure
    assert(filename);                               // check filename
    assert(bitmap->pixels);                         // check pixels
    assert(bitmap->type==BitmapTypeTwoColors);      // check bitmap type
    if (!bitmap) return false;                      // bitmap does not exists
    if (!bitmap->pixels) return false;              // pixel array does not exists
    if (!filename) return false;                    // empty filename
    if (bitmap->type!=BitmapTypeTwoColors) return false; // invalid bitmap type

    size=bitmap->bytesPerLine;                      // compute scanline size
    
    // fill in PCX header
    memset(&pcxHeader, 0, sizeof(pcxHeader));       // clear header
    pcxHeader.id=10;                                // manufacturer
    pcxHeader.version=5;                            // Paintbrush version 3.0 and >
    pcxHeader.rle=1;                                // PCX run length encoding
    pcxHeader.bpp=1;                                // 1 bit per pixel
    pcxHeader.xstart=0;                             // window coordinates
    pcxHeader.ystart=0;
    pcxHeader.xend=bitmap->width-1;
    pcxHeader.yend=bitmap->height-1;
    pcxHeader.hres=300;                             // horizontal resolution
    pcxHeader.vres=300;                             // vertical resolution
    for (i=0; i<3; i++)                             // set first color
        pcxHeader.pal[i]=0x00;
    for (i=3; i<6; i++)                             // set second color
        pcxHeader.pal[i]=0xff;
    pcxHeader.reserved=0;                           // should be set to zero
    pcxHeader.nbitp=1;                              // one bitplane
    if (bitmap->bytesPerLine % 1)
        pcxHeader.bytesPerLine=bitmap->bytesPerLine+1; // must be even number
    else
        pcxHeader.bytesPerLine=bitmap->bytesPerLine; // must be even number
    pcxHeader.palType=1;                            // palette type
    pcxHeader.horizontalSize=0;                     // horizontal screen size - new field
    pcxHeader.verticalSize=0;                       // vertical screen size - new field

    fout=fopen(filename,"wb");                      // open output file for writing
    assert(fout);
    if (!fout) return false;                        // when file could not be opened
    cnt=fwrite(&pcxHeader, sizeof(pcxHeader), 1, fout); // try to write file header
    assert(cnt==1);
    if (cnt!=1) {                                   // check file header
        fclose(fout);
        return false;                               // function failed
    }
    for (i=(bitmap->height-1); i>=0; i--) {         // write bitmap data
        unsigned char this;
        unsigned char last;
        int runCount=0;
        p=bitmap->pixels+i*size;                    // pointer to active scanline
        last=~(*p);
        for (j=0; j<(signed int)bitmap->bytesPerLine; j++) { // for all scanlines
            this=*p++;
            if (this==last) {                       // there is a "run" in the data, encode it
                runCount++;
                if (runCount==63) {                 // maximum run length
                    if (!(pcxEncPut(last, runCount, fout))) { // write two bytes
                        result=fclose(fout);        // try to close file
                        assert(result!=EOF);
                        return false;               // function failed
                    }
                    runCount=0;
                }
            }
            else {                                  // no "run"
                if (runCount) {
                    if (!(pcxEncPut(last, runCount, fout))) { // write one or two bytes
                        result=fclose(fout);        // try to close file
                        assert(result!=EOF);
                        return false;               // function failed
                    }
                }
                last=this;
                runCount=1;
            }
        }
        if (runCount) {                             // finish up scanline
            if (!(pcxEncPut(last, runCount, fout))) {
                result=fclose(fout);                // try to close file
                assert(result!=EOF);
                return false;                       // function failed
            }
        }
    }
    result=fclose(fout);                            // try to close file
    assert(result!=EOF);
    if (result==EOF) {                              // when file close failed
        return false;                               // function failed
    }
    return true;                                    // function succesed
}



// }}}
// bitmapSave2GrayScalePCX() {{{



//-----------------------------------------------------------------------------
// This function saves raster data in grayscale pixel-format to fileformat PCX
//-----------------------------------------------------------------------------
int bitmapSave2GrayScalePCX(Bitmap *bitmap, char *filename)
{
    FILE  *fout;                                    // output file
    int    size;
    int    i, j;                                    // loop counters
    int    cnt;                                     // character count
    int    result;
    unsigned char *p;
    typedef struct tagPCXHeader {                   // pcx header
        unsigned char   id;                         // file id
        unsigned char   version;                    // PCX version
        unsigned char   rle;                        // compression
        unsigned char   bpp;                        // bits per pixel
        unsigned short  xstart;                     // starting coordinates
        unsigned short  ystart;
        unsigned short  xend;                       // ending coordinates
        unsigned short  yend;
        unsigned short  hres;                       // horizontal resolution
        unsigned short  vres;                       // vertical resolution
        unsigned char   pal[48];                    // palette (up to 16 colors)
        unsigned char   reserved;                   // reserved byte
        unsigned char   nbitp;                      // number of bitplanes
        unsigned short  bytesPerLine;               // bytes per line
        unsigned short  palType;                    // palette type
        unsigned short  horizontalSize;             // horizontal size
        unsigned short  verticalSize;               // vertical size
        unsigned char   reserved2[54];              // reserved bytes
    } PCXHeader;
    PCXHeader pcxHeader;

    assert(bitmap);                                 // check bitmap structure
    assert(filename);                               // check filename
    assert(bitmap->pixels);                         // check pixels
    assert(bitmap->type==BitmapTypeGrayScale);      // check bitmap type
    if (!bitmap) return false;                      // bitmap does not exists
    if (!bitmap->pixels) return false;              // pixel array does not exists
    if (!filename) return false;                    // empty filename
    if (bitmap->type!=BitmapTypeGrayScale) return false; // invalid bitmap type

    size=bitmap->bytesPerLine;                      // compute scanline size
    
    // fill in PCX header
    memset(&pcxHeader, 0, sizeof(pcxHeader));       // clear header
    pcxHeader.id=10;                                // manufacturer
    pcxHeader.version=5;                            // Paintbrush version 3.0 and >
    pcxHeader.rle=1;                                // PCX run length encoding
    pcxHeader.bpp=8;                                // 1 bit per pixel
    pcxHeader.xstart=0;                             // window coordinates
    pcxHeader.ystart=0;
    pcxHeader.xend=bitmap->width-1;
    pcxHeader.yend=bitmap->height-1;
    pcxHeader.hres=300;                             // horizontal resolution
    pcxHeader.vres=300;                             // vertical resolution
    pcxHeader.reserved=0;                           // should be set to zero
    pcxHeader.nbitp=1;                              // one bitplane
    if (bitmap->width & 0x01)
        pcxHeader.bytesPerLine=(bitmap->width)+1;   // must be even number
    else
        pcxHeader.bytesPerLine=(bitmap->width);     // must be even number
    pcxHeader.palType=1;                            // palette type
    pcxHeader.horizontalSize=0;                     // horizontal screen size - new field
    pcxHeader.verticalSize=0;                       // vertical screen size - new field

    fout=fopen(filename,"wb");                      // open output file for writing
    assert(fout);
    if (!fout) return false;                        // when file could not be opened
    cnt=fwrite(&pcxHeader, sizeof(pcxHeader), 1, fout); // try to write file header
    assert(cnt==1);
    if (cnt!=1) {                                   // check file header
        fclose(fout);
        return false;                               // function failed
    }
    for (i=(bitmap->height-1); i>=0; i--) {         // write bitmap data
        unsigned char this;
        unsigned char last;
        int runCount=0;
        p=bitmap->pixels+i*size;
        last=~(*p);
        for (j=0; j<(signed int)(bitmap->width); j++) { // for all scanlines
            this=*p++;
            if (this==last) {                       // there is a "run" in the data, encode it
                runCount++;
                if (runCount==63) {                 // maximum run length
                    if (!(pcxEncPut(last, runCount, fout))) {
                        fclose(fout);               // try to close file
                        return false;               // function failed
                    }
                    runCount=0;
                }
            }
            else {                                  // no "run"
                if (runCount) {
                    if (!(pcxEncPut(last, runCount, fout))) { // write one or two bytes
                        fclose(fout);               // try to close file
                        return false;               // function failed
                    }
                }
                last=this;
                runCount=1;
            }
        }
        if (runCount) {                             // finish up scanline
            if (!(pcxEncPut(last, runCount, fout))) {
                fclose(fout);                       // try to close file
                return false;                       // function failed
            }
        }
        if (bitmap->bytesPerLine & 0x01) fputc(0x00, fout); // even bytes at scanline
    }
    if (fputc(0x0c, fout)==EOF) {                   // palette magic number
        result=fclose(fout);
        assert(result!=EOF);
        return false;
    }
    for (i=0; i<256; i++) {                         // write palette
        result=fputc(i, fout);                      // red color component
        assert(result!=EOF);                        // check write status
        result=fputc(i, fout);                      // green color component
        assert(result!=EOF);                        // check write status
        result=fputc(i, fout);                      // blue color component
        assert(result!=EOF);                        // check write status
    }
    result=fclose(fout);                            // try to close file
    assert(result!=EOF);
    if (result==EOF) {                              // when file close failed
        return false;                               // function failed
    }
    return true;                                    // function succesed
}



// }}}
// bitmapSave2TrueColorPCX() {{{



//-----------------------------------------------------------------------------
// This function saves raster data in true color pixel-format to fileformat PCX
//-----------------------------------------------------------------------------
int bitmapSave2TrueColorPCX(Bitmap *bitmap, char *filename)
{
    FILE  *fout;                                    // output file
    int    size;
    int    i, j, k;                                 // loop counters
    int    cnt;                                     // character count
    int    result;
    unsigned char *p;
    typedef struct tagPCXHeader {                   // pcx header
        unsigned char   id;                         // file id
        unsigned char   version;                    // PCX version
        unsigned char   rle;                        // compression
        unsigned char   bpp;                        // bits per pixel
        unsigned short  xstart;                     // starting coordinates
        unsigned short  ystart;
        unsigned short  xend;                       // ending coordinates
        unsigned short  yend;
        unsigned short  hres;                       // horizontal resolution
        unsigned short  vres;                       // vertical resolution
        unsigned char   pal[48];                    // palette (up to 16 colors)
        unsigned char   reserved;                   // reserved byte
        unsigned char   nbitp;                      // number of bitplanes
        unsigned short  bytesPerLine;               // bytes per line
        unsigned short  palType;                    // palette type
        unsigned short  horizontalSize;             // horizontal size
        unsigned short  verticalSize;               // vertical size
        unsigned char   reserved2[54];              // reserved bytes
    } PCXHeader;
    PCXHeader pcxHeader;

    assert(bitmap);                                 // check bitmap structure
    assert(filename);                               // check filename
    assert(bitmap->pixels);                         // check pixels
    assert(bitmap->type==BitmapTypeTrueColor);      // check bitmap type
    if (!bitmap) return false;                      // bitmap does not exists
    if (!bitmap->pixels) return false;              // pixel array does not exists
    if (!filename) return false;                    // empty filename
    if (bitmap->type!=BitmapTypeTrueColor) return false; // invalid bitmap type

    size=bitmap->bytesPerLine;                      // compute scanline size
    
    // fill in PCX header
    memset(&pcxHeader, 0, sizeof(pcxHeader));       // clear header
    pcxHeader.id=10;                                // manufacturer
    pcxHeader.version=5;                            // Paintbrush version 3.0 and >
    pcxHeader.rle=1;                                // PCX run length encoding
    pcxHeader.bpp=8;                                // 1 bit per pixel
    pcxHeader.xstart=0;                             // window coordinates
    pcxHeader.ystart=0;
    pcxHeader.xend=bitmap->width-1;
    pcxHeader.yend=bitmap->height-1;
    pcxHeader.hres=300;                             // horizontal resolution
    pcxHeader.vres=300;                             // vertical resolution
    pcxHeader.reserved=0;                           // should be set to zero
    pcxHeader.nbitp=3;                              // one bitplane
    if (bitmap->width & 0x01)
        pcxHeader.bytesPerLine=(bitmap->width)+1;   // must be even number
    else
        pcxHeader.bytesPerLine=(bitmap->width);     // must be even number
    pcxHeader.palType=1;                            // palette type
    pcxHeader.horizontalSize=0;                     // horizontal screen size - new field
    pcxHeader.verticalSize=0;                       // vertical screen size - new field

    fout=fopen(filename,"wb");                      // open output file for writing
    assert(fout);
    if (!fout) return false;                        // when file could not be opened
    cnt=fwrite(&pcxHeader, sizeof(pcxHeader), 1, fout); // try to write file header
    assert(cnt==1);
    if (cnt!=1) {                                   // check file header
        fclose(fout);
        return false;                               // function failed
    }
    for (i=(bitmap->height-1); i>=0; i--) {         // write bitmap data
        unsigned char this;
        unsigned char last;
        for (k=0; k<3; k++) {
            int runCount=0;
            p=bitmap->pixels+i*size+(2-k);
            last=~(*p);
            for (j=0; j<(signed int)(bitmap->width); j++) { // for all scanlines
                this=*p;
                p+=3;
                if (this==last) {                   // there is a "run" in the data, encode it
                    runCount++;
                    if (runCount==63) {             // maximum run length
                        if (!(pcxEncPut(last, runCount, fout))) {
                            fclose(fout);           // try to close file
                            return false;           // function failed
                        }
                        runCount=0;
                    }
                }
                else {                              // no "run"
                    if (runCount) {
                        if (!(pcxEncPut(last, runCount, fout))) {
                            fclose(fout);           // try to close file
                            return false;           // function failed
                        }
                    }
                    last=this;
                    runCount=1;
                }
            }
            if (runCount) {                         // finish up
                if (!(pcxEncPut(last, runCount, fout))) {
                    fclose(fout);                   // try to close file
                    return false;                   // function failed
                }
            }
            if (bitmap->bytesPerLine & 0x01) fputc(0x00, fout); // even bytes at scanline
        }
    }
    result=fclose(fout);                            // try to close file
    assert(result!=EOF);
    if (result==EOF) {                              // when file close failed
        return false;                               // function failed
    }
    return true;                                    // function succesed
}



// }}}
// bitmapLoadFromPCX() {{{



//-----------------------------------------------------------------------------
// This function reads raster data from PCX file
//-----------------------------------------------------------------------------
int bitmapLoadFromPCX(Bitmap **bitmap, char *filename)
{
    FILE *fin;
    int   width, height;                            // bitmap/pixmap properties
    int   i;
    int   count;                                    // block count
    Bitmap *bmp=NULL;
    typedef struct tagPCXHeader {                   // pcx header
        unsigned char   id;                         // file id
        unsigned char   version;                    // PCX version
        unsigned char   rle;                        // compression
        unsigned char   bpp;                        // bits per pixel
        unsigned short  xstart;                     // starting coordinates
        unsigned short  ystart;
        unsigned short  xend;                       // ending coordinates
        unsigned short  yend;
        unsigned short  hres;                       // horizontal resolution
        unsigned short  vres;                       // vertical resolution
        unsigned char   pal[48];                    // palette (up to 16 colors)
        unsigned char   reserved;                   // reserved byte
        unsigned char   nbitp;                      // number of bitplanes
        unsigned short  bytesPerLine;               // bytes per line
        unsigned short  palType;                    // palette type
        unsigned short  horizontalSize;             // horizontal size
        unsigned short  verticalSize;               // vertical size
        unsigned char   reserved2[54];              // reserved bytes
    } PCXHeader;
    PCXHeader           hdr;                        // pcx header
    unsigned char       pal[768];                   // palette

    assert(filename);
    if (!filename) return false;                    // bad filename - function failed
    fin=fopen(filename, "rb");                      // try to open input file
    assert(fin);
    if (fin) {                                      // file open OK
        count=fread(&hdr, sizeof(hdr), 1, fin);     // try to read file header
        assert(count==1);
        if (count!=1) {
            fclose(fin);
            return false;                           // file is corrupted - function failed
        }
        assert(hdr.id==0x0a);                       // check header
        assert(hdr.rle==1);
        if (hdr.id != 0x0a || hdr.rle != 1) {       // check file header
            fclose(fin);
            return false;
        }
        width=hdr.xend-hdr.xstart+1;                // bitmap size
        height=hdr.yend-hdr.ystart+1;

        // create window bitmap
        if (hdr.bpp==1 && hdr.nbitp==1) bmp=bitmapCreateTwoColors(width, height);
        if (hdr.bpp==1 && hdr.nbitp==4) bmp=bitmapCreateGrayScale(width, height);
        if (hdr.bpp==8 && hdr.nbitp==1) bmp=bitmapCreateGrayScale(width, height);
        if (hdr.bpp==8 && hdr.nbitp==3) bmp=bitmapCreateTrueColor(width, height);

        if (hdr.bpp==1 && hdr.nbitp==1) {           // 1bpp files
            memset(pal, 0, 768);
            for (i=0; i<6; i++) {
                pal[i]=hdr.pal[i]>>2;               // read palette
            }
        }
        if (hdr.bpp==1 && hdr.nbitp==4) {           // 4 bpp files
           memset(pal, 0, 768);
           for (i=0; i<48; i++) {
               pal[i]=hdr.pal[i]>>2;                // read palette
           }
        }
        if (hdr.bpp==8 && hdr.nbitp==1) {           // 8 bpp files
            fseek(fin, -769, SEEK_END);
            i=getc(fin);                            // read palette magic number
            if (i==0x0c) {                          // palette is presented
                for (i=0; i<768; i++) {             // read palette
                    pal[i]=((unsigned char)getc(fin))>>2;
                }
            }
        }
        fseek(fin, sizeof(PCXHeader), SEEK_SET);    // goto pixel records
        while (height--) {                          // for all scan lines
            unsigned char c;
            unsigned char *outpix;                  // output pixels
            if (hdr.bpp==1 && hdr.nbitp==1) {       // 1bpp pixmaps
                outpix=bmp->pixels+height*bmp->bytesPerLine;
                i=0;
                do {                                // for all pixels on scan line
                    c=(unsigned char)getc(fin);     // read byte
                    if ((c & 0xc0) != 0xc0) {       // normal color
                        *outpix++=c;                // write pixel color
                        i++;
                    }
                    else {                          // in "run"
                        unsigned char v;
                        v=(unsigned char)getc(fin); // read pixel color
                        c&=~0xc0;                   // decode "run" length
                        while (c>0 && i<width) {    // write pixel colors
                            *outpix++=v;
                            i++;
                            c--;
                        }
                    }
                } while (i<(width>>3));
            }
            if (hdr.bpp==8 && hdr.nbitp==1) {       // 8bpp pixmaps
                outpix=bmp->pixels+height*bmp->bytesPerLine;
                i=0;
                do {                                // for all pixels on scan line
                    c=(unsigned char)getc(fin);     // read byte
                    if ((c & 0xc0) != 0xc0) {       // normal color
                        *outpix++=c;                // write pixel color
                        i++;
                    }
                    else {                          // in "run"
                        unsigned char v;
                        v=(unsigned char)getc(fin); // read pixel color
                        c&=~0xc0;                   // decode "run" length
                        while (c>0 && i<width) {    // write pixel colors
                            *outpix++=v;
                            i++;
                            c--;
                        }
                    }
                } while (i<width);
            }
            if (hdr.bpp==8 && hdr.nbitp==3) {       // 24bpp pixmaps
                int k;
                for (k=0; k<3; k++) {
                    outpix=bmp->pixels+height*bmp->bytesPerLine+2-k;
                    i=0;
                    do {                            // for all pixels on scan line
                        c=(unsigned char)getc(fin); // read byte
                        if ((c & 0xc0) != 0xc0) {   // normal color
                            *outpix=c;              // write pixel color
                            outpix+=3;
                            i++;
                        }
                        else {                      // in "run"
                            unsigned char v;
                            v=(unsigned char)getc(fin);// read pixel color
                            c&=~0xc0;               // decode "run" length
                            while (c>0 && i<width) {// write pixel colors
                                *outpix=v;
                                outpix+=3;
                                i++;
                                c--;
                            }
                        }
                    } while (i<width);
                }
            }
        }
        *bitmap=bmp;                                // return pointer to new bitmap
        return true;
    }
    else {
        return false;                               // file could not be opened
    }                                               // function failed
}



// }}}



#ifdef __BORLANDC__                                 // this option is to avoid some
#pragma option -w-                                  // compiler warnings
#endif



// ----------------------------------------------------------------------------
// finito
// ----------------------------------------------------------------------------

// vim:expandtab:foldenable:foldmethod=marker:foldclose=:foldmarker={{{,}}}

