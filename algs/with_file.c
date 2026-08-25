#include "alg.h"
#include "../alg.h"

void File_close(FILE **f0)
{
    if (f0==NULL || *f0==NULL) return;

    fclose(*f0);
    *f0=NULL;
}

void File_open_rb(unsigned char const *pth_fl ,FILE **f)
{
    if (*f!=NULL) File_close(f);

    *f=fopen(pth_fl, "rb");
    if (*f==NULL) 
    {
        printf("File_open error.\n");
        return;
    }
}

void File_open_wb(unsigned char const *pth_fl ,FILE **f)
{
    if (*f!=NULL) File_close(f);

    *f=fopen(pth_fl, "wb");
    if (*f==NULL) 
    {
        printf("File_open error.\n");
        return;
    }
}


size_t scan_File_size(FILE *f)
{
    if (f==NULL) return -1;

    fseek(f, 0, SEEK_END);
    size_t size=ftell(f);
    rewind(f);

    return size;
}

void scan_File_text(unsigned char *s_str, FILE *fl)
{
    if (fl==NULL || s_str==NULL) return;

    size_t size_s=scan_File_size(fl);
    size_t byte_r=fread(s_str, 1, size_s, fl);
}

unsigned char const* scan_File_name(unsigned char const *pth)
{
    if (pth==NULL) return "Nan";

    char const *name=pth;
    for (char const *p=pth; *p; p++)
        if (*p== '/' || *p=='\\')
            name=p+1;
    
    return name;
}


void File_write_b(FILE *f, unsigned char *str, size_t str_len)
{
    if (f==NULL || str==NULL) return;

    fwrite(str, sizeof(char), str_len, f);
}

