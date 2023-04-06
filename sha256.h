#ifndef sha256_H
#define sha256_H

#include <stdint.h>
#include <stdbool.h>

void sha256_digest(uint8_t* message, size_t message_len, uint32_t digest[], bool debug);

#endif