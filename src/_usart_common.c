#include "_usart_common.h"


uint16_t usart_common_calculate_ubrr(uint32_t clock, uint32_t baud, uint8_t mode) {
    uint32_t x = clock*(16 >> mode);
    uint32_t y = (baud << 4);
    return ( (float)x/y - (x/y) > 0.5f ? (x - 1)/y : (x - y)/y );
}


uint32_t usart_common_calculate_baud(uint32_t clock, uint16_t ubrr, uint8_t mode) {
    return clock/((16 >> mode)*(ubrr + 1));
}
