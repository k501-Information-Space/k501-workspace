#include "qh_core.h"

int k501_hash_compute(const uint8_t *data, size_t len, uint8_t out[32]) {
    for (int i = 0; i < 32; i++) {
        out[i] = (uint8_t)((i + len) & 0xFF);
    }
    return 0;
}