#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <stdbool.h>

#include "sha512.h"
#include "sha1.h"

static int is_bigEndian() {
    int i = 1;
    return *(char*)&i;
}

static uint8_t is_BigEndian() {
    uint32_t num = 0x01000000;
    return *(uint8_t*)&num;
}

int main() {

    char* message = "This is an example string ! ";

    sha512 hash = SHA512();

    hash.digest(&hash, message, strlen(message));

    for (int i = 0; i < 8; i++) {
        printf("%lx ", hash.digests[i]);
    }    
    printf("\n");

    uint32_t an_hex_num = 0x76543210;
    uint8_t *an_hex_num_byte_pointer = (uint8_t*) &an_hex_num;

    printf("%x \n", *an_hex_num_byte_pointer); 
    printf("%x \n", *(an_hex_num_byte_pointer + 1));
    printf("%x \n", *(an_hex_num_byte_pointer + 2));
    printf("%x \n", *(an_hex_num_byte_pointer + 3));
    printf("is_BigEndian() : %d \n", is_BigEndian());

    return 0;
}
