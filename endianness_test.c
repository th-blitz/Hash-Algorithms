#include <stdint.h>

static uint8_t is_big_endian() {
    uint32_t num = 0x01000000;
    return *(uint8_t*)&num;
}

static uint8_t is_little_endian() {
    uint32_t num = 0x00000001;
    return *(uint8_t*)&num;
}

