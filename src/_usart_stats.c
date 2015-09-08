#include "_usart_stats.h"
#include "util/mem.h"


void usart_stats_reset(usart_stats *stats) {
    uint8_t stats_size = sizeof(usart_stats);
    UTIL_MEM_ZERO(stats, stats_size);
}
