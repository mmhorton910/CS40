/***********************************************************************
 * 
 *                      rgb_to_video.h
 *      Assignment: Arith
 *      Authors: Mishona Horton and Perucy Mussiba
 *      Date: 10/17/2024
 *      Purpose: This file contains function and struct definitions for 
 *              rgb_to_video.c 
 * 
 ***********************************************************************/


#ifndef RGB_TO_VIDEO_INCLUDED
#define RGB_TO_VIDEO_INCLUDED
#include "a2methods.h"
#include "pnm.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include "a2blocked.h"

/*struct to hold the component video color space*/
typedef struct color_space {
        float y;
        float pb;
        float pr;
} *color_space;

/*struct passed as a closure in the mapping functions
 it contains a UArray_2 array, methods and denominator*/
typedef struct a2_cl {
        A2Methods_UArray2 array;
        A2Methods_T methods;
        unsigned int denominator;
} *a2_cl;

/**************************rgb_to_videocs********************************
 * 
 * Parameters: 
 *      Pnm_ppm image: ppm image to be compressed
 *      A2_Methods_T methods: methods for UArray_2 operations
 *      A2Methods_mapfun *map: maps for UArray_2
 * 
 * Return: 
 *      returns a UArray_2 video_cs whose elements are structs containing
 *       the component video color space variables
 * 
 * Expects: valid ppm image, methods, and map
 * 
 * Notes: CRE is raised when malloc fails. This is the main function that
 *      deals with converting Pnm_rgb  pixels to the component video
 *      color space
 * 
 * *******************************************************************/
extern A2Methods_UArray2 rgb_to_videocs(Pnm_ppm image, A2Methods_T methods,
        A2Methods_mapfun *map);

/**************************transform_rgbpixels********************************
 * 
 * Parameters:
 *      int col: col index of Pnm_rgb pixel to be transformed 
 *      int row: row index of Pnm_rgb pixel to be transformed
 *      A2Methods_UArray2 arr: 2D array containing Pnm_rgb pixels
 *      void *elem: Pnm_rgb pixel
 *      void *cl: a struct containing methods, denominator and a UArray_2 for
 *              component video color space
 * 
 * Return: 
 *      None
 * 
 * Expects: valid col, row, arr, elem and cl
 * 
 * Notes: CRE is raised when invalid col and row is used to access
 *         elements in the 2d array. The function computes the y, pb and pr
 *      values and calls a helper function that checks if they are in the 
 *      required range
 * 
 * *******************************************************************/
extern void transform_rgbpixels(int col, int row, A2Methods_UArray2 arr, 
        void *elem, void *cl);

/**************************vidcs_to_rgb********************************
 * 
 * Parameters: 
 *      A2Methods_UArray2 video_cs: UArray2 whose elements are structs 
 *        containing the component video color space variables
 *      A2_Methods_T methods: methods for UArray_2 operations
 *      A2Methods_mapfun *map: maps for UArray_2
 * 
 * Return: 
 *      returns a UArray_2 whose elements are structs containing the
 *        component rgb values
 * 
 * Expects: UArray_2 whose elements are color_space structs, methods, and 
*           map
 * 
 * Notes: CRE is raised when malloc fails. This is the main function that
 *      deals with converting the component video color space to Pnm_rgb  pixels
 * 
 * *******************************************************************/
extern A2Methods_UArray2 vidcs_to_rgb(A2Methods_UArray2 video_cs, 
        A2Methods_T methods, A2Methods_mapfun *map);

/**************************transform_vcspixels********************************
 * 
 * Parameters:
 *      int col: col index of color_space pixel to be transformed 
 *      int row: row index of color_space pixel to be transformed
 *      A2Methods_UArray2 arr: 2D array containing color_space pixels
 *      void *elem: color_space pixel
 *      void *cl: a struct containing methods, denominator and a UArray_2 for
 *              component video color space
 * 
 * Return: 
 *      None
 * 
 * Expects: valid col, row, arr, elem and cl
 * 
 * Notes: CRE is raised when invalid col and row is used to access
 *         elements in the 2d array. The function computes the Pnm_rgb pixels
 *      from the video component color space and calls function that checks 
 *      if they are in the required range
 * 
 * *******************************************************************/
extern void transform_vcspixels(int col, int row, A2Methods_UArray2 arr, 
        void *elem, void *cl);

#endif
