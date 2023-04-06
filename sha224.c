
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// https://eips.ethereum.org/assets/eip-2680/sha256-384-512.pdf

#include "sha224.h"

#define h0 0xc1059ed8
#define h1 0x367cd507
#define h2 0x3070dd17
#define h3 0xf70e5939
#define h4 0xffc00b31
#define h5 0x68581511
#define h6 0x64f98fa7
#define h7 0xbefa4fa4

static uint32_t k[64] = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
    };

#define rotateleft(x, n) ((x << n) | (x >> (32 - n)))
#define rotateright(x, n) ((x >> n) | (x << (32 - n)))

#define u32_swap_endian(x) ( ((x >> 24) & 0xff) | ((x << 8) & 0xff0000) | ((x >> 8) & 0xff00) | ((x << 24) & 0xff000000) )


uint64_t u64_swap_endian(uint64_t x) {
    x = (x & 0x00000000ffffffff) << 32 | (x & 0xffffffff00000000) >> 32;
    x = (x & 0x0000ffff0000ffff) << 16 | (x & 0xffff0000ffff0000) >> 16;
    x = (x & 0x00ff00ff00ff00ff) << 8  | (x & 0xff00ff00ff00ff00) >> 8;
    return x;
}


void sha224_core(uint8_t* blocks, size_t blocks_len, uint32_t digest[]) {
    
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t s0, s1, S0, S1, ch, maj, temp1, temp2;
    uint32_t buffer[64];
    
    for (size_t block = 0; block < blocks_len; block += 64) {

        for (size_t i = 0; i < 16; i++) {
            buffer[i] = u32_swap_endian(*(uint32_t*)(blocks + block + (i * 4)));
        }

        for (size_t i = 16; i < 64; i++) {
            s0 = (
                rotateright(buffer[i - 15], 7) ^
                rotateright(buffer[i - 15], 18) ^
                (buffer[i - 15] >> 3)
            );
            s1 = (
                rotateright(buffer[i - 2], 17) ^
                rotateright(buffer[i - 2], 19) ^
                (buffer[i - 2] >> 10)
            );
            buffer[i] = buffer[i - 16] + s0 + buffer[i - 7] + s1;
        }

        a = digest[0];
        b = digest[1];
        c = digest[2];
        d = digest[3];
        e = digest[4];
        f = digest[5];
        g = digest[6];
        h = digest[7];

        for (uint32_t i = 0; i < 64; i++) {
            S1 = (rotateright(e, 6) ^ rotateright(e, 11) ^ rotateright(e, 25));
            ch = ((e & f) ^ ((~e) & g));
            temp1 = h + S1 + ch + k[i] + buffer[i];
            S0 = (rotateright(a, 2) ^ rotateright(a, 13) ^ rotateright(a, 22));
            maj = ((a & b) ^ (a & c) ^ (b & c));
            temp2 = S0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;  
        }

        digest[0] += a;
        digest[1] += b;
        digest[2] += c;
        digest[3] += d;
        digest[4] += e;
        digest[5] += f;
        digest[6] += g;
        digest[7] += h;
    }
}

void sha224_digest(uint8_t* message, size_t message_len, uint32_t digest[], bool debug) {

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
    digest[5] = (uint32_t)h5;
    digest[6] = (uint32_t)h6;
    digest[7] = (uint32_t)h7;

    sha224_core(message, total_len - final_block_len, digest);

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

    sha224_core(final_block, final_block_len, digest);
    if (debug == true) {
        printf("- (4/4) | md5_core() final digest done.\n");
    } 
}

