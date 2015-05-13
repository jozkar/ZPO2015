/*  Projekt do predmetu ZPO - Nacitani a ukladani obrazu
 *  Autori: Zdenek Biberle (xbiber00), Josef Ridky(xridky00), Zdenek Tretter (xtrett00)
 *  Popis: Implementace nacitani a ukladani obrazu ve formatu PCX (8 bitu - grayscale, 24 bitu - RGB)
 *  Datum: 9. 5. 2015
 */
#include "pcx.hpp"

/*
 *  Konstruktor bitmapy
 */
BMP::BMP(int type, unsigned int width, unsigned int height){

    if(width <= 0 || height <= 0){
		std::cerr << "Zero width or height.\n";
		exit(ERROR);
	}
    switch (type) {
        case GrayScale: createGrayScale(width, height);
            			break;
        case TrueColor: createTrueColor(width, height);
            			break;
        default:  std::cerr << "Unknow bitmap format.\n";
            	  exit(ERROR);
				  break;
    };
}

/*
 *  Funkce pro vytvoreni grayscale bitmapy
 */
void BMP::createGrayScale(unsigned int width, unsigned int height){

    unsigned int size;

    if(width <= 0 || height <= 0){
		std::cerr << "Zero width or height.\n";
		exit(ERROR);
	}

    this->setType(GrayScale);
    this->setBitsPerPixel(8);
    this->setWidth(width);
    this->setHeight(height);
    this->setBytesPerLine(width);
    size = this->getBytesPerLine() * this->getHeight();
    this->setPixelCount(width * height);
    this->setByteCount(size);
    this->loaded = false;

	this->pixels = (unsigned char *)malloc(sizeof(unsigned char) * size);

	if(this->pixels == NULL){
		std::cout << "ERROR: Not enough memory on heap.\n";
		exit(ERROR);
	}

	fillPixels();
}

/*
 *  Funkce pro vytvoreni RGP bitmapy
 */
void BMP::createTrueColor(unsigned int width, unsigned int height){

    unsigned int size;

    if(width <= 0 || height <= 0){
		std::cerr << "Zero width or height.\n";
		exit(ERROR);
	}

    this->setType(TrueColor);
    this->setBitsPerPixel(24);
    this->setWidth(width);
    this->setHeight(height);
    this->setBytesPerLine(width * 3);
    size = this->getBytesPerLine() * this->getHeight();
    this->setPixelCount(width * height);
    this->setByteCount(size);
    this->loaded = false;

	this->pixels = (unsigned char *)malloc(sizeof(unsigned char) * size);

	if(this->pixels == NULL){
		std::cout << "ERROR: Not enough memory on heap.\n";
		exit(ERROR);
	}

	fillPixels();
}

/*
 *  Uvolneni pameti po poli pixelu
 */
BMP::~BMP(){
    free(this->pixels);
}

/*
 *  Vyplneni bitmapy podle barevneho modelu
 */
void BMP::fillPixels(){

    unsigned char *pixel = this->pixels;
    unsigned char *old = pixel;

    switch (this->getBitsPerPixel()) {
        case 8: for(unsigned int y = 0; y < this->getHeight(); y++) {
                	for(unsigned int x = 0; x < this->getWidth(); x++) {
                    	*pixel++ = (unsigned char)(x+y);
                	}
                	old += this->getBytesPerLine();
                	pixel = old;
            	}
            	break;
        case 24: for(unsigned int y = 0; y < this->getHeight(); y++) {
                 	for(unsigned int x = 0; x < this->getWidth(); x++) {
                    	*pixel++ = (unsigned char)x;
                    	*pixel++ = (unsigned char)y;
                    	*pixel++ = (unsigned char)(x+y);
                	}
                	old += this->getBytesPerLine();
                	pixel = old;
            	 }
            	 break;
        default: break;
    }
}

/*
 *  Ziskani typu bitmapy
 */
int BMP::getType(){

	return this->type;
}

/*
 *  Ziskani sirky
 */
unsigned int BMP::getWidth(){

	return this->width;
}

