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


#include <stdint.h>

#include <avr/io.h>
#include <util/atomic.h>


/**
 * @brief structure describing the memory location of gpio registers
 */
typedef struct _gpio_map {
    /// PINX (pull-up control, input, toggle)
    volatile uint8_t pin;

    /// DDRX (data direction)
    volatile uint8_t ddr;

    /// PORTX (pull-up, IO)
    volatile uint8_t port;
} gpio_map;


/**
 * @brief structure describing a single io line
 */
typedef struct _gpio_pin {
    /// gpio port map
	volatile gpio_map * const gm;

    /// pin (bit)
	uint8_t pin;
} gpio_pin;


/**
 * @brief finds a gpio_map pointer given the PORTX register
 *
 * @param __portx pointer to PORTX register
 */
#define GET_GPIO_MAP_FROM_PORT(__portx) \
    ((volatile gpio_map *)(((volatile uint8_t *)__portx) - 2))


/**
 * @brief finds a gpio_map pointer given the PINX register
 *
 * @param __pinx pointer to PINX register
 */
#define GET_GPIO_MAP_FROM_PIN(__pinx) \
    ((volatile gpio_map *)__pinx)


/**
 * @brief finds a gpio_map pointer given the DDRX register
 *
 * @param __ddrx pointer to DDRX register
 */
#define GET_GPIO_MAP_FROM_DDR(__ddrx) \
    ((volatile gpio_map *)(((volatile uint8_t *)__ddrx) - 1))


/**
 * @brief configures given gpio as output
 *
 * @param __val pointer to gpio_pin instance.
 */
#define GPIO_CONFIGURE_AS_OUTPUT(__gpio) \
	((__gpio)->gm->ddr) |= _BV((__gpio)->pin)


/**
 * @brief configures given gpio as input
 *
 * @param __val pointer to gpio_pin instance.
 */
#define GPIO_CONFIGURE_AS_INPUT(__gpio) \
	((__gpio)->gm->ddr) &= ~_BV((__gpio)->pin)


/**
 * @brief get the value of GPIO pin
 *
 * @param __val pointer to gpio_pin instance.
 *
 * @return Zero if unset, non-zero value otherwise.
 */
#define GPIO_GET(__gpio) \
	(((__gpio)->gm->pin) & _BV((__gpio)->pin))


/**
 * @brief set the value of GPIO pin low
 *
 * @param __val pointer to gpio_pin instance.
 */
#define GPIO_SET_LOW(__gpio) \
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
        ((__gpio)->gm->port) &= ~_BV((__gpio)->pin); \
	}


/**
 * @brief set the value of GPIO pin high
 *
 * @param __val pointer to gpio_pin instance.
 */
#define GPIO_SET_HIGH(__gpio) \
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
        ((__gpio)->gm->port) |= _BV((__gpio)->pin); \
	}


/**
 * @brief toggle's (negate it's current value) the value of gpio pin
 *
 * @param __val pointer to gpio_pin instance.
 */
#define GPIO_TOGGLE(__gpio) \
    ((__gpio)->gm->pin) |= _BV((__gpio)->pin)


#endif /* end of include guard: GPIO_H_TUGJ3L7E */

