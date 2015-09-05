#ifndef GPIO_H_TUGJ3L7E
#define GPIO_H_TUGJ3L7E

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


/**
 * @file gpio.h
 *
 * @brief Simple GPIO abstraction layer
 *
 * This module defines simple abstraction layer for the GPIO lines. You can build gpio independent drivers with this
 * implementation. There's no longer need to use ambiguous macro definitions. Please refer to the gpio example
 * on details how to use this API
 *
 * @example gpio.c
 *
 * Basic usage of the gpio api
 */


#include "config.h"
#include "common.h"

#include <avr/io.h>
#include <util/atomic.h>


/**
 * @brief structure describing a single io line
 */
typedef struct _gpio_pin {
	volatile uint8_t *port;
	uint8_t pin;
} gpio_pin;


/**
 * @brief get pointer to DDR register from PORT register
 *
 * @param __val pointer to PORT register
 *
 * @return pointer to DDR register
 */
#define GET_DDRX_FROM_PORTX(__portx) \
	(__portx - 1)


/**
 * @brief get pointer to PIN register from PORT register
 *
 * @param __val pointer to PORT register
 *
 * @return pointer to PIN register
 */
#define GET_PINX_FROM_PORTX(__portx) \
	(__portx - 2)


/**
 * @brief configures given gpio as output
 *
 * @param __val pointer to gpio_pin instance.
 */
#define GPIO_CONFIGURE_AS_OUTPUT(__gpio) \
	*(GET_DDRX_FROM_PORTX((__gpio)->port)) |= _BV((__gpio)->pin)


/**
 * @brief configures given gpio as input
 *
 * @param __val pointer to gpio_pin instance.
 */
#define GPIO_CONFIGURE_AS_INPUT(__gpio) \
	*(GET_DDRX_FROM_PORTX((__gpio)->port)) &= ~_BV((__gpio)->pin)


/**
 * @brief get the value of GPIO pin
 *
 * @param __val pointer to gpio_pin instance.
 *
 * @return Zero if unset, non-zero value otherwise.
 */
#define GPIO_GET(__gpio) \
	(*(GET_PINX_FROM_PORTX((__gpio)->port)) & _BV((__gpio)->pin))


/**
 * @brief set the value of GPIO pin low
 *
 * @param __val pointer to gpio_pin instance.
 */
#define GPIO_SET_LOW(__gpio) \
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
		(*(__gpio)->port) &= ~_BV((__gpio)->pin); \
	}


/**
 * @brief set the value of GPIO pin high
 *
 * @param __val pointer to gpio_pin instance.
 */
#define GPIO_SET_HIGH(__gpio) \
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
		(*(__gpio)->port) |= _BV((__gpio)->pin); \
	}


/**
 * @brief toggle's (negate it's current value) the value of gpio pin
 *
 * @param __val pointer to gpio_pin instance.
 */
#define GPIO_TOGGLE(__gpio) \
	*(GET_PINX_FROM_PORTX((__gpio)->port)) = _BV((__gpio)->pin)


#endif /* end of include guard: GPIO_H_TUGJ3L7E */

