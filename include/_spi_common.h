#ifndef _SPI_COMMON_H_BRF0OXAN
#define _SPI_COMMON_H_BRF0OXAN

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


typedef enum _spi_mode {
    /// Sample data line on clock's rising edge, clock stable = low
    SPI_MODE_0 = 0,

    /// Sample data line on clock's falling edge, clock stable = low
    SPI_MODE_1,

    /// Sample data line on clock's rising edge, clock stable = high
    SPI_MODE_2,

    /// Sample data line on clock's falling edge, clock stable = high
    SPI_MODE_3
} spi_mode;

#endif /* end of include guard: _SPI_COMMON_H_BRF0OXAN */
