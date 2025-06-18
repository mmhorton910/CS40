/***********************************************************************
 * 
 *                      videocs_to_word.c
 *      Assignment: Arith
 *      Authors: Mishona Horton and Perucy Mussiba
 *      Date: 10/17/2024
 *      Purpose: This file implements conversion of component video color
 *              space pixels to compressed binary image pixels
 * 
 ***********************************************************************/
#include "videocs_to_word.h"

/**************************bcd_range********************************
 * 
 * Parameters:
 *      float val: a float to be checked if in the range [-0.3, 0.3]
 * 
 * Return: 
 *      a float in the specified range for b, c, d values
 * 
 * Expects: valid float
 * 
 * Notes: bcd_range checks if the float is in the specified range. If so,
 *      the value is returned otherwise, -0.3 or 0.3 is returned depending
 *      on which is close
 * 
 * *******************************************************************/
static float bcd_range(float val) {
        if (val > 0.3) {
                return 0.3;
        } else if (val < -0.3) {
                return -0.3;
        }
        return val;
}

/**************************a_range********************************
 * 
 * Parameters:
 *      float val: a float to be checked if in the range [0.0, 1.0]
 * 
 * Return: 
 *      a float in the specified range for a values
 * 
 * Expects: valid float
 * 
 * Notes: a_range checks if the float is in the specified range. If so,
 *      the value is returned otherwise, 0.0 or 1.0 is returned depending
 *      on which is close
 * 
 * *******************************************************************/
static float a_range(float val) {
        if (val > 1) {
                return 1.0;
        } else if (val < 0) {
                return 0.0;
        }
        return val;
}

/**************************a_float_to_int********************************
 * 
 * Parameters:
 *      float val: a float to be converted to uint64_t
 * 
 * Return: 
 *      a uint64_t value of float val
 * 
 * Expects: valid float
 * 
 * Notes: a_float_to_int converts a float value of a to uint64_t by
 *       multiplying it with a floating-point literal 511.0.We chose
 *      511 because it is the maximum value of a in bits for a width of  9
 * 
 * *******************************************************************/
static uint64_t a_float_to_int(float val) {
        return (uint64_t)(val * 511.0);
}

/**************************a_int_to_float********************************
 * 
 * Parameters:
 *      uint64_t val: a uint64_t val to be converted to a float
 * 
 * Return: 
 *      a uint64_t value of float val
 * 
 * Expects: valid uint64_t
 * 
 * Notes: a_float_to_int converts uint64_t to a float by
 *       dividing it with a floating-point literal 511.0. We chose
 *      511 because it is the maximum value of a in bits for a width of  9
 * 
 * *******************************************************************/
static float a_int_to_float(uint64_t val) {
        return ((float) val) / ((float) 511.0);
}

/**************************bcd_float_to_int********************************
 * 
 * Parameters:
 *      float val: a float to be converted to int64_t
 * 
 * Return: 
 *      a int64_t value of float val
 * 
 * Expects: valid float
 * 
 * Notes: bcd_float_to_int converts a float value of a to int64_t by
 *       multiplying it with a floating-point literal 31.0. We chose
 *      31 because it is the maximum value of b, c, d in bits for a width of 5.
 *      Before conversion b, c, d values are put in the range [-0.3, 0.3]
 * 
 * *******************************************************************/
static int64_t bcd_float_to_int(float val) {
        val = bcd_range(val);
        return (int64_t)(val * 31.0);
}

/**************************bcd_int_to_float********************************
 * 
 * Parameters:
 *      int64_t val: a int64_t val to be converted to a float
 * 
 * Return: 
 *      a uint64_t value of float val
 * 
 * Expects: valid uint64_t
 * 
 * Notes: bcd_int_to_float converts int64_t to a float by
 *       dividing it with a floating-point literal 31.0. We chose
 *      31 because it is the maximum value of b, c, d in bits for a width of 5.
 * 
 * *******************************************************************/
