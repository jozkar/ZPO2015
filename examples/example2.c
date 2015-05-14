#include <zpogif.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

void* allocator(zpogif_format format, uint16_t width, uint16_t height, ptrdiff_t* pixel_stride, ptrdiff_t* row_stride, void* priv);
void deallocator(zpogif_format format, uint16_t width, uint16_t height, void* image, void* priv);
int ends_with(const char *str, const char *suffix);

int main(int argc, char** argv)
{
	void* image;
	uint16_t width;
	uint16_t height;
	ptrdiff_t pixel_stride;
	ptrdiff_t row_stride;
	zpogif_error err;
	zpogif_format format = ZPOGIF_RGB;

	// Otevřeme vstupní soubor
	FILE* fin = fopen(argv[1], "rb");
	if (!fin) { printf("Could not open input file\n"); return 1; }
	
	// Načteme obrázek do paměti
	err = zpogif_load(fin, format, &image, &width, &height, &pixel_stride, &row_stride, allocator, deallocator, NULL);
	if (err != ZPOGIF_NO_ERROR) { printf("GIF loading failed (%d)\n", err); fclose(fin); return 2; }
	
	// Uzavřeme vstupní soubor
	fclose(fin);
	
	printf("GIF loaded: %ux%upx, strides: %td, %td\n", width, height, pixel_stride, row_stride);
	
	// Dogenerujeme náhodný šum
	for (unsigned y = 0; y < height; y++)
	for (unsigned x = 0; x < width; x++)
	{
		uint8_t* pixel = (uint8_t*)image+ y * row_stride + x * pixel_stride;
		for (int i = 0; i < 3; i++)
		{
			pixel[i] = MIN(pixel[i] + rand() % 5, 255);
		}
	}
	
	// Otavřeme výstupní soubor
	FILE* fout = fopen(argv[2], "wb");
	if (!fout) { printf("Could not open output file\n"); return 3; }
	
	// Uložíme obrázek do souboru
	err = zpogif_save(fout, format, image, width, height, pixel_stride, row_stride);
	if (err != ZPOGIF_NO_ERROR) { printf("GIF saving failed (%d)\n", err); fclose(fout); return 4; }
	
	// Uzavřeme vstupní soubor
	fclose(fout);
	
	// Dealokujeme obrázek
	deallocator(format, width, height, image, NULL);
	
	return 0;
}

void* allocator(zpogif_format format, uint16_t width, uint16_t height, ptrdiff_t* pixel_stride, ptrdiff_t* row_stride, void* priv)
{
	printf("Allocating %ux%u image\n", width, height);
	int pixelsize = format == ZPOGIF_GRAYSCALE ? 1 : 3;
	*pixel_stride = pixelsize;
	*row_stride = width * pixelsize;
	return malloc(width * height * pixelsize);
}

void deallocator(zpogif_format format, uint16_t width, uint16_t height, void* image, void* priv)
{
	printf("Deallocating %ux%u image\n", width, height);
	free(image);
}