/*
 *  Ziskani vysky
 */
unsigned int BMP::getHeight(){

	return this->height;
}

/*
 *  Ziskani bajtu na radek
 */
unsigned int BMP::getBytesPerLine(){

	return this->bytesPerLine;
}

/*
 *  Ziskani bitu na pixel
 */
unsigned int BMP::getBitsPerPixel(){

	return this->bitsPerPixel;
}

/*
 *  Ziskani celkoveho poctu pixelu
 */
unsigned int BMP::getPixelCount(){

	return this->pixelCount;
}

/*
 *  Ziskani celkoveho poctu bajtu
 */
unsigned int BMP::getByteCount(){

	return this->byteCount;
}

/*
 *  Ziskani barvy
 */
unsigned int BMP::getColor(){

	return this->color;
}

/*
 *  Ziskani RLE priznaku
 */
unsigned int BMP::getRLE(){

        return this->header.rle;
}

/*
 *  Ziskani hodnoty pixelu ze zadane pozice
 */
int BMP::getPixel(int x, int y){

    if((x < 0 || x >= (int)this->getWidth()) &&
	   (y < 0 || y >= (int)this->getHeight())){

	   return 0;
	}

    switch (this->getType()) {
        case GrayScale: return getPixelGrayScale(x, y);
            			break;
        case TrueColor: return getPixelRGB(x, y);
            			break;
        default: return 0;
            	 break;
    }
}

/*
 *  Ziskani hodnoty pixelu ze zadane pozice (verze pro GrayScale)
 */
int BMP::getPixelGrayScale(int x, int y){

    return *(this->pixels + x + y * this->getBytesPerLine());
}

/*
 *  Ziskani hodnoty pixelu ze zadane pozice (verze pro RGB)
 */
int BMP::getPixelRGB(int x, int y){

    unsigned char *px;
    int result;

    px = this->pixels + 3 * x + y * this->getBytesPerLine();
    result = *px++;
    result |= (*px++)<<8;
    result |= (*px)<<16;

    return result;
}

/*
 *  Nastaveni typu bitmapy
 */
void BMP::setType(int type){
	this->type = type;
}

/*
 *  Nastaveni "Bitu na pixel"
 */
void BMP::setBitsPerPixel(unsigned int bpp){
	this->bitsPerPixel = bpp ;
}

/*
 *  Nastaveni sirky obrazu
 */
void BMP::setWidth(unsigned int width){
	this->width = width;
}

/*
 *  Nastaveni vysky
 */
void BMP::setHeight(unsigned int height){
	this->height = height;
}

/*
 *  Nastaveni bajtu na radek
 */
void BMP::setBytesPerLine(unsigned int bpl){
	this->bytesPerLine = bpl;
}

/*
 *  Nastaveni poctu pixelu
 */
void BMP::setPixelCount(unsigned int count){
	this->pixelCount = count;
}

/*
 *  Nastaveni poctu bajtu
 */
void BMP::setByteCount(unsigned int count){
	this->byteCount = count;
}

/*
 *  Nastaveni barvy
 */
void BMP::setColor(int color){

    if(color > 0x00ffffff){
		return;
	}

    this->color = color;
    this->R = (unsigned char)(color & 0x000000ff);
    this->G = (unsigned char)((color & 0x0000ff00) >> 8);
    this->B = (unsigned char)((color & 0x00ff0000) >> 16);
}

/*
 *  Nastaveni RGB barvy
 */
void BMP::setColorRGB(unsigned char r, unsigned char g, unsigned char b){

    this->color = (r) | (g << 8) | (b << 16);
    this->R = r;
    this->G = g;
    this->B = b;
}

/*
 *  "Vykresleni" pixelu na danou pozici
 */
void BMP::putPixel(int x, int y){

    if((x < 0 || x >= (int)this->getWidth()) &&
       (y < 0 || y >= (int)this->getHeight())){

	  		return;
	}

    switch (this->getType()) {
        case GrayScale: return putPixelGrayScale(x, y, (unsigned char)this->getColor());
            			break;
        case TrueColor: return putPixelInt(x, y, this->color);
            			break;
        default: return;
            	break;
    }
}


