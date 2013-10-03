#include "soft_onewire.h"
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <util/atomic.h>


#define SOW_OUTPUT(__ddr, __pin) \
	*__ddr |= _BV(__pin)


#define SOW_INPUT(__ddr, __pin) \
	*__ddr &= ~_BV(__pin)


#define SOW_HIGH(__port, __pin) \
	*__port |= _BV(__pin)


#define SOW_LOW(__port, __pin) \
	*__port &= ~_BV(__pin)



void sow_init(struct soft_ow *a_bus) {

	// disable pull-ups
	MCUCR |= _BV(PUD);

	// configure as input, (state high because of pull-up)
	SOW_INPUT(a_bus->ddr, a_bus->pin);

	// sleep for a while
	_delay_us(500);
}


void sow_string_pullup(struct soft_ow *a_bus, uint8_t a_enable) {
	if (a_enable) {
		SOW_OUTPUT(a_bus->ddr, a_bus->pin);
		SOW_HIGH(a_bus->outp, a_bus->pin);
		return;
	}

	SOW_INPUT(a_bus->ddr, a_bus->pin);
}


uint8_t sow_reset(struct soft_ow *a_bus) {

	uint8_t presence = 0;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		SOW_OUTPUT(a_bus->ddr, a_bus->pin);
		SOW_LOW(a_bus->outp, a_bus->pin);
		_delay_us(500);

		SOW_INPUT(a_bus->ddr, a_bus->pin);
		_delay_us(50); // 15 - 60

		if (bit_is_clear(a_bus->inp, a_bus->pin)) {
			// after 60 - 240
			_delay_us(300); 
			if (bit_is_set(a_bus->inp, a_bus->pin)) {
				presence = 1;
			}
		} // bit is clear
	} // ATOMIC_BLOCK
	
	return presence;
}


static uint8_t ow_write_bit(uint8_t a_bit) {
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		OW_OUTPUT;
		OW_LOW;
		_delay_us(5); // max 15
		if (a_bit & 0x01)
			OW_INPUT;

		_delay_us(80);
		OW_INPUT;
	}
	return 1;
}


static uint8_t ow_read_bit() {
	uint8_t bit = 0;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		OW_OUTPUT;
		OW_LOW;
		_delay_us(1); // >= 1
		OW_INPUT;

		_delay_us(12); // sample within 15u slot
		if (bit_is_set(OW_PORTI, OW_PIN))
			bit = 1;

		// time slot duration
		_delay_us(60);

		// recovery time
		_delay_us(1); 
	}
	return bit;
}

