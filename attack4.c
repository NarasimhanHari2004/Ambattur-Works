#include <stdio.h>
#include <stdlib.h>

int target = 0;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input>\n", argv[0]);
        exit(1);
    }

    printf("Before: target = 0x%x\n", target);
    printf(argv[1], ((char *)&target) + 2, &target);
    printf("\nAfter:  target = 0x%x\n", target);

    unsigned char *p = (unsigned char *)&target;
    printf("target bytes: %02x %02x %02x %02x\n", p[0], p[1], p[2], p[3]);

    return 0;
}

