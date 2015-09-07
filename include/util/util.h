#ifndef UTIL_H_5C0L98NG
#define UTIL_H_5C0L98NG

#include "util/compiler.h"
#include <stdint.h>


#define UTIL_CONCATENATE(__x, __y) __x##__y
#define UTIL_CONCATENATE_STATIC(__x, __y) UTIL_CONCATENATE(__x,__y)


#define STATIC_ASSERT(__cond) \
    static uint8_t UTIL_CONCATENATE_STATIC(__pca_unused, __LINE__)[2*(__cond) - 1] UNUSED


#endif /* end of include guard: UTIL_H_5C0L98NG */
