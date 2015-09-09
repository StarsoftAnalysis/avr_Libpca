#ifndef __USART_H__
#define __USART_H__

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
#include "_usart_common.h"

#include "config/usart.h"

#include <stdint.h>


/**
 * @brief General purpose USART descriptor
 */
typedef struct _usart_ctx {
    /// USART device port map pointer
    volatile usart_map * um;

    /// which usart ? (zero for devices with only one usart)
    uint8_t usart_dev_no;

    /// ingress ring buffer
    union {
        volatile ring_buffer8 ring;
        volatile uint8_t _raw[USART_RX_RING_SIZE + sizeof(ring_buffer8)];
    } rx;


    /// egress ring buffer
    union {
        volatile ring_buffer8 ring;
        volatile uint8_t _raw[USART_TX_RING_SIZE + sizeof(ring_buffer8)];
    } tx;


#if USART_COLLECT_STATS == 1
    volatile usart_stats stats;
#endif

} usart_ctx;


/**
 * @brief initializes the USART contexts. Must be called before any other USART
 * operations.
 */
void usart_init(void);


/**
 * @brief Configures and enables selected USART device.
 *
 * @param usart_dev_no USART device id (starting from zero)
 * @param settings (USART settings)
 *
 */
void usart_configure(uint8_t usart_dev_no, usart_settings* settings);


#endif /* __USART_H__ */
