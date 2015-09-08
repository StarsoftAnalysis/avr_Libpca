#include "usart.h"

#include "util/mem.h"
#include "util/util.h"

#include <avr/power.h>


/// rx & tx rings must be a power of two
STATIC_ASSERT((USART_RX_RING_SIZE & (USART_RX_RING_SIZE - 1)) == 0);
STATIC_ASSERT((USART_TX_RING_SIZE & (USART_TX_RING_SIZE - 1)) == 0);


/**
 * @brief Clock definition is required for baudrate calculations.
 */
#ifndef F_CPU
#error Clock frequency undefined (F_CPU).
#endif


void usart_enable(uint8_t n) {
#define USART_POWER_ENABLE(__no) \
    case __no : power_usart##__no##_enable(); break
    switch(n) {
#ifdef power_usart0_enable
        USART_POWER_ENABLE(0);
#endif
#ifdef power_usart1_enable
        USART_POWER_ENABLE(1);
#endif
#ifdef power_usart2_enable
        USART_POWER_ENABLE(2);
#endif
#ifdef power_usart3_enable
        USART_POWER_ENABLE(3);
#endif
        default: break;
    } // switch
#undef USART_POWER_ENABLE
}


volatile void* usart_base_get(uint8_t n) {
#define USART_BASE_ADDR_GET(__no) \
    case __no : return (volatile void *)&UCSR##__no##A
    switch(n) {
#ifdef UCSR0A
        USART_BASE_ADDR_GET(0);
#endif
#ifdef UCSR1A
        USART_BASE_ADDR_GET(1);
#endif
#ifdef UCSR2A
        USART_BASE_ADDR_GET(2);
#endif
#ifdef UCSR3A
        USART_BASE_ADDR_GET(3);
#endif
        default: break;
    } // switch

    return NULL;
#undef USART_BASE_ADDR_GET
}


void usart_init(uint8_t usart_dev_no,
        usart_settings* settings,
        volatile usart_ctx *ctx) {

    uint8_t buff_size = sizeof(ctx->rx);
    UTIL_MEM_ZERO(&ctx->rx, buff_size);

    buff_size = sizeof(ctx->tx);
    UTIL_MEM_ZERO(&ctx->tx, buff_size);

#if USART_COLLECT_STATS == 1
    usart_stats_reset(&ctx->stats);
#endif

    ctx->usart_dev_no = usart_dev_no;
    ctx->um = usart_base_get(usart_dev_no);
    usart_enable(usart_dev_no);
}
