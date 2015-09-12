#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include <serial.h>


#if SERIAL_IMPLEMENT_TX_INT == 1

/**
 * @brief transmission complete interrupt
 *
 * @param USART_TX_vect
 * @param ISR_BLOCK
 */
ISR(USART_UDRE_vect, ISR_BLOCK) {
	
	// proceed if there still is data to be send
	if (g_tx_buff.u.r.head != g_tx_buff.u.r.tail) {
		UDR0 = g_tx_buff.u.r.ring[g_tx_buff.u.r.tail];
		g_tx_buff.u.r.tail = (g_tx_buff.u.r.tail + 1) % SERIAL_TX_RING_SIZE;

#if SERIAL_COLLECT_STATS == 1
		g_tx_buff.stats.ok++;
#endif
	}
	else {
		// mask the interrupt everything has been send
		UCSR0B &= ~_BV(UDRIE0);
	}
}
#endif

/* ================================================================================ */

/**
 * @brief wrapper for stdio, just to make the serial API interface compatible
 *
 * @param c character to 'print'
 * @param stream file stream
 */
static int _serial_putc(char c, FILE *stream) {
	if ('\n' == c) {
		_serial_putc('\r', stream);
	}

#if SERIAL_STDOUT_POLL == 1
	serial_poll_sendc(c);
#elif SERIAL_STDOUT_POLL == 0
	while (!serial_sendc(c));
#else
#error SERIAL_STDOUT_POLL must be either 0 (interrupt driven) or 1 (polling)
#endif

	return 0;
}


/**
 * @brief wrapper for stdio, just to make the serial API interface compatible
 *
 * @param stream 
 *
 * @return character received
 */
static int _serial_getc(FILE *stream) {
	unsigned char c = 0x00;

#if SERIAL_STDIN_POLL == 1
	serial_poll_getc(&c);
#elif SERIAL_STDIN_POLL == 0	
	while (!serial_getc(&c));
#endif

	return (char)c;
}


/* ================================================================================ */


e_return serial_init(uint32_t a_speed) {
	serial_flush();
	return RET_OK;
}


void serial_install_stdio() {
	static FILE uart_stdout = FDEV_SETUP_STREAM(_serial_putc, NULL, _FDEV_SETUP_WRITE);
	static FILE uart_stdin = FDEV_SETUP_STREAM(NULL, _serial_getc, _FDEV_SETUP_READ);

	stdout = &uart_stdout;
	stdin = &uart_stdin;
}


#if SERIAL_IMPLEMENT_RX_INT == 1
inline unsigned char serial_available() {
	return (SERIAL_RX_RING_SIZE + g_rx_buff.u.r.head - g_rx_buff.u.r.tail) % SERIAL_RX_RING_SIZE;
}


unsigned char serial_peek(void *a_data, unsigned char a_size) {
	unsigned char read = serial_available();
	if (read > a_size)
		read = a_size;

	for (unsigned char i = 0; i<read; i++) {
		((unsigned char *)a_data)[i] =
			g_rx_buff.u.r.ring[ (g_rx_buff.u.r.tail + i) % SERIAL_RX_RING_SIZE ];
	}

	return read;
}


unsigned int serial_recv(void *a_data, unsigned int a_size, unsigned char a_waitall) {
	unsigned int read = 0x00;

	if (!a_waitall && g_rx_buff.u.r.head == g_rx_buff.u.r.tail) {
		return 0;
	}

	while ( read < a_size ) {

		while (serial_available() && (read < a_size)) 
		{
			((unsigned char *)a_data)[read] = g_rx_buff.u.r.ring[ g_rx_buff.u.r.tail ];
			g_rx_buff.u.r.tail = (g_rx_buff.u.r.tail + 1) % SERIAL_RX_RING_SIZE;
			read++;
		}

		if (!a_waitall)
			break;
	}
	
	return read;
}


unsigned char serial_getc(unsigned char *a_data) {
	
	if (g_rx_buff.u.r.head == g_rx_buff.u.r.tail)
		return 0;

	*a_data = g_rx_buff.u.r.ring[g_rx_buff.u.r.tail];
	g_rx_buff.u.r.tail = (g_rx_buff.u.r.tail + 1) % SERIAL_RX_RING_SIZE;
	return 1;
}
#endif


unsigned char serial_poll_recv(unsigned char *a_data, unsigned int a_size) {
	unsigned int cnt = a_size;

	while (a_size) {
		/* Wait for data to be received */
		while ( !(UCSR0A & (1<<RXC0)) );

		/* Get and return received data from buffer */
		*a_data = UDR0;
		a_data++;
		a_size--;
	}

	return cnt;
}


unsigned char serial_poll_getc(unsigned char *a_data) {
	return serial_poll_recv(a_data, 1);
}


#if SERIAL_IMPLEMENT_TX_INT == 1
unsigned char serial_send(void *a_data, unsigned int a_size, unsigned char a_waitall) {
	uint8_t n = 0x00;
	uint8_t *data = (uint8_t *)a_data;
	uint8_t initiated = 0x00;

	while (a_size) {
		volatile unsigned char next =
		   	((g_tx_buff.u.r.head + 1) % SERIAL_TX_RING_SIZE);

		/// do not overflow the buffer
		if (next != g_tx_buff.u.r.tail) {
			g_tx_buff.u.r.ring[g_tx_buff.u.r.head] = *data;
			g_tx_buff.u.r.head = next;			
		}
		else {
			if (a_waitall) {
				if (!initiated) {
					initiated = 0x01;

					// enable data register empty interrupt
					UCSR0B |= _BV(UDRIE0);
				}
				continue;
			}
			else
				break;
		}

		a_size--;
		n++;
		data++;
	}

	if (!a_waitall || !initiated) {
		initiated = 0x01;
		// enable data register empty interrupt
		UCSR0B |= _BV(UDRIE0);
	}

	return n;
}


unsigned char serial_sendc(unsigned char a_data) {

	uint8_t n = 0x00;
	uint8_t next =
		((g_tx_buff.u.r.head + 1) % SERIAL_TX_RING_SIZE);

	/// do not overflow the buffer
	if (next != g_tx_buff.u.r.tail) {
		g_tx_buff.u.r.ring[g_tx_buff.u.r.head] = a_data;
		g_tx_buff.u.r.head = next;
		n = 1;

		// enable data register empty interrupt
		UCSR0B |= _BV(UDRIE0);
	}
#if SERIAL_COLLECT_STATS == 1
	else {
		g_tx_buff.stats.dropped++;
	}
#endif

	return n;
}
#endif


unsigned int serial_poll_send(void *data, unsigned int a_size) {
	unsigned int i = 0x00;
	while (i < a_size) {
		// wait until usart buffer is empty
		while ( bit_is_clear(UCSR0A, UDRE0) );

		// send the data
		UDR0 = ((unsigned char *)data)[i++];
	}

	return i;
}


unsigned int serial_poll_sendc(unsigned char a_char) {
	return serial_poll_send((void *)&a_char, 1);
}


void serial_flush() {
	unsigned char dummy __attribute__((unused)) = 0x00;

#if SERIAL_IMPLEMENT_RX_INT == 1
	g_rx_buff.u.r.tail = g_rx_buff.u.r.head = 0x00;
#endif

	// flush the input fifo
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}


#if SERIAL_IMPLEMENT_RX_INT == 1
volatile t_buffer* serial_get_rx_state() {
	return &g_rx_buff;
}
#endif


#if SERIAL_IMPLEMENT_TX_INT == 1
volatile t_buffer* serial_get_tx_state() {
	return &g_tx_buff;
}
#endif
