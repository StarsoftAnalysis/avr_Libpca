#include "tdelay_common.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* ================================================================================ */

#if TDELAY_IMPLEMENT_T0_INT == 1 || TDELAY_IMPLEMENT_T1_INT == 1 || TDELAY_IMPLEMENT_T2_INT == 1
static volatile struct {
	volatile uint32_t duration;
	volatile uint8_t reset_cmpa_pin;
} g_tc[E_TIMER_LAST];


/**
 * @brief interrupt context structure
 */
struct regs {
	volatile e_timer timer;
	volatile uint8_t *timsk;
	volatile uint8_t *tccrb;
	volatile uint8_t *port;
	volatile uint8_t pin;
};

/* ================================================================================ */

static void _isr_tdelay_handler(volatile struct regs *sregs) {

	if (!g_tc[sregs->timer].duration) {
		// MASK the interrupt + disable the clock;		
		*(sregs->timsk) &= ~_BV(1); // OCIEXA
		*(sregs->tccrb) &= 0xf8;

		if (!g_tc[sregs->timer].reset_cmpa_pin)
			*(sregs->port) &= ~_BV(sregs->pin);
	}
	else {
		g_tc[sregs->timer].duration--;
	}
}

#endif

static uint8_t __tdc_block(e_timer a_timer, volatile uint8_t *timsk, uint8_t map) {
	if (bit_is_clear((*timsk), map)) {				
		_tdc_set_duration(a_timer, 0x00);
		return 0;
	}
	return 1;
}

/* ================================================================================ */

#if TDELAY_IMPLEMENT_T2_INT == 1
/**
 * @brief timer 2
 *
 * @param TIMER2_COMPA_vect
 */
ISR(TIMER2_COMPA_vect, ISR_NOBLOCK) {
	volatile struct regs tr2 = { E_TIMER2, &TIMSK2, &TCCR2B, &PORTB, PORTB3 };
	_isr_tdelay_handler( &tr2 );
}

#endif


#if TDELAY_IMPLEMENT_T1_INT == 1
/**
 * @brief timer 1
 *
 * @param TIMER1_COMPA_vect
 */
ISR(TIMER1_COMPA_vect, ISR_NOBLOCK) {
	volatile struct regs tr1 = { E_TIMER1, &TIMSK1, &TCCR1B, &PORTB, PORTB1 };
	_isr_tdelay_handler( &tr1 );
}

#endif


#if TDELAY_IMPLEMENT_T0_INT == 1
/**
 * @brief timer 0
 *
 * @param TIMER0_COMPA_vect
 */
ISR(TIMER0_COMPA_vect, ISR_NOBLOCK) {
	volatile struct regs tr0 = { E_TIMER0, &TIMSK0, &TCCR0B, &PORTD, PORTD6 };
	_isr_tdelay_handler( &tr0 );
}

#endif

/* ================================================================================ */


inline void _tdc_set_duration(e_timer a_tim, uint32_t a_dur) {
	g_tc[a_tim].duration = a_dur;
}


inline void _tdc_set_cmp_pin(e_timer a_tim, uint8_t a_pin) {
	g_tc[a_tim].reset_cmpa_pin = a_pin;
}


void _tdc_setup_delay(e_timer a_timer, uint32_t a_freq, uint32_t a_delay) {

	uint32_t pocr = 0x00;

	// which pin
	switch(a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TIMER0:
			pocr = _tdc_freq_prescale(a_freq, 255);
			TCCR0B &= 0xf8;
			TCCR0B |= (pocr >> 24) & 0x07;			
			OCR0A = pocr & 0xff;
			TCNT0 = 0x00;
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TIMER1:
			pocr = _tdc_freq_prescale(a_freq, 260);
			TCCR1B &= 0xf8;
			TCCR1B |= ((pocr >> 24) & 0x07);
			OCR1AL = pocr & 0xff;
			OCR1AH = (pocr >> 8) & 0xff;
			TCNT1H = TCNT1L = 0x00;
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			pocr = _tdc_freq_prescale(a_freq, 255);
			TCCR2B &= 0xf8;
			TCCR2B |= (pocr >> 24) & 0x07;			
			OCR2A = pocr & 0xff;
			TCNT2 = 0x00;
			break;
#endif

		default:
			break;
	} // switch

	_tdc_set_duration(a_timer, (uint32_t)((a_freq*a_delay)/500));
}


void _tdc_enable_interrupt(e_timer a_timer) {
	// which pin
	switch(a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TIMER0:
			TIMSK0 |= _BV(OCIE0A);
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TIMER1:
			TIMSK1 |= _BV(OCIE1A);
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			TIMSK2 |= _BV(OCIE2A);
			break;
#endif

		default:
			break;
	} // switch
}


void _tdc_disable_interrupt(e_timer a_timer) {
	// which pin
	switch(a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TIMER0:
			TIMSK0 &= ~_BV(OCIE0A);
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TIMER1:
			TIMSK1 &= ~_BV(OCIE1A);
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			TIMSK2 &= ~_BV(OCIE2A);
			break;
#endif

		default:
			break;
	} // switch
}



void _tdc_block(e_timer a_timer) {
	unsigned char wait = 0;

	// deadlock protection
	switch (a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TIMER0:
			wait = __tdc_block(a_timer, &TIMSK0, OCIE0A);
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TIMER1:
			wait = __tdc_block(a_timer, &TIMSK1, OCIE1A);
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			wait = __tdc_block(a_timer, &TIMSK2, OCIE2A);
			break;
#endif

		default:
			break;
	} // switch

	if (wait) while (g_tc[a_timer].duration);
}


uint32_t _tdc_freq_prescale(uint32_t a_freq, uint16_t a_criterion) {
	uint8_t prescalers[] = { 0x00, 0x03, 0x06, 0x08, 0x0a, 0x00 };

	/**
	 * combine both prescaler and ocr value in one 32bit value 
	 */
	uint32_t retval = 0x00;
	uint16_t *ocr = (uint16_t *)&retval;
	uint8_t *presc = ((uint8_t *)&retval) + 3;

	do {
		*ocr = F_CPU / ((a_freq << 1) * (0x01 << prescalers[*presc]));
		++*presc;		
	} while ((*ocr > a_criterion) && (prescalers[*presc]));

	/* --*ocr; */
	if (*ocr > a_criterion) *ocr = a_criterion;
	return retval;
}
