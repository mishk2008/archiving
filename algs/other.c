#include "alg.h"
#include "../alg.h"

bool clear_num()
{
    int c;
    bool clNum=1;

    while((c=getchar())!='\n' && c!=EOF)
        if (c<'0' || c>'9') clNum=0;


    return clNum;
}

bool scan_zu(size_t *num0)
{
    if (num0==NULL) return 0;

    long long tnum;
    bool flag0=1;

    if (!scanf("%lld", &tnum) || tnum<0 || !(flag0=clear_num())) 
    {
        if (flag0) clear_num();
        return 0;
    }

    *num0=tnum;
    return 1;
}

bool scan_int(int *num1)
{
    if (num1==NULL) return 0;

    int tnum0;
    bool flag00=1;

    if (!scanf("%d", &tnum0) || !(flag00=clear_num()))
    {
        if (flag00) clear_num();
        return 0;
    }

    *num1=tnum0;
    return 1;
}

void scan_s(unsigned char *str, size_t size)
{
    fgets(str, size, stdin);
    size_t len=strlen(str);
    if (len>0 && str[len-1]=='\n') {
        str[len-1]='\0';
    } else clear_num();
}


void free_str(unsigned char **str)
{
    if (*str==NULL) return;

    free(*str);
    *str=NULL;
}

void malloc_str(unsigned char **str, size_t size)
{
    if (str==NULL) {
        printf("pointer error.\n");
        return;
    }

    if (*str!=NULL) 
        free_str(str);

    *str=malloc(size);
    if (*str==NULL)
        printf("malloc error.\n");
}

void realloc_str(unsigned char **str, size_t size)
{
    if (str==NULL) {
        printf("pointer error.\n");
        return;
    }

    if (size==0) {
        free_str(str);
        return;
    }

    unsigned char *temp=realloc(*str, size);
    if (temp==NULL) {
        printf("realloc error.\n");
        return;
    }

    *str=temp;
}

void print_str(unsigned char const *strr, size_t len_)
{
    if (strr==NULL) return;

    for (size_t i=0; i<len_; i++)
        printf("%c", strr[i]);
    printf("\n");
}

void print_str_HEX(unsigned char const *str, size_t len_)
{
    if (str==NULL) return;

    for (size_t i=0; i<len_; i++) printf("%02X ", str[i]);
    printf("\n");
}

long double shannon_entropy(const unsigned  char *s, size_t length)
{
    if (s==NULL) return -1;

    int num_letter[ASCII];
    for (size_t i=0; i<ASCII; i++) num_letter[i]=0;

    for (size_t i=0; i<length; i++) 
        num_letter[(unsigned char)s[i]]++;
 

    long double probability_letter[ASCII];
    for (size_t i=0; i<ASCII; i++) probability_letter[i]=0; 

    long double entropy=0;
    for (size_t i=0; i<ASCII; i++)
        if (num_letter[i]>0) 
        {
            probability_letter[i]=(long double)num_letter[i]/(long double)length;
            entropy+=probability_letter[i]*log2(1/probability_letter[i]);
        }

    return entropy;
}

char *size_param(size_t size)
{
    static char buf[100];
    if (size<1000) {
        snprintf(buf, sizeof(buf), "%zu B", size);
    } else if (size<1000000) {
        snprintf(buf, sizeof(buf), "%.3f KB", (double)size/1000.0);
    } else if (size<1000000000) {
        snprintf(buf, sizeof(buf), "%.3f MB", (double)size/1000000.0);
    } else {
        snprintf(buf, sizeof(buf), "%.3f GB", (double)size/1000000000.0);
    }
    
    return buf;
}

unsigned char* coders_test(unsigned char *str_, size_t len_, size_t *newlen_)
{
    if (str_==NULL || len_==0 || newlen_==NULL) return NULL;

    unsigned char *temp=NULL;

    printf("\n+------------+------------+------------+------------+\n");
    printf("| BASIC SIZE |    RLE     |    LZ77    |   HUFFMAN  |\n");
    printf("+------------+------------+------------+------------+\n");

    printf("| %10s ", size_param(len_));

    temp=rle_encoder(str_, len_, newlen_);
    printf("| %10s ", size_param(*newlen_));
    free_str(&temp);

    temp=lz77_encoder(str_, len_, newlen_);
    printf("| %10s ", size_param(*newlen_));
    free_str(&temp);   

    temp=huf_encoder(str_, len_, newlen_);
    printf("| %10s |\n", size_param(*newlen_));
    free_str(&temp);

    printf("+------------+------------+------------+------------+\n");

    return NULL;
}
