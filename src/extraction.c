#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

int is2byte(char chr);

int main()
{
    int chr;
    while ((chr = getchar()) != EOF) {
        if (is2byte(chr) == TRUE) {
            putchar(chr);
            putchar(getchar());
            putchar(getchar());
        }
    }

    return 0;
}

int is2byte(char c)
{
    if ((c>>8) < 0)
        return TRUE;
    return FALSE;
}
