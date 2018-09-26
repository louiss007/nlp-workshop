#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUTNO 5
#define HIDDENNO 5
#define OUTPUTNO 5
#define WORDLEN 50

double fs(double u); 
void readwh(double wh[HIDDENNO][INPUTNO+1+HIDDENNO]);
void readwo(double wo[OUTPUTNO][HIDDENNO+1]);
double drnd(void);
double forward(double wh[HIDDENNO][INPUTNO+1+HIDDENNO]
        , double wo[HIDDENNO+1], double hi[]
        , double e[]);
void putword(double inputdata[]);


int main(int argc, char **argv)
{
    double wh[HIDDENNO][INPUTNO+1+HIDDENNO];
    double wo[OUTPUTNO][HIDDENNO+1];
    double hi[HIDDENNO+1] = {0};
    double o[OUTPUTNO];
    double inputdata[INPUTNO+HIDDENNO] = {0};
    int i, j;
    int startno;

    if (argc < 2) {
        startno = 0;
    }
    else {
        startno = atoi(argv[1]);
        if (startno < 0 || (startno >= INPUTNO)) {
            fprintf(stderr, "start word no. %d\n", startno);
            exit(1);
        }
    }
    inputdata[startno] = 1.0;
    readwh(wh);
    readwo(wo);

    for (i=0; i<WORDLEN; i++) {
        //for (j=0; j<INPUTNO+HIDDENNO; ++j)
        //    fprintf(stderr, "%.3lf ", e[i][j]);
        //fprintf(stderr, "\n");
        putword(inputdata);
        for (j=0; j<OUTPUTNO; ++j)
            o[j] = forward(wh, wo[j], hi, inputdata);
        for (j=0; j<HIDDENNO; ++j)
            inputdata[j] = o[j];
        for (j=0; j<HIDDENNO; ++j)
            inputdata[INPUTNO+j] = hi[j];
    }

    return 0;
}

void putword(double inputdata[])
{
    int i;
    int maxindex = 0;
    double max = inputdata[0];

    for (i=1; i<INPUTNO; ++i)
        if (max < inputdata[i]) {
            max = inputdata[i];
            maxindex = i;
        }
    for (i=0; i<INPUTNO; ++i) {
        if (i == maxindex) 
            printf("1 ");
        else 
            printf("0 ");
    }
    printf("\n");
}

double forward(double wh[HIDDENNO][INPUTNO+1+HIDDENNO], 
        double wo[HIDDENNO+1], double hi[], double e[])
{
    int i, j;
    double u;
    double o;

    for (i=0; i<HIDDENNO; ++i) {
        u = 0;
        for (j=0; j<INPUTNO+HIDDENNO; ++j)
            u += e[j] * wh[i][j];
        u -= wh[i][j];
        hi[i] = fs(u);
    }

    o = 0;
    for (i=0; i<HIDDENNO; i++)
        o += hi[i] * wo[i];
    o -= wo[i];

    return fs(o);
}

void readwh(double wh[HIDDENNO][INPUTNO+1+HIDDENNO])
{
    int i, j;

    for (i=0; i<HIDDENNO; i++) {
        for (j=0; j<INPUTNO+1+HIDDENNO; ++j)
            scanf("%lf", &(wh[i][j]));
    }
}

void readwo(double wo[OUTPUTNO][HIDDENNO+1])
{
    int i, j;

    for (i=0; i<OUTPUTNO; ++i) {
        for (j=0; j<HIDDENNO; ++j)
            scanf("%lf", &(wo[i][j]));
    }
}

double fs(double u)
{
    return 1.0 / (1.0+exp(-u));
}
