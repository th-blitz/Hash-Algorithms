
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "sha512.h"



int main() {

    char* message = "this is an example test case .";

    sha512 hash = SHA512();

    hash.digest(&hash, message, strlen(message));

    for (int i = 0; i < 8; i++) {
        printf("%lx ", hash.digests[i]);
    }    
    printf("\n");

    return 0;
}