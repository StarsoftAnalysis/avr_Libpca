#include "serial.h"

#include "util/mem.h"
#include "util/util.h"


/// rx & tx rings must be a power of two
STATIC_ASSERT((SERIAL_RX_RING_SIZE & (SERIAL_RX_RING_SIZE - 1)) == 0);
STATIC_ASSERT((SERIAL_TX_RING_SIZE & (SERIAL_TX_RING_SIZE - 1)) == 0);


void serial_init(uint8_t usart_dev_no,
        usart_settings* settings,
        volatile usart_ctx *ctx) {

    uint8_t ring_size = sizeof(ctx->rx);
    UTIL_MEM_ZERO(&ctx->rx, ring_size);

    ring_size = sizeof(ctx->tx);
    UTIL_MEM_ZERO(&ctx->tx, ring_size);

    ctx->usart_dev_no = usart_dev_no;

}
