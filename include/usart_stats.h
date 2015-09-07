#ifndef USART_STATS_H_JTB3WXE4
#define USART_STATS_H_JTB3WXE4

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
 * @brief IO buffer statistics type  declaration
 */
typedef struct _usart_stats {
	/// number of a valid rx/tx bytes
	volatile uint32_t ok;

	/// number of dropped data bytes due to buffer being full
	volatile uint32_t dropped;

	/// number of bytes received with frame error indicator
	volatile uint32_t frame_error;
} usart_stats;


#endif /* end of include guard: USART_STATS_H_JTB3WXE4 */
