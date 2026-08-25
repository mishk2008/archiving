#include "alg.h"
#include "../alg.h"

unsigned char* rle_encoder(unsigned char *str_, size_t len_, size_t *newlen_)
{
    if (str_==NULL || len_==0 || newlen_==NULL) return NULL;

    unsigned char *temp=NULL;
    malloc_str(&temp, 2*len_);
    if (temp==NULL) return NULL; 


    size_t pos_=0, temp_pos_=0, run;

    while (pos_<len_)
    {
        run=1;
        unsigned char element=str_[pos_];
        while (pos_+run<len_ && str_[pos_+run]==element && run<255)
            run++;

        temp[temp_pos_++]=element;
        temp[temp_pos_++]=run;
        pos_+=run;
    }
    *newlen_=temp_pos_;
    realloc_str(&temp, temp_pos_);

    return temp;
}

unsigned char* rle_decoder(unsigned char *str_, size_t len_, size_t *newlen_)
{
    if (str_==NULL || len_==0 || newlen_==NULL) return NULL;

    if ((len_) % 2!=0) {
        printf("rle_len error.\n");
        return NULL;
    }

    unsigned char *temp=NULL;
    size_t len_temp=0;
    for (size_t i=1; i<len_; i+=2)
        len_temp+=(size_t)str_[i];

    malloc_str(&temp, len_temp+1);

    size_t pos_=0;
    for (size_t i=0; i<len_; i+=2)
        for (size_t j=1; j<=str_[i+1]; j++)
            temp[pos_++]=str_[i];
    *newlen_=pos_;

    return temp;
}