static float bcd_int_to_float(int64_t val) {
        return ((float) val) / ((float)31.0);
}
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
A2Methods_UArray2 vcs_to_word(A2Methods_UArray2 video_cs, 
        A2Methods_T methods, A2Methods_mapfun *map)
{
        /*since we are taking 4 values as in a box then the width
        and height of new array for bitword are halved*/
        int width = (methods->width(video_cs)) / 2;
        int height = (methods->height(video_cs)) / 2;

        A2Methods_UArray2 word_arr = methods->new(width, height, 
                sizeof(struct bitword));

      
        bit_cl cl = malloc(sizeof(struct bit_cl));
        assert(cl != NULL);
        cl->array = video_cs;
        cl->methods = methods;

        map(word_arr, trans_vcs_word, cl);

        if(cl != NULL) {
                free(cl);
                cl = NULL;
        }
        
        return word_arr; 
}
/**************************trans_vcs_word********************************
 * 
 * Parameters:
 *      int col: col index of UArray_2 word_arr (uncoded words) 
 *      int row: row index of UArray_2 word_arr (uncoded words)
 *      A2Methods_UArray2 arr: 2D array containing struct of uncoded word
 *                              a, b, c, d, avpb and avpr
 *      void *elem: Component video color space pixels
 *      void *cl: a struct containing methods, and a UArray_2 for
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
void trans_vcs_word(int col, int row, A2Methods_UArray2 arr, void *elem, 
        void *cl) 
{
        (void) arr;
        color_space cs1, cs2, cs3, cs4;
        float f_a, f_b, f_c, f_d;

        bitword bit = elem;
        bit_cl m_bitcl = cl;

        /*indices to be visited in a 2 by 2 block*/
        int c_col = 2 * col;
        int c_row = 2 * row;

        /*getting the 2 by 2 block color space pixels values*/
        cs1 = m_bitcl->methods->at(m_bitcl->array, c_col, c_row);
        cs2 = m_bitcl->methods->at(m_bitcl->array, c_col + 1, c_row);
        cs3 = m_bitcl->methods->at(m_bitcl->array, c_col, c_row + 1);
        cs4 = m_bitcl->methods->at(m_bitcl->array, c_col + 1, c_row + 1);

        f_a = (cs4->y + cs3->y + cs2->y + cs1->y)/4.0;
        f_b = (cs4->y + cs3->y - cs2->y - cs1->y)/4.0;
        f_c = (cs4->y - cs3->y + cs2->y - cs1->y)/4.0;
        f_d = (cs4->y - cs3->y - cs2->y + cs1->y)/4.0;

        float a_pb = (cs1->pb + cs2->pb + cs3->pb+ cs4->pb) / 4;
        float a_pr = (cs1->pr + cs2->pr + cs3->pr+ cs4->pr) / 4;

        bit->av_pb = (uint64_t) (Arith40_index_of_chroma(a_pb));
        bit->av_pr = (uint64_t) (Arith40_index_of_chroma(a_pr));    
        bit->a = a_float_to_int(f_a);
        bit->b = bcd_float_to_int(f_b);
        bit->c = bcd_float_to_int(f_c);
        bit->d = bcd_float_to_int(f_d);

}
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
A2Methods_UArray2 word_to_vcs(A2Methods_UArray2 word_arr, 
        A2Methods_T methods, A2Methods_mapfun *map)
{
        int width = (methods->width(word_arr)) * 2;
        int height = (methods->height(word_arr)) * 2;

        A2Methods_UArray2 vcs_arr = methods->new(width, height, 
                sizeof(struct color_space));
       
        bit_cl cl = malloc(sizeof(struct bit_cl));
        assert(cl != NULL);
        cl->array = vcs_arr;
        cl->methods = methods;

        map(word_arr, transform_word, cl);

        if(cl != NULL) {
                free(cl);
                cl = NULL;
        }

        return vcs_arr;  
}
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
void transform_word(int col, int row, A2Methods_UArray2 arr, void *elem, 
        void *cl)
{
        (void) arr;
        bitword bt = elem; 
        bit_cl m_cl = cl; 

        int idx_col = col * 2;
        int idx_row = row * 2;

        float a, b, c, d, pb, pr;
        a = a_int_to_float(bt->a);
        b = bcd_int_to_float(bt->b);
        c = bcd_int_to_float(bt->c);
        d = bcd_int_to_float(bt->d);

        pb = Arith40_chroma_of_index(bt->av_pb);
        pr = Arith40_chroma_of_index(bt->av_pr);

        color_space vcs1 = m_cl->methods->at(m_cl->array, idx_col, idx_row);
        color_space vcs2 = m_cl->methods->at(m_cl->array, idx_col + 1, idx_row);
        color_space vcs3 = m_cl->methods->at(m_cl->array, idx_col, idx_row + 1);
        color_space vcs4 = m_cl->methods->at(m_cl->array, idx_col + 1, 
                idx_row + 1);

        vcs1->pb = pb;
        vcs1->pr = pr;
        vcs2->pb = pb;
        vcs2->pr = pr;        
        vcs3->pb = pb;
        vcs3->pr = pr;
        vcs4->pb = pb;
        vcs4->pr = pr; 

        vcs1->y = a_range(a - b - c + d);
        vcs2->y = a_range(a - b + c - d);
        vcs3->y = a_range(a + b - c - d);
        vcs4->y = a_range(a + b + c + d);
}
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
 *      deals with converting uncoded word to coded word using DCT 
 *      functions
 * 
 * *******************************************************************/
