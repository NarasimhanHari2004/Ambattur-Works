#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char user_input[100];
    if (argc != 2) {
        printf("Usage: %s <input>\n", argv[0]);
        exit(1);
    }

    snprintf(user_input, sizeof(user_input), argv[1]);
    printf(user_input);  // Vulnerability!
    printf("\n");

    return 0;
}

