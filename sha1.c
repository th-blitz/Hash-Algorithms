#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define h0 0x67452301
#define h1 0xEFCDAB89
#define h2 0x98BADCFE
#define h3 0x10325476
#define h4 0xC3D2E1F0

#define rotateleft(x, n) ((x << n) | (x >> (32 - n)))

#define u32_swap_endian(x) ( ((x >> 24) & 0xff) | ((x << 8) & 0xff0000) | ((x >> 8) & 0xff00) | ((x << 24) & 0xff000000) )


uint64_t u64_swap_endian(uint64_t x) {
    x = (x & 0x00000000ffffffff) << 32 | (x & 0xffffffff00000000) >> 32;
    x = (x & 0x0000ffff0000ffff) << 16 | (x & 0xffff0000ffff0000) >> 16;
    x = (x & 0x00ff00ff00ff00ff) << 8  | (x & 0xff00ff00ff00ff00) >> 8;
    return x;
}

uint32_t eighty_word_buffer(uint32_t* buffer, uint32_t index) {
    
    uint32_t word;
    
    switch(index / 16) {
        case 0:
            word = buffer[index];
            break;
        default:
            word = rotateleft(
                    (uint32_t)(
                            buffer[(index - 3) % 16] ^
                            buffer[(index - 8) % 16] ^
                            buffer[(index - 14) % 16] ^
                            buffer[(index - 16) % 16]
                        ),
                    1
                );
            buffer[(index - 16) % 16] = word;  
    }
    return word;
}


void sha1_core(uint8_t* blocks, size_t blocks_len, uint32_t digest[4]) {
    
    uint32_t a, b, c, d, e, f, k, temp;
    uint32_t buffer[80];
    
    for (size_t block = 0; block < blocks_len; block += 64) {
        
        a = digest[0];
        b = digest[1];
        c = digest[2];
        d = digest[3];
        e = digest[4];

        for (size_t i = 0; i < 16; i++) {
            buffer[i] = u32_swap_endian(*(uint32_t*)(blocks + block + (i * 4)));
        }

        for (uint32_t i = 0; i < 80; i++) {
            switch(i / 20) {
                case 0:
                    f = (b & c) ^ ((~b) & d);
                    k = 0x5A827999;
                    break;
                case 1:
                    f = (b ^ c ^ d);
                    k = 0x6ED9EBA1;
                    break;
                case 2:
                    f = (b & c) ^ (b & d) ^ (c & d);
                    k = 0x8F1BBCDC;
                    break;
                default:
                    f = b ^ c ^ d;
                    k = 0xCA62C1D6;
            }
            temp = rotateleft(a, 5) + f + e + k + eighty_word_buffer(buffer, i);
            e = d;
            d = c;
            c = rotateleft(b, 30);
            b = a;
            a = temp;
        }

        digest[0] += a;
        digest[1] += b;
        digest[2] += c;
        digest[3] += d;
        digest[4] += e;
    }
}

void sha1_digest(uint8_t* message, size_t message_len, uint32_t digest[5], bool debug) {

    size_t padding_len = 64 - ((message_len + 8) % 64);
    (padding_len == 0) ? (padding_len = 64) : (padding_len = padding_len);

    size_t final_block_len;
    (padding_len <= 56) ? (final_block_len = 64) : (final_block_len = 128);
    
    size_t total_len = message_len + padding_len + 8;

    if (debug == true) {
        printf("- (1/4) | total_len: %ld, message_len: %ld, padding_len: %ld, final_block_len: %ld \n", total_len, message_len, padding_len, final_block_len);
    }

    digest[0] = (uint32_t)h0;
    digest[1] = (uint32_t)h1;
    digest[2] = (uint32_t)h2;
    digest[3] = (uint32_t)h3;
    digest[4] = (uint32_t)h4;

    sha1_core(message, total_len - final_block_len, digest);

    if (debug == true) {
        printf("- (2/4) | md5_core() initial digest done.\n");
    }

    size_t offset = message_len - (total_len - final_block_len);

    uint8_t final_block[128];
    for (size_t i = 0; i < offset; i++) {
        final_block[i] = *(uint8_t*)(message + (total_len - final_block_len) + i);
    }
    final_block[offset] = 0b10000000;
    for (size_t i = offset + 1; i < offset + padding_len; i++) {
        final_block[i] = 0b00000000;
    }
    uint64_t message_len_in_bits = u64_swap_endian((uint64_t)(message_len * 8));
    memcpy(final_block + offset + padding_len, &message_len_in_bits, 8);

    if (debug == true) {
        printf("- (3/4) | md5 padding done.\n");
    }

    sha1_core(final_block, final_block_len, digest);
    if (debug == true) {
        printf("- (4/4) | md5_core() final digest done.\n");
    } 
}


int main(size_t argv, char* argc[]) {

    bool debug = false;
    uint32_t digest[5] = {0};

    if (argv > 1) {
        for (int i = 1; i < argv; i++) {
            sha1_digest(argc[i], strlen(argc[i]), digest, debug);
            for (int i = 0; i < 5; i++) {
                printf("%x ", digest[i]);
            }
            printf("\n");
        }
    } else if (argv == 1) {
        sha1_digest("", 0, digest, debug);
        for (int i = 0; i < 5; i++) {
                printf("%x ", digest[i]);
            }
        printf("\n");
    }

    return 0;
}