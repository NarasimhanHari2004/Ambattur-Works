#include <stdio.h>
#include <stdlib.h>

int target = 0;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input>\n", argv[0]);
        exit(1);
    }

    printf("Before: target = 0x%x\n", target);

    // Pass the input as a format string AND real argument
    printf(argv[1], &target);

    printf("\nAfter:  target = 0x%x\n", target);

    return 0;
}

