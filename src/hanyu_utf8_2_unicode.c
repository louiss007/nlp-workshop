#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TRUE 1
#define FALSE 0

int is2byte(char chr);
int enc_get_utf8_size(const unsigned char* p_input);
int enc_utf8_to_unicode_one(const unsigned char* pInput, unsigned long *Unic);
/*int main()
{
    char chr;
    char s[10] = {'\0'};
    while ((chr = getchar()) != EOF) {
        if (is2byte(chr) == TRUE) {
            putchar(chr);
            putchar(getchar());
        }
    }
    putchar('\n');

    return 0;
}*/

int main()
{
    char filename[] = "1.txt";
    FILE *fp ;
    char str_line[1024];
    if ((fp = fopen(filename, "r")) == NULL) 
    {
        printf("open file failed!\n");
        return -1;
    }
    unsigned long *unic = malloc(128 * sizeof(long));
    int ret = 0;
    while (!feof(fp)) 
    {
        fgets(str_line, 1024, fp);
        ret = enc_utf8_to_unicode_one(str_line, unic);
        printf("%ld\n", *unic);
    }
    free(unic);
    return 0;
}

int is2byte(char c)
{
    if ((c>>8) < 0)
        return TRUE;
    return FALSE;
}

int enc_get_utf8_size(const unsigned char* pInput) 
{
    int i, ret = 0;
    if (NULL == pInput)
        return 0;
    for (i=0; i<8; i++) {
        if (((*pInput) & (0x80>>i)))
            ret++;
        else
            break;
    }
    return ret;
}

int enc_utf8_to_unicode_one(const unsigned char* pInput, unsigned long *Unic)
{
    assert(pInput != NULL && Unic != NULL);

    // b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...
    char b1, b2, b3, b4, b5, b6;

    *Unic = 0x0;
    int utfbytes = enc_get_utf8_size(pInput);
    printf("utfbytes: %d\n", utfbytes);
    unsigned char *pOutput = (unsigned char *) Unic;

    switch ( utfbytes )
    {
        case 0:
            *pOutput     = *pInput;
            utfbytes    += 1;
            break;
        case 2:
            b1 = *pInput;
            b2 = *(pInput + 1);
            if ( (b2 & 0xE0) != 0x80 )
                return 0;
            *pOutput     = (b1 << 6) + (b2 & 0x3F);
            *(pOutput+1) = (b1 >> 2) & 0x07;
            break;
        case 3:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b2 << 6) + (b3 & 0x3F);
            *(pOutput+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
            break;
        case 4:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b3 << 6) + (b4 & 0x3F);
            *(pOutput+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
            *(pOutput+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);
            break;
        case 5:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b4 << 6) + (b5 & 0x3F);
            *(pOutput+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
            *(pOutput+2) = (b2 << 2) + ((b3 >> 4) & 0x03);
            *(pOutput+3) = (b1 << 6);
            break;
        case 6:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            b6 = *(pInput + 5);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
                    || ((b6 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b5 << 6) + (b6 & 0x3F);
            *(pOutput+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
            *(pOutput+2) = (b3 << 2) + ((b4 >> 4) & 0x03);
            *(pOutput+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
            break;
        default:
            return 0;
            break;
    }

    return utfbytes;
}

