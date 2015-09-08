#ifndef USART_H_GPKUWB0J
#define USART_H_GPKUWB0J

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


/**
 * @brief change to 0 and rebuild if you do not need stats information
 */
#define USART_COLLECT_STATS 0


/**
 * @brief RX ring buffer size if USART_IMPLEMENT_RX_INT == 1
 *
 * For high speeds and very busy applications one may consider increasing the RX ring buffer size
 * This variable has any useful meaning only if USART_IMPLEMENT_RX_INT == 1
 */
#define USART_RX_RING_SIZE 64


/**
 * @brief TX ring buffer size if USART_IMPLEMENT_TX_INT == 1
 *
 * For high speeds and very busy applications one may consider increasing the TX ring buffer size
 * This variable has any useful meaning only if USART_IMPLEMENT_TX_INT == 1
 */
#define USART_TX_RING_SIZE 64


#endif /* end of include guard: USART_H_GPKUWB0J */
