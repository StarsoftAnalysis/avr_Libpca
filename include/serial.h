#ifndef __SERIAL_H__
#define __SERIAL_H__

/* Copyright (C)
 * 2015 - Tomasz Wisniewski
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */


#include "ring.h"
#include "_usart_map.h"
#include "_usart_stats.h"

#include "config/serial.h"

#include <stdint.h>


typedef enum _usart_parity {
    USART_PARITY_DISABLED = 0x00,
    USART_PARITY_EVEN = 0x02,
    USART_PARITY_ODD = 0x03,
} usart_parity;


typedef enum _usart_databits {
    USART_DATABITS5 = 0x00,
    USART_DATABITS6 = 0x01,
    USART_DATABITS7 = 0x02,
    USART_DATABITS8 = 0x03,
    USART_DATABITS9 = 0x07,
} usart_databits;


typedef enum _usart_stopbits {
    USART_STOPBITS0 = 0x00,
    USART_STOPBITS1 = 0x01,
} usart_stopbits;


typedef struct _usart_settings {
    uint32_t baudrate;
    uint8_t is_rx_enable;
    uint8_t is_tx_enable;
    uint8_t parity;
    uint8_t stopbits;
    uint8_t databits;
    uint8_t is_synchronous;
} usart_settings;


#define USART_DEFAULT_ASYNC(__baud) \
    { \
        .baudrate = (__baud), \
        .is_rx_enabled = 0x01, \
        .is_tx_enabled = 0x01, \
        .parity = USART_PARITY_DISABLED, \
        .stopbits = USART_STOPBITS1, \
        .databits = USART_DATABITS8, \
        .is_synchronous = 0x00 \
    }


/**
 * @brief General purpose USART descriptor
 */
typedef struct _usart_ctx {
    /// USART device port map pointer
    volatile usart_map * const um;

    /// which usart ? (zero for devices with only one usart)
    uint8_t usart_dev_no;

    /// ingress ring buffer
    union {
        volatile ring_buffer8 ring;
        volatile uint8_t _raw[SERIAL_RX_RING_SIZE + sizeof(ring_buffer8)];
    } rx;


    /// egress ring buffer
    union {
        volatile ring_buffer8 ring;
        volatile uint8_t _raw[SERIAL_TX_RING_SIZE + sizeof(ring_buffer8)];
    } tx;


#if SERIAL_COLLECT_STATS == 1
    volatile usart_stats stats;
#endif

} usart_ctx;


void serial_init(uint8_t usart_dev_no, usart_settings* settings, volatile usart_ctx *ctx);


#endif /* __SERIAL_H__ */
