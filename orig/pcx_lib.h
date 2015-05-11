// ----------------------------------------------------------------------------
// application: PCX bitmaps manipulation library
// author:      Pavel Tisnovsky
// file name:   pcx_lib.h
// description: base bitmaps manipulation library, common bitmap functions,
//              functions for reading and writing bitmaps to/from PCX format
// ----------------------------------------------------------------------------



#ifndef _PCX_LIB_H_
#define _PCX_LIB_H_

// bitmap types
enum BitmapType {
    BitmapTypeUndefined     =0,
    BitmapTypeTwoColors     =1,
    BitmapTypeGrayScale     =2,
    BitmapTypeTrueColor     =3
};

// bitmap errors
enum BitmapError {
    BitmapOK                =0,
    BitmapErrorUndefined    =1,
    BitmapErrorMalloc       =2
};

typedef enum {
    BitmapFileOK,
    BitmapFileErrorBadFileName,
    BitmapFileErrorBadBitmapType,
    BitmapFileErrorNullBitmap,
    BitmapFileErrorFunctionNotImplemented,
    BitmapFileErrorFileOpenFailed,
    BitmapFileErrorFileCloseFailed,
    BitmapFileErrorFileReadFailed,
    BitmapFileErrorFileWriteFailed
} BitmapFileError;

// bitmap structure
typedef struct {
    int             type;
    unsigned int    width;
    unsigned int    height;
    unsigned int    bpp;
    unsigned int    bytesPerLine;
    unsigned int    totalPixels;
    unsigned int    totalBytes;
    unsigned char  *pixels;
    int             currentColorBW;
    unsigned int    currentColor;
    unsigned char   currentColorR;
    unsigned char   currentColorG;
    unsigned char   currentColorB;
    unsigned int    cpx;
    unsigned int    cpy;
} Bitmap;



#ifdef __cplusplus
    extern "C" {
#endif

/* creating and destroying bitmaps */
Bitmap * bitmapCreate(int type, unsigned int width, unsigned int height);
Bitmap * bitmapCreateTwoColors(unsigned int width, unsigned int height);
Bitmap * bitmapCreateGrayScale(unsigned int width, unsigned int height);
Bitmap * bitmapCreateTrueColor(unsigned int width, unsigned int height);
int      bitmapDestroy(Bitmap *bitmap);

/* internal functions */
void     bitmapFillPixels(Bitmap *bitmap);

/* getters */
int      bitmapGetInfo(Bitmap *bitmap, char *string, int strlen);
int      bitmapGetWidth(Bitmap *bitmap);
int      bitmapGetHeight(Bitmap *bitmap);
int      bitmapGetSize(Bitmap *bitmap, int *width, int *height);
int      bitmapGetPixel(Bitmap *bitmap, int x, int y);
int      bitmapGetPixelBit(Bitmap *bitmap, int x, int y);
int      bitmapGetPixelGrayScale(Bitmap *bitmap, int x, int y);
int      bitmapGetPixelInt(Bitmap *bitmap, int x, int y);

/* setters */
int      bitmapSetColorBit(Bitmap *bitmap, int color);
int      bitmapSetColorInt(Bitmap *bitmap, int color);
int      bitmapSetColorRGB(Bitmap *bitmap, unsigned char r, unsigned char g, unsigned char b);

/* drawing functions */
int      bitmapPutPixel(Bitmap *bitmap, int x, int y);
int      bitmapPutPixelBit(Bitmap *bitmap, int x, int y, int color);
int      bitmapPutPixelGrayScale(Bitmap *bitmap, int x, int y, unsigned char color);
int      bitmapPutPixelInt(Bitmap *bitmap, int x, int y, int color);
int      bitmapPutPixelRGB(Bitmap *bitmap, int x, int y, unsigned char r, unsigned char g, unsigned char b);
int      bitmapMoveTo(Bitmap *bitmap, int x, int y);
int      bitmapLineTo(Bitmap *bitmap, int x, int y);
int      bitmapLine(Bitmap *bitmap, int x1, int y1, int x2, int y2);
int      bitmapClear(Bitmap *bitmap);
int      bitmapFill(Bitmap *bitmap);

/* PCX manipulation */
int      bitmapSave2PCX(Bitmap *bitmap, char *filename);
int      bitmapSave2TwoColorPCX(Bitmap *bitmap, char *filename);
int      bitmapSave2GrayScalePCX(Bitmap *bitmap, char *filename);
int      bitmapSave2TrueColorPCX(Bitmap *bitmap, char *filename);
int      bitmapLoadFromPCX(Bitmap **bitmap, char *filename);

#ifdef __cplusplus
    }
#endif

#endif



// ----------------------------------------------------------------------------
// finito
// ----------------------------------------------------------------------------