A2Methods_UArray2 word_to_codedword(A2Methods_UArray2 word_arr, 
        A2Methods_T methods, A2Methods_mapfun *map)
{
        int width = methods->width(word_arr);
        int height = methods->height(word_arr);

        A2Methods_UArray2 coded_arr = methods->new(width, height, 
                sizeof(uint64_t));

       
        bit_cl cl = malloc(sizeof(struct bit_cl));
        assert(cl != NULL);
        cl->array = coded_arr;
        cl->methods = methods;

        map(word_arr, bitword_packing, cl);

        if(cl != NULL) {
                free(cl);
                cl = NULL;
        }
        
        return coded_arr; 
}
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
void bitword_packing(int col, int row, A2Methods_UArray2 arr, void *elem, 
        void *cl)
{
        (void) arr;
        bit_cl mbit_cl = cl; 
        bitword bit = elem; 

        uint64_t *pack_word = mbit_cl->methods->at(mbit_cl->array, col, row);

        /*packing a, b, c, d, avpb and avpr*/
        unsigned bcd_width = 5;
        unsigned a_width = 9;
        unsigned pba_width = 4;
        unsigned a_lsb = 23;
        unsigned b_lsb = 18;
        unsigned c_lsb = 13;
        unsigned d_lsb = 8;
        unsigned pb_lsb = 4;
        unsigned pr_lsb = 0;
        uint64_t word = 0;
        word = Bitpack_newu(word, a_width, a_lsb, bit->a);
        word = Bitpack_news(word, bcd_width, b_lsb, bit->b);
        word = Bitpack_news(word, bcd_width, c_lsb, bit->c);
        word = Bitpack_news(word, bcd_width, d_lsb, bit->d);
        word =  Bitpack_newu(word, pba_width, pb_lsb, bit->av_pb);
        word = Bitpack_newu(word, pba_width, pr_lsb, bit->av_pr);

        *pack_word = word;
}
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
        A2Methods_T methods, A2Methods_mapfun *map)
{
        int width = methods->width(coded_arr);
        int height = methods->height(coded_arr);

       
        A2Methods_UArray2 word_arr = methods->new(width, height, 
                sizeof(struct bitword));

        bit_cl cl = malloc(sizeof(struct bit_cl));
        assert(cl != NULL);
        cl->array = coded_arr;
        cl->methods = methods;
        
        map(word_arr, bitword_unpacking, cl);

        if(cl != NULL) {
                free(cl);
                cl = NULL;
        }
        
        return word_arr; 
}
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
void bitword_unpacking(int col, int row, A2Methods_UArray2 arr, void *elem, 
        void *cl)
{
        (void) arr;
        bit_cl mbit_cl = cl; 
        bitword bit = elem; 
        
        uint64_t *code_word = mbit_cl->methods->at(mbit_cl->array, col, row);
        uint64_t word = *code_word;

        unsigned bcd_width = 5;
        unsigned a_width = 9;
        unsigned pba_width = 4;
        unsigned a_lsb = 23;
        unsigned b_lsb = 18;
        unsigned c_lsb = 13;
        unsigned d_lsb = 8;
        unsigned pb_lsb = 4;
        unsigned pr_lsb = 0;
      
        bit->a = Bitpack_getu(word, a_width, a_lsb);
        bit->b = Bitpack_gets(word, bcd_width, b_lsb);
        bit->c = Bitpack_gets(word, bcd_width, c_lsb);
        bit->d = Bitpack_gets(word, bcd_width, d_lsb);
        bit->av_pb =  Bitpack_getu(word, pba_width, pb_lsb);
        bit->av_pr = Bitpack_getu(word, pba_width, pr_lsb);
}
