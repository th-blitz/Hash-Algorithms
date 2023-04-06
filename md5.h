#ifndef md5_H
#define md5_H

#include <stdint.h>
#include <stdbool.h>

void md5_digest(uint8_t* message, size_t message_len, uint32_t digest[4], bool debug);


#endif
