#ifndef USART_MAP_H_JDTJTRFD
#define USART_MAP_H_JDTJTRFD

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


#include <stdint.h>


/**
 * @brief USARTX register map
 */
typedef struct _usart_map {
    volatile uint8_t ucsra;
    volatile uint8_t ucsrb;
    volatile uint8_t ucsrc;
    uint8_t __reserved1;

    union {
        volatile uint16_t ubrr;
        struct {
            volatile uint8_t ubrrl;
            volatile uint8_t ubrrh;
        };
    };
    volatile uint8_t udr;
} usart_map;


#endif /* end of include guard: USART_MAP_H_JDTJTRFD */