/*
 *  "Vykresleni" pixelu na danou pozici (pouzito pro GrayScale)
 */
void BMP::putPixelGrayScale(int x, int y, unsigned char color){

    unsigned char *px;
    px = this->pixels + x + y * this->getBytesPerLine();
    *px = color;
}

/*
 *  "Vykresleni" pixelu na danou pozici (pouzito pro RGB)
 */
void BMP::putPixelInt(int x, int y, int color){

    unsigned char *px;
    px = this->pixels + 3 * x + y * this->getBytesPerLine();
    *px = (unsigned char)(color & 0x000000ff);
    px++;
    *px = (unsigned char)((color & 0x0000ff00) >> 8);
    px++;
    *px = (unsigned char)((color & 0x00ff0000) >> 16);
}

/*
 *  "Vykresleni" pixelu na danou pozici (pouzito pro RGB)
 */
void BMP::putPixelRGB(int x, int y, unsigned char r, unsigned char g, unsigned char b){

    unsigned char *px;
    px = this->pixels + 3 * x + y * this->getBytesPerLine();
    *px++ = r;
    *px++ = g;
    *px = b;
}

/*
 *  Posunuti cursor pointeru na danou pozici
 */
void BMP::moveTo(int x, int y){

    if((x > (int)this->getWidth()) ||
       (y > (int)this->getHeight())){
	   		return;
	}

	this->cpx = x;
    this->cpy = y;
}

/*
 *  "Vykresleni" radku od aktualniho cursor pointeru po danou pozici
 */
void BMP::lineTo(int x, int y)
{
    unsigned char *px;
    int dx, dy, npx, delta, di1, di2, xi1, xi2, yi1, yi2,
		oldCpx = this->cpx, oldCpy = this->cpy;

    if((x >= (int)this->getWidth()) ||
	   (y >= (int)this->getHeight())){
	   		return;
	}

    switch (this->getType()){
        case GrayScale: px = this->pixels + this->cpx + this->cpy * this->getWidth();
            			dx=abs(x - this->cpx);
            			dy=abs(y - this->cpy);

				if(dx >= dy){
					// uhel je mensi nez 45 stupnu
                			npx = dx;
			                delta = (dy << 1) - dx;
			                di1 = dy << 1;
			                di2 = (dy - dx) << 1;
			                xi1 = xi2 = 1;
			                yi1 = 0;
			                yi2 = this->getWidth();
            			}else{
			                npx = dy;
			                delta = (dx << 1) - dy;
			                di1 = dx << 1;
			                di2 = (dx - dy) << 1;
			                xi1 = 0;
			                yi1 = yi2 = this->getWidth();
			                xi2 = 1;
			            }

				if((int)this->cpx > x){
					// inverze x-ovych prirustku
			                xi1 = -xi1;
			                xi2 = -xi2;
			            }

				if((int)this->cpy > y){
				        // inverze y-ovych prirustku
			                yi1 = -yi1;
			                yi2 = -yi2;
			            }

				for (int i = 0; i <= npx; i++) {
			                *px = (unsigned char)this->getColor();
			                if(delta < 0){
			                    delta += di1;
			                    px += xi1+yi1;
			                }else{
			                    delta += di2;
			                    px += xi2 + yi2;
			                }
			            }
			            break;

		case TrueColor: px = this->pixels + 3 * (this->cpx + this->cpy * this->getWidth());
            			dx=abs(x - this->cpx);
            			dy=abs(y - this->cpy);

				if(dx >= dy){
					// uhel je mensi nez 45 stupnu
                			npx = dx;
			                delta = (dy << 1) - dx;
			                di1 = dy << 1;
			                di2 = (dy - dx) << 1;
			                xi1 = xi2 = 3;
			                yi1 = 0;
			                yi2 = 3 * this->getWidth();
            			}else{
			                npx = dy;
			                delta = (dx << 1) - dy;
			                di1 = dx << 1;
			                di2 = (dx - dy) << 1;
			                xi1 = 0;
			                yi1 = yi2 = 3 * this->getWidth();
			                xi2 = 3;
			            }

				if((int)this->cpx > x){
					// inverze x-ovych prirustku
			                xi1 = -xi1;
			                xi2 = -xi2;
			            }

				if((int)this->cpy > y){
 	 	 		        // inverze y-ovych prirustku
			                yi1 = -yi1;
			                yi2 = -yi2;
			            }

				for (int i = 0; i <= npx; i++) {
			                *px = (unsigned char)this->R;
                			*(px+1) = (unsigned char)this->G;
                			*(px+2) = (unsigned char)this->B;

			                if(delta < 0){
			                    delta += di1;
			                    px += xi1+yi1;
			                }else{
			                    delta += di2;
			                    px += xi2 + yi2;
			                }
			            }
			            break;

        default: break;
    }

    this->cpx = x;
    this->cpy = y;
}

