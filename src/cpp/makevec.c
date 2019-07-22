#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define LENGTH 64
#define N 5000
#define INPUTFILE "w1gram.txt"
#define OUTPUTFILE "voc.txt"

int isnew(char word[], 
        char dictionary[][LENGTH], int n);
void putvec(char word[],
        char dictionary[][LENGTH], int n);

int main(int argc, char **argv)
{
    char word[LENGTH*10];
    char dictionary[N][LENGTH];
    int n = 0;
    FILE *fpi, *fpo;
    char inputfile[LENGTH] = INPUTFILE;
    char outputfile[LENGTH] = OUTPUTFILE;

    if (argc >= 2) strncpy(inputfile, argv[1], LENGTH);
    if (argc >= 3) strncpy(outputfile, argv[2], LENGTH);

    if ((fpi=fopen(inputfile, "r")) == NULL) {
        fprintf(stderr, "%s: open input file failed!\n", inputfile);
        exit(1);
    }

    if ((fpo=fopen(outputfile, "w")) == NULL) {
        fprintf(stderr, "%s: open output file failed!\n", outputfile);
        exit(1);
    }

    while (fscanf(fpi, "%s", word) != EOF) {
        if (isnew(word, dictionary, n) == TRUE) {
            strncpy(dictionary[n], word, LENGTH);
            ++n;
        }
    }
    fprintf(stderr, "word num: %d\n", n);
    rewind(fpi);

    while (fscanf(fpi, "%s", word) != EOF) {
        putvec(word, dictionary, n);
    }

    {
        int i;
        for (i=0; i<n; i++) fprintf(fpo, "%s\n", dictionary[i]);
    }
    return 0;
}

int isnew(char word[],
        char dictionary[][LENGTH], int n)
{
    int i;
    for (i=0; i<n; i++) {
        if ((strncmp(word, dictionary[i], LENGTH) == 0
                    && (strlen(word) == strlen(dictionary[i]))))
            break;
    }
    if (i < n)
        return FALSE;
    return TRUE;
}

void putvec(char word[],
        char dictionary[][LENGTH], int n)
{
    int i;
    for (i=0; i<n; i++) {
        if ((strncmp(word, dictionary[i], LENGTH) == 0)
                && (strlen(word) == strlen(dictionary[i])))
            printf("1");
        else
            printf("0");
        printf(" ");
    }
    printf("\n");
}
