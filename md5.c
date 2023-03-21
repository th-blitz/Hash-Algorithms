#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int main() {
    char message[] = "The quick brown fox jumps over the lazy dog.";
    uint64_t len = 0;
    int message_len = 0;
    // 64 bytes 512 bits block;
    // 8 bytes 64 bits block;
    int padding_len = 64 - ((message_len + 8) % 64);
    
    int total_len = (int)(message_len + padding_len + 8);
    
    printf("padding len : %d\n", padding_len);
    printf("total_len : %d\n", total_len);
    uint8_t* new = (uint8_t*)malloc(sizeof(uint8_t) * total_len);
    memcpy(new, message, message_len);
    new[message_len] = (uint8_t)0b10000000;
    for (int i = message_len + 1; i < (message_len + padding_len); i++) {
        new[i] = (uint8_t)0b00000000;
    }
    len = (uint64_t)(message_len * 8);

    memcpy(new + (padding_len + message_len), &len, 8);

    // uint64_t* len_ptr = &len;
    // for (int i = 7, j = 0; i >= 0; i--, j++) {
    //     *(new + message_len + padding_len + j) = *(len_ptr + i);
    // }

    for (int i = 0; i < (message_len + padding_len + 8); i++) {
        printf("%x ", new[i]);
    } 
    printf("\n");
    printf("%ld\n",*((uint64_t*)(new + (message_len + padding_len))));
    
    uint32_t s[] = {
        7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
        5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
        4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
        6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
    };

    uint32_t k[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
 
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
 
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
 
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 
    };
    
    uint32_t a0 = 0x67452301;
    uint32_t b0 = 0xefcdab89;
    uint32_t c0 = 0x98badcfe;
    uint32_t d0 = 0x10325476;

    // uint32_t a0 = 0x01234567;
    // uint32_t b0 = 0x89abcdef;
    // uint32_t c0 = 0xfedcba98;
    // uint32_t d0 = 0x76543210;

    uint8_t* chunk = new;
    
    for (uint32_t j = 0; j < total_len; j += 64) {
       
        chunk += j;

        uint32_t a = a0;
        uint32_t b = b0;
        uint32_t c = c0;
        uint32_t d = d0;

        for (uint32_t i = 0; i < 64; i++) {
            uint32_t f, g; 
            if (0 <= i < 16) {
                f = (b & c) | ((~b) & d);
                g = i; 
            } else if (16 <= i < 32) {
                f = (d & b) | ((~d) & c);
                g = ((5 * i) + 1) % 16;
            } else if (32 <= i < 48) {
                f = b ^ c ^ d;
                g = ((3 * i) + 5) % 16;
            } else {
                f = c ^ (b | (~d));
                g = (7 * i) % 16;
            }
            f = f + a + k[i] + *(uint32_t*)(chunk + g);
            a = d;
            d = c;
            c = b;
            b += ((f<<s[i]) | (f>>(32-s[i]))); 
        }
        a0 += a;
        b0 += b;
        c0 += c;
        d0 += d;
    }

    printf("%x", a0);
    printf("%x", b0);
    printf("%x", c0);
    printf("%x\n", d0);
    return 0;
}