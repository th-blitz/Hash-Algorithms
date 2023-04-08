#ifndef sha512_H
#define sha512_H


#include <stdint.h>
#include <stdbool.h>

typedef struct sha512 {
    uint64_t digests[8];
    uint64_t len;
    void (*init)(struct sha512* self);
    void (*update)(struct sha512* self, uint8_t* blocks, uint64_t blocks_len);
    void (*digest)(struct sha512* self, uint8_t* blocks, uint64_t blocks_len);
} sha512;

sha512 SHA512();

#endif