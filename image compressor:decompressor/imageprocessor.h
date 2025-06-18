/***********************************************************************
 * 
 *                      imageprocessor.h
 *      Assignment: Arith
 *      Authors: Mishona Horton and Perucy Mussiba
 *      Date: 10/17/2024
 *      Purpose: This file contains function declaration for imageprocessor.c
 * 
 ***********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "pnm.h"
#include "a2methods.h"

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
extern Pnm_ppm readppmimage(FILE *fp, A2Methods_T methods);

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
extern A2Methods_UArray2 code_word(FILE *fp, A2Methods_T methods);

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
extern void print_compressedimg(A2Methods_UArray2 arr, A2Methods_T methods);