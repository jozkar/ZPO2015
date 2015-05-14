#ifndef ZPOPCX_H
#define ZPOPCX_H

#ifdef __cplusplus
extern "C" {
#endif

void createGrayScalePcx(int width, int height, const char *filename);
void createTrueColorPcx(int width, int height, const char *filename);
void loadGrayScalePcx(const char *filename, const char *out);
void loadTrueColorPcx(const char *filename, const char *out);

#ifdef __cplusplus
}
#endif

#endif
