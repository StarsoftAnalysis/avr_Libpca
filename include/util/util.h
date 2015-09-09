#ifndef UTIL_H_5C0L98NG
#define UTIL_H_5C0L98NG

#include "util/compiler.h"
#include <stdint.h>


#define UTIL_CONCATENATE(__x, __y) __x##__y
#define UTIL_CONCATENATE_STATIC(__x, __y) UTIL_CONCATENATE(__x,__y)


/**
 * @brief simple static assert implementation.
 *
 * Break compilation if the given condition is false
 */
#define STATIC_ASSERT(__cond) \
    static uint8_t UTIL_CONCATENATE_STATIC(__pca_unused, __LINE__)[2*(__cond) - 1] UNUSED


/**
 * @brief converts binary value to BCD representation
 *
 * @param __val value to be converted
 *
 * @return BCD converted value
 */
#define UTIL_BCD2BIN(__val) \
	((__val & 0x0f) + (__val >> 4) * 10)


/**
 * @brief converts BCD value to binary representation
 *
 * @param __val value to be converted
 *
 * @return binary converted value
 */
#define UTIL_BIN2BCD(__val) \
	(((__val / 10) << 4) + __val % 10)


/**
 * @brief return maximum value out of two given
 *
 * @param __x first value
 * @param __y second value
 *
 * @return __x or __y depending on which is bigger
 */
#define UTIL_MAX(__x, __y) \
	( (__x) > (__y) ? (__x) : (__y))


/**
 * @brief return minimum value out of two given
 *
 * @param __x first value
 * @param __y second value
 *
 * @return __x or __y depending on which is smaller
 */
#define UTIL_MIN(__x, __y) \
	( (__x) < (__y) ? (__x) : (__y))


/**
 * @brief Very simple implementation of abs()
 *
 * @param a_value signed value
 *
 * @return absolute value
 */
uint32_t util_abs(int32_t a_value);


#endif /* end of include guard: UTIL_H_5C0L98NG */
