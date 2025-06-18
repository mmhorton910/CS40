/***********************************************************************
 * 
 *                      ppmdiff.c
 *      Assignment: Arith
 *      Authors: Mishona Horton and Perucy Mussiba
 *      Date: 10/20/2024
 *      Purpose: This file implements computing the root mean square 
 *              difference between the pixels of two ppm images 
*               using the formula given in the ppmdiff spec.
 * 
 ***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"

double calculate_sum(Pnm_ppm image_1, Pnm_ppm image_2, int col, int row);
int main(int argc, char*  argv[]) {
        (void) argc;
        FILE *fp_1 = (strcmp(argv[1], "-") == 0) ? stdin : fopen(argv[1], "r");
        FILE *fp_2 = (strcmp(argv[2], "-") == 0) ? stdin : fopen(argv[2], "r");
        if (fp_1 == NULL || fp_2 == NULL) {
                fprintf(stderr, "Error opening files\n");
                return 1.0;
        }

        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        Pnm_ppm image_1 = Pnm_ppmread(fp_1,methods);
        Pnm_ppm image_2 = Pnm_ppmread(fp_2,methods);

        unsigned w_diff = (image_1->width > image_2->width) ? 
                image_1->width - image_2->width : 
                image_2->width - image_1->width;
        unsigned h_diff = (image_1->height > image_2->height) ? 
                image_1->height - image_2->height : 
                image_2->height - image_1->height;

        if (w_diff > 1 || h_diff > 1) {
                fprintf(stderr, "Height or Width difference exceed 1\n");
                printf("1.0\n");
        }

        unsigned w = (image_1->width >= image_2->width) ? image_2->width : 
                image_1->width;
        unsigned h = (image_1->height >= image_2->height) ? image_2->height : 
                image_1->height;

        unsigned int row, col;
        unsigned sum = 0;
       
        for(col = 0; col < w; col++){
              
                for(row = 0; row < h; row++){
                    
                        sum += calculate_sum(image_1, image_2, col, row);
                }
        }
        double E = sqrt((double)sum / (3.0 * w * h));
        printf("%.4f\n", E);


}

/**************************calculate_sum********************************
 * 
 * Parameters:
 *      Pnm_ppm image_1: a Pnm_ppm struct representing a ppm image
 *      Pnm_ppm image_2: a Pnm_ppm struct representing a ppm image
 *      int col: col index of current pixel
 *      int row: row index of current pixel
 * 
 * Return: 
 *      the summation in the equation given in the ppmdiff spec
 * 
 * Expects: valid ppm images, valid ints for the col and row
 * 
 * Notes: Implents only summation part of the equation given in the spec
 * 
 * *******************************************************************/
double calculate_sum(Pnm_ppm image_1, Pnm_ppm image_2, int col, int row) {
        int rd_diff = 0;
        int bl_diff = 0;
        int gr_diff = 0;
        int sum = 0;
        Pnm_rgb p1 = image_1->methods->at(image_1->pixels, col, row);
        Pnm_rgb p2 = image_2->methods->at(image_2->pixels, col, row);
        int red1 = p1->red / image_1->denominator;
        int green1 = p1->green / image_1->denominator;
        int blue1 = p1->blue / image_1->denominator;

        int red2 = p2->red / image_2->denominator;
        int green2 = p2->green / image_2->denominator;
        int blue2 = p2->blue / image_2->denominator;

        rd_diff = red1 - red2;
        bl_diff = blue1 - blue2;
        gr_diff = green1 - green2;
        sum = (rd_diff * rd_diff) + (bl_diff * bl_diff) + (gr_diff * gr_diff);
        return sum;
}