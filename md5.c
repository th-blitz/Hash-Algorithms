#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476

#define F(B, C, D) (B & C) | ((~B) & D)
#define G(B, C, D) (B & D) | (C & (~D))
#define H(B, C, D) (B ^ C ^ D)
#define I(B, C, D) (C ^ (B | (~D)))


#define rotateleft(x, n) ((x << n) | (x >> (32 - n)))

#define u32_big_to_little_endian(x) ( ((x >> 24) & 0xff) | ((x << 8) & 0xff0000) | ((x >> 8) & 0xff00) | ((x << 24) & 0xff000000) )


static uint32_t S[] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

static uint32_t K[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };


int main() {

    char message[] = "Hello, world!";
    uint64_t message_len = 5;

    uint64_t padding_len = 64 - ((message_len + 8) % 64);
    if (padding_len == 0) {
        padding_len = 56;
    }
    
    uint8_t* input_buffer = (uint8_t*)malloc(message_len + padding_len + 8);
    uint8_t pad = 0x80;

    memcpy(input_buffer, message, message_len);
    input_buffer[message_len] = 0b10000000;
    for (size_t i = message_len + 1; i < padding_len; i++) {
        input_buffer[message_len] = (uint8_t)0x00;
    }
    uint64_t message_len_in_bits = message_len * 8;
    memcpy(input_buffer + message_len + padding_len, &message_len_in_bits, 8);
    printf("%02x", input_buffer[0]);
    for (int i = 0; i < message_len + padding_len + 8; i++) {
        i % 16 == 0 ? printf("\n"):printf(" ");
        printf("%x", input_buffer[i]);
    }
    printf("\n");

    uint32_t a0 = A;
    uint32_t b0 = B;
    uint32_t c0 = C;
    uint32_t d0 = D;

    for (uint32_t block = 0; block < message_len + padding_len + 8; block += 64) {

        uint32_t a = a0;
        uint32_t b = b0;
        uint32_t c = c0;
        uint32_t d = d0;

        uint32_t f, g;

        for (uint32_t i = 0; i < 64; i++) {
            if ( i >= 0 && i < 16) {
                f = F(b, c, d);
                g = i;
            } else if (i >= 16 && i < 32) {
                f = G(b, c, d);
                g = ((5 * i) + 1) % 16;
            } else if (i >= 32 && i < 48) {
                f = H(b, c, d);
                g = ((3 * i) + 5) % 16;
            } else if (i >= 48 && i < 64) {
                f = I(b, c, d);
                g = (7 * i) % 16;
            }
            uint32_t temp = d;
            d = c;
            c = b;
            b = b + (uint32_t)rotateleft(a + f + K[i] + *(uint32_t*)(input_buffer + block + (g * 4)), S[i]);
            a = temp;
        }                

        a0 += a;
        b0 += b;
        c0 += c;
        d0 += d;
    }

    printf("%x ", a0);
    printf("%x ", b0);
    printf("%x ", c0);
    printf("%x ", d0);
    printf("\n");

    return 0;
}