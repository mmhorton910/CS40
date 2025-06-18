/***********************************************************************
 * 
 *                      bitpack.c
 *      Assignment: Arith
 *      Authors: Mishona Horton and Perucy Mussiba
 *      Date: 10/17/2024
 *      Purpose: This file implements bitpacking and unpacking of integers
 * 
 ***********************************************************************/
#include "bitpack.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "except.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };
/**********************Bitpack_fitsu******************************
 * 
 * Parameters:
 *           uint64_t n: a 64-bit integer
 *           unsigned width: number of bits to hold n
 * 
 * Return: 
 *      returns true if n can be represented in width bits and false otherwise
 * 
 * Expects: a 64-bit unsigned integer and a valid width from 0 to 64
 * 
 * Notes: Bitpack_fitsu checks if n can be represented in width bits.
 *        The function determines the max value that can be represented in
 *        width bits and checks if n is less than  or equal to the max value and
 *        returns true and false otherwise
 * 
 *******************************************************************/
bool Bitpack_fitsu(uint64_t n, unsigned width) {
       assert(width > 0 && width <= 64);
       uint64_t x = 1;
       uint64_t max_val = (x << width) - 1;
       return (n <= max_val);
}
/**********************Bitpack_fitss******************************
 * 
 * Parameters:
 *           int64_t n: a 64-bit integer
 *           unsigned width: number of bits to hold n
 * 
 * Return: 
 *      returns true if n can be represented in width bits and false otherwise
 * 
 * Expects: a 64-bit signed intger and a valid width from 0 to 64
 * 
 * Notes: Bitpack_fitss checks if n can be represented in width bits.
 *        The function determines the max value that can be represented in
 *        width bits and checks if n is less than  or equal to the max value and
 *        returns true and false otherwise
 * 
 *******************************************************************/
bool Bitpack_fitss(int64_t n, unsigned width) {
        assert(width > 0 && width <= 64);
        int64_t x = 1;
        int64_t min_val = -(x << (width - 1));
        int64_t max_val = (x << (width - 1)) - 1;
        return (n >= min_val && n <= max_val);
}
/**********************Bitpack_getu******************************
 * 
 * Parameters:
 *           uint64_t word: a 64-bit word
 *           unsigned width: number of bits to extract from word
 *           unsigned lsb: position of least significant bit to be extracted
 * 
 * Return: 
 *      returns an unsigned value of a given width extracted from word
 * 
 * Expects: a 64-bit unsigned integer, a valid width from 0 to 64 and lsb
 * 
 * Notes: Bitpack_getu retrieves a field of a specified width and lsb 
 *      The function will CRE when width or width + lsb is invalid
 * 
 *******************************************************************/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) {
        assert((width <= 64) && (width + lsb <= 64));

        if (width == 0) {
                return 0;
        }
        uint64_t x = 1;
        uint64_t val = (x << width) - 1;
        word = (word >> lsb) & val;
        return word;

}
/**********************Bitpack_gets******************************
 * 
 * Parameters:
 *           int64_t word: a 64-bit word
 *           unsigned width: number of bits to extract from word
 *           unsigned lsb: position of least significant bit to be extracted
 * 
 * Return: 
 *      returns a signed value of a given width extracted from word
 * 
 * Expects: a 64-bit unsigned integer, a valid width from 0 to 64 and lsb
 * 
 * Notes: Bitpack_gets retrieves a field of a specified width and lsb 
 *      The function will CRE when width or width + lsb is invalid
 * 
 *******************************************************************/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) {
        assert((width <= 64) && (width + lsb <= 64));

        if (width == 0) {
                return 0;
        }
        uint64_t u_val = Bitpack_getu(word, width, lsb);
        uint64_t x = 1;
        if (u_val >> (width - 1)) {
                uint64_t val = (x << width) - 1;
                u_val |= ~val;
        }
        return (int64_t)u_val;

 }
 /**********************Bitpack_newu******************************
 * 
 * Parameters:
 *           uint64_t word: a 64-bit word
 *           unsigned width: number of bits to extract from word
 *           unsigned lsb: position of least significant bit to be updated
 *           uint64_t value: a value to be added or included in word
 * 
 * Return: 
 *      returns an updated word containing value
 * 
 * Expects: a 64-bit unsigned integr word, value, unsigned width and lsb
 * 
 * Notes: Bitpack_newu updates a field of specified width and lsb in word by 
 *      incorporating value in it. The function will CRE when width or width + 
 *      lsb is invalid or bit overflow occurs
 * 
 *******************************************************************/
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
        uint64_t value)
{
        assert((width <= 64) && (width + lsb <= 64));
        
        if (Bitpack_fitsu(value, width) == 0) {
                RAISE(Bitpack_Overflow);
        }

        uint64_t x = 1;
        uint64_t val = ((x << width) - 1) << lsb;
        word = word & (~val);
        word = word | (value << lsb);

        return word; 
}
 /**********************Bitpack_news******************************
 * 
 * Parameters:
 *           uint64_t word: a 64-bit word
 *           unsigned width: number of bits to extract from word
 *           unsigned lsb: position of least significant bit to be updated
 *           int64_t value: a value to be added or included in word
 * 
 * Return: 
 *      returns an updated word containing value
 * 
 * Expects: a 64-bit unsigned integer word, signed value, unsigned width and lsb
 * 
 * Notes: Bitpack_news updates a field of specified width and lsb in word by 
 *      incorporating value in it. The function will CRE when width or width + 
 *      lsb is invalid or bit overflow occurs
 * 
 *******************************************************************/
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  
        int64_t value) 
{
        assert((width <= 64) && (width + lsb <= 64));
        if (Bitpack_fitss(value, width) == 0) {
                RAISE(Bitpack_Overflow);
        }
        uint64_t x = 1;
        uint64_t val = ((x << width) - 1) << lsb;
        word = word & (~val);

        uint64_t new_value = ((uint64_t) value & ((x << width) - 1)) << lsb;

        return word | new_value;

}

