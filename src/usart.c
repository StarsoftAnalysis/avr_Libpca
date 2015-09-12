/**
 * @file serial.c
 *
 * @brief Serial API Implementation
 *
 */

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
static void _usart_configure_ff(volatile usart_ctx* ctx, usart_settings* s);


/**
 * @brief receive USART interrupt
 *
 * Data is received in this ISR and placed in the RX ring buffer - if there is still space available.
 *  If statistics support is enabled those will be updated in this ISR as well
 *
 */
static void _usart_rx_isr(uint8_t n);

/* ========================================================================== */


#ifdef USART0_RXC_vect
#define USART_ISR_RX0 USART0_RXC_vect
#elif defined(USART0_RX_vect)
#define USART_ISR_RX0 USART0_RX_vect
#elif defined(USART_RXC_vect)
#define USART_ISR_RX0 USART_RXC_vect
#elif defined(USART_RX_vect)
#define USART_ISR_RX0 USART_RX_vect
#elif defined(UART0_RX_vect)
#define USART_ISR_RX0 UART0_RX_vect
#elif defined(UART_RX_vect)
#define USART_ISR_RX0 UART_RX_vect
#endif

#ifdef USART_ISR_RX0
ISR(USART_ISR_RX0) {
    _usart_rx_isr(0);
}
#endif


ISR(USART_UDRE_vect) {
}


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
    uint16_t ubrr = 0;
    cli();

    _usart_rings_reset(usart_dev_no);
    usart_enable(usart_dev_no);

    _usart_configure_baudrate(&g_usart[usart_dev_no], settings);
    _usart_configure_ff(&g_usart[usart_dev_no], settings);

    // set ubrr to zero in synchronous mode
    if (USART_SYNC == settings->mode) {
        ubrr = g_usart[usart_dev_no].um->ubrr;
        g_usart[usart_dev_no].um->ubrr = 0;
        DDRD |= _BV(PORTD4);
    }

    // restore the required settings
    if (USART_SYNC == settings->mode) {
        g_usart[usart_dev_no].um->ubrr = ubrr;
    }

	// disable transmission complete interrupt
    g_usart[usart_dev_no].um->ucsrb |= ~_BV(TXCIE0);

    // enable receive interrupt
    g_usart[usart_dev_no].um->ucsrb |= _BV(RXCIE0);

    USART_COMMON_TX_SET(&g_usart[usart_dev_no], settings->is_tx_enable);
    USART_COMMON_RX_SET(&g_usart[usart_dev_no], settings->is_rx_enable);

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

        mode = util_abs(s->baudrate - sbrate) > util_abs(s->baudrate - dbrate);
        brate  = mode ? dmode_ubrr : smode_ubrr;
    }
    else {
        brate = usart_common_calculate_ubrr(F_CPU, s->baudrate, mode);
    }

    ctx->um->ubrr = brate;

    // taking an assumption here that UCSRA has the same layout for every
    // UART (which might not always be the case)
    switch(mode) {
        case USART_ASYNC_NORMAL:
            ctx->um->ucsra &= ~_BV(U2X0);
            break;

        case USART_ASYNC_DOUBLE:
            ctx->um->ucsra |= _BV(U2X0);
            break;

        // usart in SPI mode
        case USART_SYNC:
            break;
    }
}


static void _usart_configure_ff(volatile usart_ctx* ctx, usart_settings* s) {
    if (USART_SYNC == s->mode) {
        ctx->um->ucsrc =
            (s->mode << UMSEL00) |
            (s->ff.spi.dataorder << UDORD0) |
            (s->ff.spi.spimode & 0x03);
    }
    else {
        // usart in async mode
        ctx->um->ucsrc =
            (s->mode << UMSEL00) |
            ((s->ff.usart.parity & 0x03) << UPM00) |
            ((s->ff.usart.stopbits & 0x01) << USBS0) |
            ((s->ff.usart.databits & 0x07) << UCSZ00);
    }
}


/* ========================================================================== */


static void _usart_rx_isr(uint8_t n) {
    volatile usart_ctx *ctx = &g_usart[n];

	// UCSR0A must be read before UDR0 !!!
	if (bit_is_clear(ctx->um->ucsra, FE0)) {

		/// must read the data in order to clear the interrupt flag
		volatile uint8_t data = ctx->um->udr;

		/// calculate the next available ring buffer data bucket index
		volatile uint8_t next =
		   	((ctx->rx.ring.head + 1) & (USART_RX_RING_SIZE - 1));

		/// do not overflow the buffer
		if (next != ctx->rx.ring.tail) {
            ctx->rx.ring.ring[ctx->rx.ring.head] = data;
            ctx->rx.ring.head = next;
#if USART_COLLECT_STATS == 1
            ctx->stats.ok++;
#endif
		}
		else {
#if USART_COLLECT_STATS == 1
			/// increase the dropped counter
            ctx->stats.dropped++;
#endif
		}
	}
	else {
		/// must read the data in order to clear the interrupt flag
		volatile uint8_t data UNUSED = UDR0;

#if USART_COLLECT_STATS == 1
		/// increase the frame error counter
        ctx->stats.frame_error++;
#endif
	}

}


/* ========================================================================== */
