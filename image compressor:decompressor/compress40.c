/***********************************************************************
 * 
 *                      compress40.c
 *      Assignment: Arith
 *      Authors: Mishona Horton and Perucy Mussiba
 *      Date: 10/17/2024
 *      Purpose: This file implements the image compression and
 *              decompression functions
 * 
 ***********************************************************************/
#include "compress40.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "rgb_to_video.h"
#include "videocs_to_word.h"
#include "assert.h"
#include "uarray2.h"
#include "bitpack.h"
#include "imageprocessor.h"
#include <stdint.h>
#include <stdbool.h>
/**************************compress40********************************
 * 
 * Parameters: 
 *      FILE *input: A file pointer to an input file or stdin
 * 
 * Return: 
 *      None
 * 
 * Expects: valid input file pointer
 * 
 * Notes: compress40 calls the compression functions that transform a PPM
 *      image to a compressed binary image
 * 
 * *******************************************************************/
void compress40 (FILE *input) {

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        /*ppm image from input file*/
        Pnm_ppm image = readppmimage(input, methods);
  
        /*rgb pixels to video component color space*/
        A2Methods_UArray2 video_cs = rgb_to_videocs(image, methods, map);
        
        /*from component video color space to cosine coeff a,b,c,d & pb, pr*/
        A2Methods_UArray2 bit_word = vcs_to_word(video_cs, methods, map);
        
        /*32-bit word packing*/
        A2Methods_UArray2 pack_word = word_to_codedword(bit_word, methods, map);
   
        /*printing to stdout*/
        print_compressedimg(pack_word, methods);
        
        Pnm_ppmfree(&image);
        methods->free(&video_cs);
        methods->free(&bit_word);
        methods->free(&pack_word);
}
/**************************decompress40********************************
 * 
 * Parameters: 
 *      FILE *input: A file pointer to an input file or stdin
 * 
 * Return: 
 *      None
 * 
 * Expects: valid input file pointer
 * 
 * Notes: decompress40 calls the compression functions that transform a 
 *      compressed binary image to a PPM image.
 * 
 * *******************************************************************/
void decompress40(FILE *input) {

        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);
        
        /*reading the compressed file into an array of 32-bit code words*/
        A2Methods_UArray2 coded_arr = code_word(input, methods);
        
        /*coded word to an uncoded word*/
        A2Methods_UArray2 word_arr = codedword_to_word(coded_arr, methods, map);

        /*uncoded word to component video color space pixels*/
        A2Methods_UArray2 vcs_arr = word_to_vcs(word_arr, methods, map);

        /*component video color space to RGB pixels*/
        A2Methods_UArray2 rgb_pix = vidcs_to_rgb(vcs_arr, methods, map);
        
        /*initializing a Pnm_ppm image*/
        Pnm_ppm finalpix = malloc(sizeof(*finalpix));
        assert(finalpix != NULL);

        finalpix->methods = methods;
        finalpix->pixels = rgb_pix;
        finalpix->denominator = 255;
        finalpix->width = (unsigned) methods->width(rgb_pix);
        finalpix->height = (unsigned) methods->height(rgb_pix);
        
        Pnm_ppmwrite(stdout, finalpix);
        
        methods->free(&coded_arr);
        methods->free(&word_arr);
        methods->free(&vcs_arr);
        Pnm_ppmfree(&finalpix);
}