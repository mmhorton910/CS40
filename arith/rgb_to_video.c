/***********************************************************************
 * 
 *                      rgb_to_video.c
 *      Assignment: Arith
 *      Authors: Mishona Horton and Perucy Mussiba
 *      Date: 10/17/2024
 *      Purpose: This file implements conversion of PPM image rgb pixels
 *              to a component video color space and viceversa
 * 
 ***********************************************************************/

#include "rgb_to_video.h"
#define DENOMINATOR 255
/**********************get_range******************************
 * 
 * Parameters:
 *      float value: value to check if is in the low to high range
 *      float low: low limit of the allowed range
 *      float high: high limit of the allowed range
 * 
 * Return: 
 *      returns a value in the specified range
 * 
 * Expects: valid value, low and high floats
 * 
 * Notes: get_range determines if y, pb and pr are in the required
 *      range
 * 
 *******************************************************************/
static float get_range(float value, float low, float high) {
        if (value < low) {
                return low;
        } else if (value > high) {
                return high;
        }
        return value;
}
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
A2Methods_UArray2 rgb_to_videocs(Pnm_ppm image, A2Methods_T methods,
        A2Methods_mapfun *map)
{
        int width = methods->width(image->pixels);
        int height = methods->height(image->pixels);

        A2Methods_UArray2 arr = methods->new(width, height, 
                sizeof(struct color_space));

        a2_cl cl = malloc(sizeof(struct a2_cl));
        assert(cl != NULL);
        
        cl->methods = methods;
        cl->array = arr;
        cl->denominator = image->denominator;

        map(image->pixels, transform_rgbpixels, cl);

        if(cl != NULL) {
                free(cl);
                cl = NULL;
        }

        return arr;
}
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
void transform_rgbpixels(int col, int row, A2Methods_UArray2 arr, void *elem, 
        void *cl)
{
        (void) arr;
        a2_cl temp = cl;
        float y, pb, pr;

        /*getting the rgb pixels*/
        Pnm_rgb pixel = elem;
        unsigned int r = pixel->red;
        unsigned int g = pixel->green;
        unsigned int b = pixel->blue;

        /*getting y, pb and pr*/
        color_space temp_cs =  temp->methods->at(temp->array, col, row);

        y = (0.299 * r + 0.587 * g + 0.114 * b) /temp->denominator;
        temp_cs->y = get_range(y, 0, 1);
        pb = (-0.168736 * r - 0.331264 * g + 0.5 * b) /temp->denominator;
        temp_cs->pb = get_range(pb, -0.5, 0.5);
        pr = (0.5 * r - 0.418688 * g - 0.081312 * b) /temp->denominator;
        temp_cs->pr = get_range(pr, -0.5, 0.5);

}
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
A2Methods_UArray2 vidcs_to_rgb(A2Methods_UArray2 video_cs, A2Methods_T methods,
        A2Methods_mapfun *map)
{
        int width = methods->width(video_cs);
        int height = methods->height(video_cs);

        A2Methods_UArray2 arr = methods->new(width, height, 
                sizeof(struct Pnm_rgb));
        
        a2_cl cl = malloc(sizeof(struct a2_cl));
        assert(cl != NULL);

        cl->methods = methods;
        cl->array = arr;
        cl->denominator = DENOMINATOR;

        map(video_cs, transform_vcspixels, cl);

        if(cl != NULL) {
                free(cl);
                cl = NULL;
        }

        return arr;

}
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
void transform_vcspixels(int col, int row, A2Methods_UArray2 arr, void *elem, 
        void *cl)
{
        (void) arr;
        /*cl is a struct of array, denominator and methods for RGB pixels*/
        a2_cl temp = cl;

        /*getting float rgb pixels r, g, b*/
        color_space cs_pix = elem;
        float r, g, b;
        r = 1.0 * cs_pix->y + 0.0 * cs_pix->pb + 1.402 * cs_pix->pr;
        g = 1.0 * cs_pix->y - 0.344136 * cs_pix->pb - 0.714136 * cs_pix->pr;
        b = 1.0 * cs_pix->y + 1.772 * cs_pix->pb + 0.0 * cs_pix->pr;
        
        /*populating 2d array of rgb pixels at a given col and row*/
        Pnm_rgb rgb_pix = temp->methods->at(temp->array, col, row);
        r = get_range(r, 0.0, 1.0);
        rgb_pix->red = (unsigned) (r * temp->denominator);
        g = get_range(g, 0.0, 1.0);
        rgb_pix->green = (unsigned) (g * temp->denominator);
        b = get_range(b, 0.0, 1.0);
        rgb_pix->blue = (unsigned) (b * temp->denominator);

}
