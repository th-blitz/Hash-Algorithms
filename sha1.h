#ifndef sha1_H
#define sha1_H

#include <stdint.h>
#include <stdbool.h>

void sha1_digest(uint8_t* message, size_t message_len, uint32_t digest[5], bool debug);


#endif