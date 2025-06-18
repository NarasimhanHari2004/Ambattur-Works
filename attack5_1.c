// vuln_shell.c
#include <stdio.h>
#include <string.h>

int main() {
    char buffer[128];

    printf("Enter input: ");
    fgets(buffer, sizeof(buffer), stdin);

    printf("You entered: %s\n", buffer);
    return 0;
}

