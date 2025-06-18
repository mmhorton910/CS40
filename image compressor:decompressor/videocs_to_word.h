/***********************************************************************
 * 
 *                      videocs_to_word.h
 *      Assignment: Arith
 *      Authors: Mishona Horton and Perucy Mussiba
 *      Date: 10/17/2024
 *      Purpose: This file contains function declarations for 
 *              videocs_to_word.c
 * 
 ***********************************************************************/
#ifndef VIDEOCS_TO_WORD_INCLUDED
#define VIDEOCS_TO_WORD_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "assert.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "arith40.h"
#include "rgb_to_video.h"
#include "bitpack.h"

/*struct containing variables for an uncoded word made of cosine coefficients*/
typedef struct bitword {
        uint64_t a;
        int64_t b;
        int64_t c;
        int64_t d;
        uint64_t av_pb;
        uint64_t av_pr;
} *bitword;

/*struct passed as a closure in the mapping or apply functions*/
typedef struct bit_cl {
        A2Methods_UArray2 array;
        A2Methods_T methods;
} *bit_cl;

/**************************vcs_to_word********************************
 * 
 * Parameters: 
 *      A2Methods_UArray2 video_cs: 2d array of component video color space
 *      A2_Methods_T methods: methods for UArray_2 operations
 *      A2Methods_mapfun *map: maps for UArray_2
 * 
 * Return: 
 *      returns a UArray_2 of uncoded words containing structs with variables
 *      a, b, c, d, avpb and avpr
 * 
 * Expects: valid 2d UArray, methods, and map
 * 
 * Notes: CRE is raised when malloc fails. This is the main function that
 *      deals with converting component video color space to uncoded word
 * 
 * *******************************************************************/
extern A2Methods_UArray2 vcs_to_word(A2Methods_UArray2 video_cs, 
        A2Methods_T methods, A2Methods_mapfun *map);

/**************************trans_vcs_word********************************
 * 
 * Parameters:
 *      int col: col index of UArray_2 word_arr (uncoded words) 
 *      int row: row index of UArray_2 word_arr (uncoded words)
 *      A2Methods_UArray2 arr: 2D array containing struct of uncoded word
 *                              a, b, c, d, avpb and avpr
 *      void *elem: Component video color space pixels
 *      void *cl: a struct containing methods, denominator and a UArray_2 for
 *              component video color space
 * 
 * Return: 
 *      None
 * 
 * Expects: valid col, row, arr, elem and cl
 * 
 * Notes: CRE is raised when invalid col and row is used to access
 *         elements in the 2d array. The function computes the a, b, c, d
 *         avpb and avpr for the uncoded word
 * 
 * *******************************************************************/
extern void trans_vcs_word(int col, int row, A2Methods_UArray2 arr, 
        void *elem, void *cl);

/**************************word_to_codedword********************************
 * 
 * Parameters: 
 *      A2Methods_UArray2 word_arr: 2d array of uncoded words
 *      A2_Methods_T methods: methods for UArray_2 operations
 *      A2Methods_mapfun *map: maps for UArray_2
 * 
 * Return: 
 *      returns a UArray_2 of coded words whose elements are unsigned 64-bit 
 *      integers
 * 
 * Expects: valid 2d UArray, methods, and map
 * 
 * Notes: CRE is raised when malloc fails. This is the main function that
 *      deals with converting coded word to uncoded word using DCT 
 *      functions
 * 
 * *******************************************************************/
extern A2Methods_UArray2 word_to_codedword(A2Methods_UArray2 word_arr, 
        A2Methods_T methods, A2Methods_mapfun *map);

/**************************bitword_packing********************************
 * 
 * Parameters:
 *      int col: col index of UArray_2 word_arr (uncoded words) 
 *      int row: row index of UArray_2 word_arr (uncoded words)
 *      A2Methods_UArray2 arr: 2D array containing struct of uncoded word
 *                              a, b, c, d, avpb and avpr
 *      void *elem: struct of uncoded word a, b, c, d, avpb and avpr
 *      void *cl: a struct containing methods, and a UArray_2 of uint64_t
 *              integers (coded words)
 * 
 * Return: 
 *      None
 * 
 * Expects: valid col, row, arr, elem and cl
 * 
 * Notes: CRE is raised when invalid col and row is used to access
 *         elements in the 2d array. The function calls bitpacking field
 *         update functions to pack bits
 * 
 * *******************************************************************/
