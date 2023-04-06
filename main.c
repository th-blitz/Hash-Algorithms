
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "sha512.h"



int main() {

    char* message = "hi there";

    sha512 hash = SHA512();

    hash.end(&hash, message, strlen(message));

    for (int i = 0; i < 8; i++) {
        printf("%lx ", hash.digest[i]);
    }    
    printf("\n");

    return 0;
}