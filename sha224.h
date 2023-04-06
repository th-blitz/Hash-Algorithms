#ifndef sha224_H
#define sha224_H

#include <stdint.h>
#include <stdbool.h>

void sha224_digest(uint8_t* message, size_t message_len, uint32_t digest[], bool debug);


#endif