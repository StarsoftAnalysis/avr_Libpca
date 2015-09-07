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
#include "usart_map.h"
#include "usart_stats.h"

#include "config/serial.h"

#include <stdint.h>


/**
 * @brief General purpose USART descriptor
 */
typedef struct _usart {
    /// USART device port map pointer
    volatile usart_map * const um;


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

} usart;


#endif /* __SERIAL_H__ */
