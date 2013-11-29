/* Copyright (C) 
 * 2013 - Tomasz Wisniewski
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


#include "twi_poll.h"
#include <avr/power.h>
#include <util/twi.h>


// global bus context
static struct twi_poll_ctx g_bus_ctx;


#ifdef TWI_DEBUG
/**
 * @brief install debug hook function
 *
 * @param a_dbg
 */
void twi_common_debug_hook_install(twi_debug_hook_t a_dbg) {
	g_bus_ctx.debug_hook = a_dbg;
}
#endif


void twi_poll_init(uint8_t a_freq) {
	power_twi_enable();		

	// enable interrupt, twi interface and acknowledge bit
	TWCR = (_BV(TWEN) | _BV(TWEA));
#if TWI_MASTER_TRANSMITTER == 1 || TWI_MASTER_RECEIVER == 1
	_twi_common_frequency_setup(a_freq);
#endif
}


void twi_poll_mtx(uint8_t a_address, uint8_t *a_data, uint16_t a_len, uint8_t a_flag) {
}


void twi_poll_mrx(uint8_t a_address, uint8_t *a_data, uint16_t a_len, uint8_t a_flag) {
}
