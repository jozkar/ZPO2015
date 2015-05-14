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

	this->pixels = (unsigned char *)malloc(sizeof(unsigned char) * size);

	if(this->pixels == NULL){
		std::cout << "ERROR: Not enough memory on heap.\n";
		exit(ERROR);
	}

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

	this->pixels = (unsigned char *)malloc(sizeof(unsigned char) * size);

	if(this->pixels == NULL){
		std::cout << "ERROR: Not enough memory on heap.\n";
		exit(ERROR);
	}

}

/*
 *  Uvolneni pameti po poli pixelu
 */
BMP::~BMP(){
    free(this->pixels);
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
    int size, count;
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
    int size, count;
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
	    this->header.numberBitPlanes = 3;
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
		}else{
        if(this->header.bpp == 8 && this->header.numberBitPlanes == 3){
			createTrueColor(width, height);
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
