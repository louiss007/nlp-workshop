#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define N 9

int is2byte(char chr);
void put3gram(char chr);
int invert(int flag);

static int count = 1;
int main() 
{
    char chr;
    while ((chr = getchar()) != EOF) {
        if (is2byte(chr) == TRUE) {
            put3gram(chr);
            put3gram(getchar());
            put3gram(getchar());
        }
    }

    return 0;
}

int invert(int flag) 
{
    if (flag == FALSE) 
        return TRUE;
    return FALSE;
}

void put3gram(char chr) 
{
    static char queue[N] = "    ";
    static int flag = FALSE;
    int i;

    for (i=0; i<N-1; i++)
        queue[i] = queue[i+1];
    queue[N-1] = chr;
    count++;

    if (flag == TRUE) {
        for (i=0; i<N; ++i)
            putchar(queue[i]);
        putchar('\n');
    }
    if (count % 3 == 0)
        //flag = invert(flag);
        flag = TRUE;
    else
        flag = FALSE;
}

int is2byte(char c)
{
    if ((c>>8) < 0)
        return TRUE;
    return FALSE;
}

