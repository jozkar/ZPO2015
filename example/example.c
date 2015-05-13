#include <zpogif.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void* allocator(uint16_t width, uint16_t height, ptrdiff_t* color_stride, ptrdiff_t* row_stride, void* priv);
void deallocator(uint16_t width, uint16_t height, void* image, void* priv);
int ends_with(const char *str, const char *suffix);

int main(int argc, char** argv)
{
	void* image;
	uint16_t width;
	uint16_t height;
	ptrdiff_t color_stride;
	ptrdiff_t row_stride;
	zpogif_error err;
	
	FILE* fin = fopen(argv[1], "rb");
	if (!fin) { printf("Nepodařilo se otevřít vstupní soubor\n"); return 1; }
	err = zpogif_load(fin, ZPOGIF_GRAYSCALE, &image, &width, &height, &color_stride, &row_stride, allocator, deallocator, NULL);
	if (err != ZPOGIF_NO_ERROR) { printf("GIF loading failed\n"); fclose(fin); return 2; }
	fclose(fin);
	
	printf("GIF loaded: %ux%upx, strides: %td, %td\n", width, height, color_stride, row_stride);
	
	FILE* fout = fopen(argv[2], "wb");
	if (!fout) { printf("Nepodařilo se otevřít výstupní soubor\n"); return 3; }
	err = zpogif_save(fout, ZPOGIF_GRAYSCALE, image, width, height, color_stride, row_stride);
	if (err != ZPOGIF_NO_ERROR) { printf("GIF loading failed\n"); fclose(fout); return 4; }
	fclose(fout);
	
	deallocator(width, height, image, NULL);
	
	return 0;
}

int ends_with(const char *str, const char *suffix)
{
	size_t lenstr = strlen(str);
	size_t lensuffix = strlen(suffix);
	if (lensuffix >  lenstr) return 0;
	return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

void* allocator(uint16_t width, uint16_t height, ptrdiff_t* color_stride, ptrdiff_t* row_stride, void* priv)
{
	printf("Allocating %ux%u image\n", width, height);
	*color_stride = 3;
	*row_stride = width * 3;
	return malloc(width * height * 3);
}

void deallocator(uint16_t width, uint16_t height, void* image, void* priv)
{
	printf("Deallocating %ux%u image\n", width, height);
	free(image);
}
