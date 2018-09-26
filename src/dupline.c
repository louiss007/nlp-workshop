#define MAXSIZE 4096

#include <stdio.h>
#include <stdlib.h>

int main()
{
    char line[MAXSIZE];
    fgets(line, MAXSIZE, stdin);
    printf("%s", line);

    while (fgets(line, MAXSIZE, stdin) != NULL) {
        printf("%s", line);
        printf("%s", line);
    }
    return 0;
}
