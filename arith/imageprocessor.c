/***********************************************************************
 * 
 *                      imageprocessor.c
 *      Assignment: Arith
 *      Authors: Mishona Horton and Perucy Mussiba
 *      Date: 10/17/2024
 *      Purpose: This file implements PPM image processing, printing of
 *              compressed binary image to standard output and reading
 *              a compressed binary image file
 * 
 ***********************************************************************/
#include "imageprocessor.h"
Except_T file_err = { "file is too short" };
/**************************readppmimage********************************
 * 
 * Parameters:
 *      File *fp: file pointer of a PPM image
 *     A2Methods_T methods: methods for Uarray2 operations
 * 
 * Return: 
 *      Pnm_ppm image
 * 
 * Expects: valid file pointer and methods
 * 
 * Notes: the function processes a PPM image by trimming when needed. 
 * 
 * *******************************************************************/
Pnm_ppm readppmimage(FILE *fp, A2Methods_T methods) {
        Pnm_ppm image = Pnm_ppmread(fp, methods);
        bool trim = false;

        unsigned width = image->width;
        unsigned height = image->height;

        if (width % 2 != 0) {
                (image->width) = (image->width) - 1;
                width = image->width;
                trim = true;
        }
        if (height % 2 != 0) {
                (image->height) = (image->height) - 1;
                height = image->height;
                trim = true;
        }

        if (trim) {
              
                A2Methods_UArray2 new_pixels = methods->new(width, height, 
                        sizeof(struct Pnm_rgb));
                
                for(unsigned r = 0; r < height; r++) {
                        for(unsigned c = 0; c < width; c++) {
                                Pnm_rgb old_pix = image->methods->at(
                                        image->pixels, c, r
                                );
                                Pnm_rgb new_pix = methods->at(
                                        new_pixels, c, r
                                );
                                new_pix->red = old_pix->red;
                                new_pix->blue = old_pix->blue;
                                new_pix->green = old_pix->green;
                        }
                }
                methods->free(&(image->pixels));
                image->pixels = new_pixels;
        }
        return image;
}
/**************************print_compressedimg********************************
 * 
 * Parameters:
 *      A2Methods_UArray2 arr: 2D array containing coded words
 *     A2Methods_T methods: methods for Uarray2 operations
 * 
 * Return: 
 *      None
 * 
 * Expects: valid 2d array
 * 
 * Notes: the function prints coded words in big-endian order byte by byte
 * 
 * *******************************************************************/
void print_compressedimg(A2Methods_UArray2 arr, A2Methods_T methods)
{
        int width = methods->width(arr);
        int height = methods->height(arr);

        printf("COMP40 Compressed image format 2\n%u %u", width * 2, 
                height * 2);
        printf("\n");

        for(int i = 0; i < height; ++i) {
                for(int j = 0; j < width; ++j) {
                        uint64_t *word = methods->at(arr, j, i); 
                        uint32_t new_word = (uint32_t) (*word);

                        for(int w = 24; w >= 0; w = w - 8) {
                                unsigned char byte = (new_word >> w);
                                putchar(byte);
                        }
                        
                }
        }
}
/**************************code_word********************************
 * 
 * Parameters:
 *      File *fp: A file pointer to an open file of a compressed binary image
 *     A2Methods_T methods: methods for Uarray2 operations
 * 
 * Return: 
 *      A2Methods_UArray2 of coded bitwords from the file
 * 
 * Expects: valid file pointer and methods
 * 
 * Notes: the function reads the bitwords in a file byte by byte and stores
 *      the words in a 2d array of half the image width and height. The 
 *      function will CRE  when it is shorter than expected or if 
 *      it is an invalid binary file
 * 
 * *******************************************************************/
A2Methods_UArray2 code_word(FILE *fp, A2Methods_T methods) {
        unsigned width, height;
        int read = fscanf(fp, "COMP40 Compressed image format 2\n%u %u", &width,
                &height);
        assert(read == 2);
        int c = getc(fp);
        assert(c == '\n');

        width /= 2;
        height /= 2;

        A2Methods_UArray2 coded_word = methods->new(width, height, 
                sizeof(uint64_t));
        unsigned num_words = 0;
        
        for(unsigned r = 0; r < height; r++) {
                for(unsigned c = 0; c < width; c++) {
                        uint64_t word = 0;
                        for(int w = 24; w >= 0; w -= 8) {
                                int byte = getc(fp);
                                if (byte == EOF) {
                                        if (num_words < (width * height)) {
                                                RAISE(file_err);
                                        }  
                                }
                                
                                uint64_t curr = (uint64_t) byte;
                                curr = byte << w;
                                word = word | curr;  
                        }
                        
                        num_words++;
                        uint64_t *c_word = methods->at(coded_word, c, r);
                        *c_word = word;
                }
                
        }
        return coded_word;
}
