#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 256

int main()
{
    char w1[N] = "", w2[N] = "";

    while (scanf("%s", w2) != EOF) {
        printf("%s->%s\n", w1, w2);
        strncpy(w1, w2, N);
    }
    return 0;
}