/*
 *  "Vykresleni" radku od dane pozice (1) do cilove (2), ktera bude novou pozici cursor pointeru
 */

void BMP::line(int x1, int y1, int x2, int y2){

    moveTo(x1, y1);
    lineTo(x2, y2);
}

/*
 *  Vycisteni bitmapy
 */
void BMP::clear(){

    memset(this->pixels, 0, this->getByteCount());
}

/*
 *  Vyplneni bitmapy aktualni barvou
 */
void BMP::fill(){

    unsigned char *px;

     switch (this->getType()){
        case GrayScale: memset(this->pixels, this->getColor(), this->getByteCount());
            			break;
        case TrueColor: px = this->pixels;
			            for (int j = 0; j < (int)this->getHeight(); j++){
			                for (int i = 0; i < (int)this->getWidth(); i++){
			                    *px++ = this->R;
			                    *px++ = this->G;
			                    *px++ = this->B;
			                }
			            }
			            break;
        default: break;
    }
}

/*
 *  Vypsani bajtu na vystup (dvou v pripade kodovaneho vystupu)
 *  Navratova hodnota: pocet zapsanych bajtu
 */
int BMP::write(unsigned char byt, unsigned int count, FILE *out){

    if (count) {
        if ((count == 1) && (0xc0 != (0xc0 & byt))) {
            putc((int)byt, out);
            return 1;
        }else{
            putc((int)0xC0 | count, out);
            putc((int)byt, out);
            return 2;
        }
    }
    return 0;
}

/*
 *  Ulozeni bitmapy do PCX souboru
 */
int BMP::saveToPCX(char *out){

    int result;
    if(out == NULL){
	 	std::cerr << "ERROR: Empty output file name.\n";
		return ERROR;
	}
    switch (this->getType()){
        case GrayScale: result = saveToGrayScalePCX(out);
            			break;
        case TrueColor: result = saveToTrueColorPCX(out);
            			break;
        default: result = ERROR;
            	 break;
    }

    return result;
}

/*
 *  Ulozeni bitmapy do PCX souboru (pouzito pro GrayScale)
 */
