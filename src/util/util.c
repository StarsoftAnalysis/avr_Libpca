#include "util/util.h"

#include <stdint.h>


uint32_t util_abs(int32_t a_value) {
	int32_t temp = 0x00;
	temp = a_value >> 31;
	a_value ^= temp;
	a_value -= temp;
	return a_value;
}
