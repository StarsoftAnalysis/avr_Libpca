#ifndef __COMMON_H__
#define __COMMON_H__

/**
 * @file common.h 
 *
 * @brief common macros and definitions
 *
 * This file provides some general purpose data types definitions, macros and routines
 *
 */


/**
 * @brief provide a brief calculation of the memory usage and report how much is left
 *
 * @return number of bytes available
 */
uint16_t common_memory_left();


/**
 * @brief performs ordered dithering using bayer 8x8 matrix of 6 bit depth color (0-63) to 1 bit.
 *
 * @param c color (must be scaled to range 0-63)
 * @param x x coordinate
 * @param y y coordinate
 *
 * @return true if point should be drawned, flase if otherwise
 */
uint8_t common_ditherPoint(uint8_t c, uint8_t x, uint8_t y);


/**
 * @brief exchange byte order in a 32 bit word
 *
 * @param a_value
 *
 * @return 
 */
uint32_t common_swap(uint32_t a_value);

#endif /* __COMMON_H__ */
