#ifndef USART_COMMON_H_SLKXUAGC
#define USART_COMMON_H_SLKXUAGC

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
 * @brief USART parity settings (UPMn bits)
 */
typedef enum _usart_parity {
    USART_PARITY_DISABLED = 0x00,
    USART_PARITY_EVEN = 0x02,
    USART_PARITY_ODD = 0x03,
} usart_parity;


/**
 * @brief Frame's data bits
 */
typedef enum _usart_databits {
    USART_DATABITS5 = 0x00,
    USART_DATABITS6 = 0x01,
    USART_DATABITS7 = 0x02,
    USART_DATABITS8 = 0x03,
    USART_DATABITS9 = 0x07,
} usart_databits;


/**
 * @brief Frame's stop bits
 */
typedef enum _usart_stopbits {
    USART_STOPBITS0 = 0x00,
    USART_STOPBITS1 = 0x01,
} usart_stopbits;


/**
 * @brief usart operational mode
 */
typedef enum usart_mode {
    USART_ASYNC_NORMAL = 0x00,
    USART_ASYNC_DOUBLE = 0x01,
    USART_ASYNC_ANY = 0x02,
    USART_SYNC = 0x03,
} usart_mode;


/**
 * @brief This struct aggregates most important USART settings which can be
 * configured with usart_init call.
 */
typedef struct _usart_settings {
    uint32_t baudrate;
    uint8_t is_rx_enable;
    uint8_t is_tx_enable;

    // frame format settings
    union {
        // usart in asynchronous mode
        struct {
            uint8_t stopbits;
            uint8_t databits;
            uint8_t parity;
        } usart;

        // usart in SPI mode
        struct {
            // 0 - MSB first
            uint8_t dataorder;

            // refer to spi_mode enum
            uint8_t spimode;
        } spi;
    } ff;

    uint8_t mode;
} usart_settings;


/**
 * @brief crude way to count number of USART's available for given device.
 */
typedef enum _usart_dev_enumeration {
#ifdef UCSR0A
    USART_DEV_USART0,
#endif
#ifdef UCSR1A
    USART_DEV_USART1,
#endif
#ifdef UCSR2A
    USART_DEV_USART2,
#endif
#ifdef UCSR3A
    USART_DEV_USART3,
#endif

    USART_DEV_MAX_USARTS
} usart_dev_enumeration;


#define USART_DEFAULT_ASYNC(__baud) \
    { \
        .baudrate = (__baud), \
        .is_rx_enabled = 0x01, \
        .is_tx_enabled = 0x01, \
        .parity = USART_PARITY_DISABLED, \
        .stopbits = USART_STOPBITS1, \
        .databits = USART_DATABITS8, \
        .mode = USART_ASYNC_ANY \
    }


/**
 * @brief Enable or disable the transmitter
 *
 * @param __ctx usart ctx
 * @param 0/1 - disable/enable
 */
#define USART_COMMON_TX_SET(__ctx, __val) \
    if (__val) { \
        (__ctx)->um->ucsrb |= _BV(TXEN0); \
    } \
    else { \
        (__ctx)->um->ucsrb &= ~_BV(TXEN0); \
    }


/**
 * @brief Enable or disable the receiver
 *
 * @param __ctx usart ctx
 * @param 0/1 - disable/enable
 */
#define USART_COMMON_RX_SET(__ctx, __val) \
    if (__val) { \
        (__ctx)->um->ucsrb |= _BV(RXEN0); \
    } \
    else { \
        (__ctx)->um->ucsrb &= ~_BV(RXEN0); \
    }


/**
 * @brief calculates the value to be written to UBRRX register
 *
 * UBRRX configures the UART's clock frequeny
 *
 * @param clock system clock speed (F_CPU macro can be used)
 * @param baud requested baud-rate
 * @param mode usart_mode
 *
 * @return calculated UBRRX
 */
uint16_t usart_common_calculate_ubrr(uint32_t clock, uint32_t baud, uint8_t mode);


/**
 * @brief calculates the baudrate from given system clock and UBRR value
 *
 * @param clock system clock speed (F_CPU macro can be used)
 * @param ubrr value from UBRR register
 * @param mode usart_mode
 *
 * @return calculated baudrate
 */
uint32_t usart_common_calculate_baud(uint32_t clock, uint16_t ubrr, uint8_t mode);


#endif /* end of include guard: USART_COMMON_H_SLKXUAGC */
