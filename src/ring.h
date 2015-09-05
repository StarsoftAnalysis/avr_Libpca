#ifndef RING_H_FDIQOKDH
#define RING_H_FDIQOKDH

/* Copyright (C)
 * 2014 - Tomasz Wisniewski
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


#include <stdint.h>
#include <stdlib.h>


#define DECLARE_RING(__type) \
    typedef struct _ring_buffer { \
        /// head of the buffer (write position) \
        volatile __type head; \
        \
        /// tail of the buffer (read position) \
        volatile __type tail; \
        \
        /// total capacity of the ring buffer \
        volatile __type capacity; \
        \
        /// data \
        volatile uint8_t ring[]; \
    }


/**
 * @brief 8 bit ring buffer.
 *
 * Defines a ring buffer of bytes type, which has internal indexes of uint8_t type.
 */
DECLARE_RING(uint8_t) ring_buffer8;


/**
 * @brief 16 ring buffer.
 *
 * Defines a ring buffer of bytes type, which has internal indexes of uint16_t type.
 */
DECLARE_RING(uint16_t) ring_buffer16;


#endif /* end of include guard: RING_H_FDIQOKDH */

