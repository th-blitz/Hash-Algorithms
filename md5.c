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

#define u32_swap_endian(x) ( ((x >> 24) & 0xff) | ((x << 8) & 0xff0000) | ((x >> 8) & 0xff00) | ((x << 24) & 0xff000000) )


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

void md5_core(uint8_t* blocks, size_t blocks_len, uint32_t digest[4]) {
    
    uint32_t a, b, c, d, f, g;
    
    for (size_t block = 0; block < blocks_len; block += 64) {
        
        a = digest[0];
        b = digest[1];
        c = digest[2];
        d = digest[3];

        for (uint32_t i = 0; i < 64; i++) {
            switch(i / 16) {
                case 0:
                    f = F(b, c, d);
                    g = i;
                    break;
                case 1:
                    f = G(b, c, d);
                    g = ((5 * i) + 1) % 16;
                    break; 
                case 2:
                    f = H(b, c, d);
                    g = ((3 * i) + 5) % 16;
                    break; 
                default:
                    f = I(b, c, d);
                    g = (7 * i) % 16;
            }
            f += a + K[i] + *(uint32_t*)(blocks + block + (g * 4));
            a = d;
            d = c;
            c = b;
            b += rotateleft(f, S[i]);
        }
        digest[0] += a;
        digest[1] += b;
        digest[2] += c;
        digest[3] += d;
    }
}

void md5_digest(uint8_t* message, size_t message_len, uint32_t digest[4]) {

    size_t padding_len = 64 - ((message_len + 8) % 64);
    (padding_len == 0) ? (padding_len = 64) : (padding_len = padding_len);

    size_t final_block_len;
    (padding_len <= 56) ? (final_block_len = 64) : (final_block_len = 128);
    
    size_t total_len = message_len + padding_len + 8;
    
    digest[0] = (uint32_t)A;
    digest[1] = (uint32_t)B;
    digest[2] = (uint32_t)C;
    digest[3] = (uint32_t)D;

    md5_core(message, total_len - final_block_len, digest);
    
    uint8_t final_block[128];
    for (size_t i = 0; i < message_len; i++) {
        final_block[i] = message[(total_len - final_block_len) + i];
    }
    final_block[message_len] = 0b10000000;
    for (size_t i = message_len + 1; i < message_len + padding_len; i++) {
        final_block[i] = 0b00000000;
    }
    uint64_t message_len_in_bits = (uint64_t)(message_len * 8);
    memcpy(final_block + message_len + padding_len, &message_len_in_bits, 8);

    md5_core(final_block, final_block_len, digest);
    digest[0] = u32_swap_endian((uint32_t)(digest[0]));
    digest[1] = u32_swap_endian((uint32_t)(digest[1]));
    digest[2] = u32_swap_endian((uint32_t)(digest[2]));
    digest[3] = u32_swap_endian((uint32_t)(digest[3]));
}


int main(size_t argv, char* argc[]) {

    char message[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz.....";
    uint64_t message_len = 57;
    // e4d909c2 90d0fb1c a068ffad df22cbd0;

    uint32_t digest[4] = {0};
    if (argv > 1) {
        for (int i = 1; i < argv; i++) {
            md5_digest(argc[i], strlen(argc[i]), digest);
            for (int i = 0; i < 4; i++) {
                printf("%x ", digest[i]);
            }
            printf("\n");
        }
    } else if (argv == 1) {
        md5_digest("", 0, digest);
        for (int i = 0; i < 4; i++) {
                printf("%x ", digest[i]);
            }
        printf("\n");
    }

    return 0;
}