int BMP::saveToGrayScalePCX(char *out){

    FILE *fout;
    int size, count, result;
    unsigned char *px;

    if(this->pixels == NULL){
		return ERROR;
	}

	if(out == NULL){
		std::cerr << "ERROR: Empty output file name.\n";
		return ERROR;
	}

    size = this->getBytesPerLine();

    // naplneni hlavicky
    memset(&this->header, 0, sizeof(this->header));
    this->header.id = 10;                                // manufacturer
    this->header.version = 5;                            // Paintbrush version 3.0 and >
    this->header.rle = 1;                                // PCX run length encoding
    this->header.bpp = 8;                                // 1 bit per pixel
    this->header.xBegin = 0;
    this->header.yBegin = 0;
    this->header.xEnd = this->getWidth() - 1;
    this->header.yEnd = this->getHeight() - 1;
    this->header.hDPI = 300;
    this->header.vDPI = 300;
    this->header.reservedByte = 0;
    this->header.numberBitPlanes = 1;
    if (this->getWidth() & 0x01)
        this->header.bytesPerLine = this->getWidth() + 1;
    else
        this->header.bytesPerLine = this->getWidth();
    this->header.paletteType = 1;
    this->header.hSize = 0;
    this->header.vSize = 0;

    fout = fopen(out,"wb");
    if(fout == NULL){
		return ERROR;
	}

	count = fwrite(&this->header, sizeof(this->header), 1, fout);

    if(count != 1) {
        fclose(fout);
        return ERROR;
    }

    for (int i = (this->getHeight() - 1); i >= 0; i--){
        unsigned char character;
        unsigned char last;
        int runCount = 0;
        px = this->pixels + i * size;
        last = ~(*px);

        for (int j = 0; j < (signed int)this->getWidth(); j++) {
            character = *px++;
            if(character == last){
                runCount++;
                if(runCount == 63){
                    if (!(this->write(last, runCount, fout))) {
                        fclose(fout);
                        return ERROR;
                    }
                    runCount=0;
                }
            }else{
                if (runCount) {
                    if(!(this->write(last, runCount, fout))){
                        fclose(fout);
                        return ERROR;
                    }
                }
                last = character;
                runCount = 1;
            }
        }
        if (runCount) {
            if (!(this->write(last, runCount, fout))) {
                fclose(fout);
                return ERROR;
            }
        }
        if(this->getBytesPerLine() & 0x01){
			fputc(0x00, fout);
		}
    }

	fputc(0x0c, fout);

    // zapis palety (po slozkach RGB)
    for(int i = 0; i < 256; i++){
        fputc(i, fout);
        fputc(i, fout);
        fputc(i, fout);

    }

    fclose(fout);

	return OK;
}

/*
 *  Ulozeni bitmapy do PCX souboru (pouzito pro RGB)
 */
int BMP::saveToTrueColorPCX(char *out){

    FILE *fout;
    int size, count, result;
    unsigned char *px;

    if(this->pixels == NULL){
		return ERROR;
	}

	if(out == NULL){
		std::cerr << "ERROR: Empty output file name.\n";
		return ERROR;
	}

    size = this->getBytesPerLine();

    // naplneni hlavicky
    if(!this->loaded){
	    memset(&this->header, 0, sizeof(this->header));
	    this->header.id = 10;                                // manufacturer
	    this->header.version = 5;                            // Paintbrush version 3.0 and >
	    this->header.rle = 1;                                // PCX run length encoding
	    this->header.bpp = 8;                                // 1 bit per pixel
	    this->header.xBegin = 0;
	    this->header.yBegin = 0;
	    this->header.xEnd = this->getWidth() - 1;
	    this->header.yEnd = this->getHeight() - 1;
	    this->header.hDPI = 300;
	    this->header.vDPI = 300;
	    this->header.reservedByte = 0;
	    this->header.numberBitPlanes = 3;
	    if (this->getWidth() & 0x01)
	        this->header.bytesPerLine = this->getWidth() + 1;
	    else
	        this->header.bytesPerLine = this->getWidth();
	    this->header.paletteType = 1;
	    this->header.hSize = 0;
            this->header.vSize = 0;
    }else{
 	    this->header.id = 10;                                // manufacturer
            this->header.version = 5;                            // Paintbrush version 3.0 and >
            this->header.rle = 1;                                // PCX run length encoding
            this->header.bpp = 8;
	    this->header.numberBitPlanes = 3;
    }

    fout = fopen(out,"wb");
    if(fout == NULL){
		return ERROR;
	}

	count = fwrite(&this->header, sizeof(this->header), 1, fout);

    if(count != 1) {
        fclose(fout);
        return ERROR;
    }

    for (int i = this->getHeight() - 1; i >= 0; i--){
        unsigned char character;
        unsigned char last;
        for(int l = 0; l < 3; l++) {
            int runCount = 0;
            px = this->pixels + i * size + (2 - l);
            last = ~(*px);

            for(int j = 0; j < (signed int)this->getWidth(); j++) {
            	character = *px;
                px += 3;
                if(character == last){
	                runCount++;
	                if(runCount == 63){
	                    if (!(this->write(last, runCount, fout))) {
	                        fclose(fout);
	                        return ERROR;
	                    }
	                    runCount=0;
	                }
	            }else{
                    if (runCount) {
	                    if(!(this->write(last, runCount, fout))){
	                        fclose(fout);
	                        return ERROR;
	                    }
	                }
	                last = character;
	                runCount = 1;
	            }
	        }
	        if (runCount) {
	            if (!(this->write(last, runCount, fout))) {
	                fclose(fout);
	                return ERROR;
	            }
	        }
	        if(this->getBytesPerLine() & 0x01){
				fputc(0x00, fout);
			}
        }
    }

    fclose(fout);

	return OK;
}


