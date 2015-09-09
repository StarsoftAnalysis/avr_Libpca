#include "usart.h"

#include "util/mem.h"
#include "util/util.h"

#include <avr/interrupt.h>
#include <avr/power.h>

#include <stdint.h>


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


/**
 * @brief global USART device context. Needed for interrupts.
 */
static volatile usart_ctx g_usart[USART_DEV_MAX_USARTS];


/* ========================================================================== */

static void _usart_rings_reset(uint8_t n);
static void _usart_configure_baudrate(volatile usart_ctx* ctx, usart_settings* s);

/* ========================================================================== */


void usart_init() {
    uint8_t n = 0;

    while (n < USART_DEV_MAX_USARTS) {
        _usart_rings_reset(n);

#if USART_COLLECT_STATS == 1
        usart_stats_reset(&g_usart[n].stats);
#endif

        g_usart[n].usart_dev_no = n;
        g_usart[n].um = usart_base_get(n);
    }
}


void usart_configure(uint8_t usart_dev_no, usart_settings* settings) {
    _usart_rings_reset(usart_dev_no);
    usart_enable(usart_dev_no);
    _usart_configure_baudrate(&g_usart[usart_dev_no], settings);

    sei();
}


/* ========================================================================== */


static void _usart_rings_reset(uint8_t n) {
    size_t rx_size = sizeof(g_usart[0].rx);
    size_t tx_size = sizeof(g_usart[0].tx);
    UTIL_MEM_ZERO(&g_usart[n].rx, rx_size);
    UTIL_MEM_ZERO(&g_usart[n].tx, tx_size);
}


static void _usart_configure_baudrate(volatile usart_ctx* ctx, usart_settings* s) {
    uint16_t brate = 0;
    uint8_t mode = s->mode;

    if (USART_ASYNC_ANY == s->mode) {
        uint16_t smode_ubrr =
            usart_common_calculate_ubrr(F_CPU, s->baudrate, USART_ASYNC_NORMAL);

        uint16_t dmode_ubrr =
            usart_common_calculate_ubrr(F_CPU, s->baudrate, USART_ASYNC_DOUBLE);

        uint32_t sbrate =
            usart_common_calculate_baud(F_CPU, smode_ubrr, USART_ASYNC_NORMAL);

        uint32_t dbrate =
            usart_common_calculate_baud(F_CPU, smode_ubrr, USART_ASYNC_DOUBLE);

        mode = util_abs(s->baudrate - sbrate) < util_abs(s->baudrate - dbrate);
        brate  = mode ? smode_ubrr : dmode_ubrr;
    }
    else {
        brate = usart_common_calculate_ubrr(F_CPU, s->baudrate, mode);
    }

    ctx->um->ubrr = brate;
    // set the double mode bit if needed
}


/* ========================================================================== */
