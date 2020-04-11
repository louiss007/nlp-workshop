#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define LENGTH 64
#define N 10000
#define VOCFILE "voc.txt"

int main(int argc, char **argv)
{
    char word[LENGTH*10];
    char dictionary[N][LENGTH];
    int n;
    FILE *fpvoc, *fpi;
    char inputfile[LENGTH];
    char vocfile[LENGTH] = VOCFILE;
    int e;
    int i;

    fpi = stdin;
    if (argc >= 2) strncpy(vocfile, argv[1], LENGTH);
    if (argc >= 3) strncpy(inputfile, argv[2], LENGTH);

    if ((fpvoc=fopen(vocfile, "r")) == NULL) {
        fprintf(stderr, "%s:open word of voc file failed!", vocfile);
        exit(1);
    }

    if ((argc>=3) && (fpi=fopen(inputfile, "r")) == NULL) {
        fprintf(stderr, "%s: open input file failed!", inputfile);
        exit(1);
    }

    while (fscanf(fpvoc, "%s", word) != EOF) {
        strncpy(dictionary[n], word, LENGTH);
        ++n;
    }
    printf("word num:%d\n", n);

    i = 0;
    while (fscanf(fpi, "%d", &e) != EOF) {
        if (e == 1) printf("%s", dictionary[i]);
        ++i;
        if (i >= n) i = 0;
    }
    printf("\n");
    return 0;
}

