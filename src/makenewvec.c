#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define N 1000
#define WN 5000
#define WLIMIT 50
#define LENGTH 64
#define SEED 65535
#define ULIMIT 5

int read1ofn (FILE *fpi, int n);
void putvec(int nextn, int n);
int searchs(int s, int n, int wn);
int matchptn(int i, int s, int n);
int rndn(int n);

char ngram[WN][N];

int main(int argc, char **argv)
{
    int n;
    int wn;
    int s;
    FILE *fpi;
    char inputfile[LENGTH];
    int i, j;
    int loopmax;

    srand(SEED);

    if (argc < 2) {
        fprintf(stderr, "less parameters \n > makenewvec"
                "word num: n\n");
        exit(1);
    }

    fpi = stdin;
    n = atoi(argv[1]);
    if (argc > 3) {
        strncpy(inputfile, argv[3], LENGTH);
        if ((fpi=fopen(inputfile, "r")) == NULL) {
            fprintf(stderr, "%s: open input file failed!", inputfile);
            exit(1);
        }
    }

    fprintf(stderr, "word num: %d, start word no. %d\n", n, s);
    if((s>=n) || (s < 0)) {
        fprintf(stderr, "s=%d,s\n", s);
        exit(1);
    }

    wn = read1ofn(fpi, n);
    putvec(s, n);

    for (i=0; i<WLIMIT; ++i) {
        loopmax = rndn(ULIMIT);
        for (j=0; j<loopmax; ++j) {
            s = searchs(s, n, wn);
            ++s;
            if (s >= wn) s = 0;
            putvec(s, n);
        }

    }
    return 0;
}

int rndn(int n)
{
    double rndno;
    while ((rndno=(double)rand()/RAND_MAX) == 1.0);
    return rndno * n;
}

int searchs(int s, int n, int wn)
{
    int i;
    for (i=s+1; i<wn; ++i)
        if (matchptn(i, s, n) == TRUE) return i;
    for (i=0; i<=s; ++i)
        if (matchptn(i, s, n) == TRUE) return i;
    fprintf(stderr, "not found in searchs()\n");
    exit(1);
}

int matchptn(int i, int s, int n)
{
    int result = TRUE;
    int index;

    for (index=0; index<n; index++) {
        if (ngram[i][index] != ngram[s][index])
            result = FALSE;
        return result;
    }
}

void putvec(int nextn, int n)
{
    int j;
    for (j=0; j<n; ++j)
        printf("%1d ", ngram[nextn][j]);
    printf("\n");
}

int read1ofn(FILE *fpi, int n)
{
    int e;
    int i = 0, j = 0;

    while ((fscanf(fpi, "%d", &e) != EOF) && (i < WN)) {
        ngram[i][j] = e;
        ++j;
        if (j >= n) {
            j = 0;
            ++i;
        }
    }
    return i;
}
