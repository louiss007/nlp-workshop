#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define VOCSIZE 12
#define WORDLEN 7
#define FILTERSIZE 3
#define POOLSIZE 3 
#define FILTERNO 2

#define INPUTNO 48
#define HIDDENNO 2
#define ALPHA 2
#define SEED 7
#define MAXINPUTNO 100
#define BIGNUM 100
#define LIMIT 0.01


void convpool(double s[WORDLEN][VOCSIZE],
        double mfilter[FILTERNO][FILTERSIZE][FILTERSIZE],
        double se[INPUTNO + 1]);
void conv(double filter[][FILTERSIZE]
        , double sentence[][VOCSIZE]
        , double convout[][VOCSIZE]);
double calcconv(double filter[][FILTERSIZE]
        , double sentence[][VOCSIZE], int i, int j); 

void convres(double convout[][VOCSIZE]);
void poolres(double poolout[][VOCSIZE]);
void pool(double convout[][VOCSIZE]
        , double poolout[][VOCSIZE]);
double maxpooling(double convout[][VOCSIZE]
        , int i, int j); 

double fs(double u);
void readwh(double wh[HIDDENNO][INPUTNO+1]);
void readwo(double wo[HIDDENNO+1]);
double drnd(void);
void print(double wh[HIDDENNO][INPUTNO+1]
        , double wo[HIDDENNO+1]);
double forward(double wh[HIDDENNO][INPUTNO+1]
        , double WO[HIDDENNO+1], double hi[]
        , double e[INPUTNO+1]);
int getdata(double sentence[MAXINPUTNO][WORDLEN][VOCSIZE]);

int main() 
{
    double mfilter[FILTERNO][FILTERSIZE][FILTERSIZE]
        = {
            {{1,0,0}, {0,1,0}, {0,0,1}},
            {{1,0,0}, {1,0,0}, {1,0,0}}
        };
    double sentence[MAXINPUTNO][WORDLEN][VOCSIZE];
    double convout[WORDLEN][VOCSIZE];
    double poolout[WORDLEN][VOCSIZE];
    double teacher[MAXINPUTNO];
    double wh[HIDDENNO][INPUTNO+1];
    double wo[HIDDENNO+1];
    double e[MAXINPUTNO][INPUTNO+1];
    double hi[HIDDENNO+1];
    double o;
    double err = BIGNUM;
    int i, j;
    int n_of_e;
    int count = 0;

    readwh(wh);
    readwo(wo);
    print(wh, wo);

    n_of_e = getdata(sentence);
    printf("sample num : %d\n", n_of_e);

    for (i=0; i<n_of_e; i++) {
        convpool(sentence[i], mfilter, e[i]);
    }

    print(wh, wo);

    for (i=0; i<n_of_e; ++i) {
        printf("%d\n", i);
        for (j=0; j<INPUTNO; j++)
            printf("%lf ", e[i][j]);
        printf("\n");
        o = forward(wh, wo, hi, e[i]);
        printf("%lf\n\n", o);
    }

    return 0;
}

void poolres(double poolout[][VOCSIZE])
{
    int i, j;
    int startpoint = FILTERSIZE / 2 + POOLSIZE / 2;
    for (i=startpoint; i<WORDLEN-startpoint; ++i) {
        for (j=startpoint; j<VOCSIZE - startpoint; ++j)
            printf("%.3lf ", poolout[i][j]);
        printf("\n");
    }
    printf("\n");
}

void pool(double convout[][VOCSIZE]
        , double poolout[][VOCSIZE])
{
    int i, j;
    int startpoint = FILTERSIZE / 2 + POOLSIZE / 2;

    for (i=startpoint; i<WORDLEN-startpoint; ++i)
        for (j=startpoint; j<VOCSIZE-startpoint;++j)
            poolout[i][j] = maxpooling(convout, i, j);
}

double maxpooling(double convout[][VOCSIZE]
        , int i, int j)
{
    int m, n;
    double max;

    max = convout[i+POOLSIZE/2][j+POOLSIZE/2];
    for (m=i-POOLSIZE/2; m<=i+POOLSIZE/2; ++m)
        for (n=j-POOLSIZE/2; n<=j+POOLSIZE/2; ++n)
            if (max < convout[m][n])
                max = convout[m][n];
    return max;
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

void convpool(double s[WORDLEN][VOCSIZE],
        double mfilter[FILTERNO][FILTERSIZE][FILTERSIZE],
        double se[INPUTNO + 1])
{
    int i, j, k;
    int startpoint = FILTERSIZE/2 + POOLSIZE/2;
    for (i=0; i<FILTERNO; ++i) {
        double convout[WORDLEN][VOCSIZE] = {0};
        double poolout[WORDLEN][VOCSIZE] = {0};

        conv(mfilter[i], s, convout);
        convres(convout);
        pool(convout, poolout);
        poolres(poolout);
        for (j=startpoint; j<WORDLEN-startpoint; ++j) {
            for (k=startpoint; k<VOCSIZE-startpoint; k++)
                se[i*INPUTNO/FILTERNO + (j-startpoint) * (VOCSIZE-startpoint*2) + (k-startpoint)]
                    = poolout[j][k];
        }
    }
}

int getdata(double sentence[MAXINPUTNO][WORDLEN][VOCSIZE])
{
    int i = 0, j = 0, k = 0;

    while (scanf("%lf", &sentence[i][j][k]) != EOF) {
        ++k;
        if (k >= VOCSIZE) {
            k = 0;
            ++j;
            if (j >= WORDLEN) {
                j = 0;
                ++i;
            }
        }
        if (i > MAXINPUTNO) 
            break;
    }
    return i;
}

double forward(double wh[HIDDENNO][INPUTNO+1]
        , double wo[HIDDENNO+1], double hi[], double e[INPUTNO+1])
{
    int i, j;
    double u;
    double o;

    for (i=0; i<HIDDENNO; ++i) {
        u = 0;
        for (j=0; j<INPUTNO; ++j)
            u += e[j] * wh[i][j];
        u -= wh[i][j];
        hi[i] = fs(u);
    }
    o = 0;
    for (i=0; i<HIDDENNO; ++i)
        o += hi[i] * wo[i];
    o -= wo[i];

    return fs(o);
}

void print(double wh[HIDDENNO][INPUTNO+1]
        , double wo[HIDDENNO+1])
{
    int i, j;
    for (i=0; i<HIDDENNO; ++i)
        for (j=0; j<INPUTNO+1; j++)
            printf("%lf ", wh[i][j]);
    printf("\n");

    for (i=0; i<HIDDENNO+1; i++)
        printf("%lf ", wo[i]);
    printf("\n");
}

void readwh(double wh[HIDDENNO][INPUTNO+1])
{
    int i, j;
    for (i=0; i<HIDDENNO; ++i)
        for (j=0; j<INPUTNO+1; ++j)
            scanf("%lf", &wh[i][j]);
}

void readwo(double wo[HIDDENNO+1])
{
    int i;
    for (i=0; i<HIDDENNO+1; ++i)
        scanf("%lf", &wo[i]);
}

double drnd(void) 
{
    double rndno;
    while ((rndno=(double)rand()/RAND_MAX) == 1.0);
    rndno = rndno * 2 - 1;
    return rndno;
}

double fs(double u)
{
    return 1.0 / (1.0+exp(-u));
}