extern void bitword_packing(int col, int row, A2Methods_UArray2 arr, 
        void *elem, void *cl);

/**************************codedword_to_word********************************
 * 
 * Parameters: 
 *      A2Methods_UArray2 coded_arr: 2d array of coded words
 *      A2_Methods_T methods: methods for UArray_2 operations
 *      A2Methods_mapfun *map: maps for UArray_2
 * 
 * Return: 
 *      returns a UArray_2 of uncoded words whose elements are structs 
 *      containing a bitword with a, b, c, d, avpb and avpr
 * 
 * Expects: valid 2d UArray, methods, and map
 * 
 * Notes: CRE is raised when malloc fails. This is the main function that
 *      deals with converting coded word to uncoded word using inverse DCT 
 *      functions
 * 
 * *******************************************************************/
A2Methods_UArray2 codedword_to_word(A2Methods_UArray2 coded_arr, 
        A2Methods_T methods, A2Methods_mapfun *map);

/**************************bitword_unpacking********************************
 * 
 * Parameters:
 *      int col: col index of UArray_2 word_arr (uncoded words) 
 *      int row: row index of UArray_2 word_arr (uncoded words)
 *      A2Methods_UArray2 arr: 2D array containing struct of uncoded word
 *                              a, b, c, d, avpb and avpr
 *      void *elem: struct of uncoded word a, b, c, d, avpb and avpr
 *      void *cl: a struct containing methods, and a UArray_2 of uint64_t
 *              integers (coded words)
 * 
 * Return: 
 *      None
 * 
 * Expects: valid col, row, arr, elem and cl
 * 
 * Notes: CRE is raised when invalid col and row is used to access
 *         elements in the 2d array. The function calls bitpacking field
 *         extraction functions to unpacking bits
 * 
 * *******************************************************************/
extern void bitword_unpacking(int col, int row, A2Methods_UArray2 arr, 
        void *elem, void *cl);

/**************************word_to_vcs********************************
 * 
 * Parameters: 
 *      A2Methods_UArray2 word_arr: 2d array of uncoded words
 *      A2_Methods_T methods: methods for UArray_2 operations
 *      A2Methods_mapfun *map: maps for UArray_2
 * 
 * Return: 
 *      returns a UArray_2 of component video colorspace containing structs
 *      whose variables are y, pb and pr
 * 
 * Expects: valid 2d UArray, methods, and map
 * 
 * Notes: CRE is raised when malloc fails. This is the main function that
 *      deals with converting uncoded word to component video color space
 * 
 * *******************************************************************/
extern A2Methods_UArray2 word_to_vcs(A2Methods_UArray2 word_arr, 
        A2Methods_T methods, A2Methods_mapfun *map);

/**************************transform_word********************************
 * 
 * Parameters:
 *      int col: col index of UArray_2 word_arr (uncoded words) 
 *      int row: row index of UArray_2 word_arr (uncoded words)
 *      A2Methods_UArray2 arr: 2D array containing struct of uncoded word
 *                              a, b, c, d, avpb and avpr
 *      void *elem: struct of uncoded word a, b, c, d, avpb and avpr
 *      void *cl: a struct containing methods, and a UArray_2 for
 *              component video color space
 * 
 * Return: 
 *      None
 * 
 * Expects: valid col, row, arr, elem and cl
 * 
 * Notes: CRE is raised when invalid col and row is used to access
 *         elements in the 2d array. The function computes the y, pb and pr
 *         in a 2 by 2 block
 * 
 * *******************************************************************/
extern void transform_word(int col, int row, A2Methods_UArray2 arr, void *elem, 
        void *cl);
#endif