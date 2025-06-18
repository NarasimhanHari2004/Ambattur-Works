#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void vulnerable(char *input) {
    char buffer[200];
    strcpy(buffer, input);     // ✅ vulnerable to buffer overflow + format string
    printf(buffer);            // ✅ format string vulnerability
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input>\n", argv[0]);
        exit(1);
    }
    vulnerable(argv[1]);
    return 0;
}