/*
 *  Nacteni souboru ve formatu PCX
 */
int BMP::loadFromPCX(char *in){

    FILE *fin;
    int width, height;                            // bitmap/pixmap properties

    int count;

    if(in == NULL){
		std::cerr << "ERROR: Empty input file name.\n";
		return ERROR;
	}

	fin = fopen(in, "rb");

    if(fin){
		count = fread(&this->header, sizeof(this->header), 1, fin);

        if(count != 1) {
            std::cerr << "ERROR: Input file is corrupted.\n";
			fclose(fin);
            return ERROR;
        }


		if(this->header.id != 0x0a || this->header.rle !=1){
			std::cerr << "ERROR: Unknown header id in input file.\n";
		    fclose(fin);
            return ERROR;
        }

		width = this->header.xEnd - this->header.xBegin + 1;
        height = this->header.yEnd - this->header.yBegin + 1;

        // vytvoreni bitmapy
        if(this->header.bpp == 8 && this->header.numberBitPlanes == 1){
			createGrayScale(width, height);
			this->loaded = true;
		}else{
        if(this->header.bpp == 8 && this->header.numberBitPlanes == 3){
			createTrueColor(width, height);
			this->loaded = true;
		}else{
			std::cerr << "ERROR: Unknown bitmap type.\n";
	                fclose(fin);
        		return ERROR;
		}
	}

        // posun v souboru na bitmapu
        fseek(fin, sizeof(this->header), SEEK_SET);

	while(height--){
            unsigned char c;
            unsigned char *opx;

            if(this->header.bpp == 8 && this->header.numberBitPlanes == 1){

				opx = this->pixels + (height * this->getBytesPerLine());
                int i=0;

                	do{
                        if((height + 1) == this->header.yEnd - this->header.yBegin + 1);
                        else if(this->getBytesPerLine() & 0x01 && i == 0)
                           c = (unsigned char)getc(fin);
	                    c = (unsigned char)getc(fin);
	                    if((c & 0xc0) != 0xc0) {       // normal color
	                        *opx++ = c;
	                        i++;
	                    }else{
	                        unsigned char d;
	                        d = (unsigned char)getc(fin);
	                        c &= ~0xc0;
	                        while(c > 0 && i < width){
	                            *opx++ = d;
	                            i++;
	                            c--;
	                        }
	                    }
	                }while(i < width);

            }

            if(this->header.bpp == 8 && this->header.numberBitPlanes == 3){

                for(int j = 0; j < 3; j++) {
                    opx = this->pixels + (height * this->getBytesPerLine()) + 2 - j;
                    int i = 0;

                    	do {
	                        c = (unsigned char)getc(fin);
	                        if((c & 0xc0) != 0xc0){
	                            *opx = c;
	                            opx += 3;
	                            i++;
	                        }else{
	                            unsigned char d;
	                            d = (unsigned char)getc(fin);
	                            c &= ~0xc0;
	                            while(c > 0 && i < width){
	                                *opx = d;
	                                opx += 3;
	                                i++;
	                                c--;
	                            }
	                        }
	                    } while(i < width);

                }
            }
        }
        return OK;
    }else{
        std::cerr << "ERROR: Unable to open input file for reading.\n";
		return ERROR;
    }
}
