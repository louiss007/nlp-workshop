#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define VOCSIZE 11
#define WORDLEN 5
#define FILTERSIZE 3

void conv(double filter[][FILTERSIZE]
        , double sentence[][VOCSIZE]
        , double convout[][VOCSIZE]);
double calcconv(double filter[][FILTERSIZE]
        , double sentence[][VOCSIZE], int i, int j);

void convres(double convout[][VOCSIZE]);
void getdata(double sentence[][VOCSIZE]);

int main()
{
    double filter[FILTERSIZE][FILTERSIZE]
        = {{1,0,0}, {0,1,0}, {0,0,1}};
    //  = {{1,0,0}, {1,0,0}, {1,0,0}};
    double sentence[WORDLEN][VOCSIZE];
    double convout[WORDLEN][VOCSIZE] = {0};

    getdata(sentence);
    conv(filter, sentence, convout);
    convres(convout);

    return 0;
}

void convres(double convout[][VOCSIZE])
{
    int i, j;
    int startpoint = FILTERSIZE / 2;

    for (i=startpoint; i<WORDLEN-1; ++i) {
        for (j=startpoint; j<VOCSIZE-1; ++j) {
            printf("%.3lf ", convout[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void getdata(double e[][VOCSIZE])
{
    int i = 0, j = 0;
    while (scanf("%lf", &e[i][j]) != EOF) {
        ++j;
        if (j >= VOCSIZE) {
            j = 0;
            ++i;
            if (i >= WORDLEN) break;
        }
    }
}

void conv(double filter[][FILTERSIZE]
        , double sentence[][VOCSIZE], double convout[][VOCSIZE])
{
    int i = 0, j = 0;
    int startpoint = FILTERSIZE / 2;

    for (i=startpoint; i<WORDLEN-startpoint; ++i)
        for (j=startpoint; j<VOCSIZE-startpoint; ++j)
            convout[i][j] = calcconv(filter, sentence, i, j);
}

double calcconv(double filter[][FILTERSIZE]
        , double sentence[][VOCSIZE], int i, int j)
{
    int m, n;
    double sum = 0;

    for (m=0; m<FILTERSIZE; ++m)
        for (n=0; n<FILTERSIZE; ++n)
            sum += sentence[i - FILTERSIZE/2 + m][j - FILTERSIZE/2 + n] * filter[m][n];
    return sum;
}

