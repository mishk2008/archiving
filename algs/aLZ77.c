#include "alg.h"
#include "../alg.h"

unsigned char* lz77_encoder(unsigned char *str_, size_t len_, size_t *newlen_)
{
    if (str_==NULL || len_==0 || newlen_==NULL) return NULL;
    
    unsigned char *temp=NULL;
    malloc_str(&temp, 3*len_);
    if (temp==NULL) return NULL;
    
    size_t num_token=0, pos=0;
    size_t len_1=len_-1;

    while (pos<len_)
    {    
        int length=0, offset=0, temp_length=0;
        unsigned char symbol=str_[pos];

        size_t offset_limit=(pos>255)?pos-255:0;

        for (size_t i=offset_limit; i<pos; i++) {
            
            temp_length=0;

            while (temp_length<255 && temp_length+pos<len_1 && str_[i+temp_length]==str_[pos+temp_length]) {
                temp_length++;
            }

            if (temp_length>0 && temp_length>length) {
                length=temp_length;
                offset=pos-i;
                symbol=str_[pos+length];

            }
        }

        temp[num_token*3]=(unsigned char)offset;
        temp[num_token*3+1]=(unsigned char)length;
        temp[num_token*3+2]=symbol;
        num_token++;

        pos+=length+1;
    }
    *newlen_=num_token*3;

    realloc_str(&temp, *newlen_);
    if (temp==NULL) return NULL;

    return temp;
}

unsigned char* lz77_decoder(unsigned char *str_, size_t len_, size_t *newlen_)
{
    if (str_==NULL || len_==0 || newlen_==NULL) return NULL;

    if ((len_) % 3!=0) {
        printf("lz77_len error.\n");
        return NULL;
    }

    size_t true_length=0;
    for (size_t i=1; i<len_; i+=3) {
        true_length+=(unsigned char)str_[i]+1;
    }

    if (true_length>(size_t)(4*10e9)) {
        printf("true_length error.\n");
        return NULL;
    }

    unsigned char *temp=NULL;
    malloc_str(&temp, true_length);
    if (temp==NULL) return NULL;

    size_t temp_pos=0;
    for (size_t i=0; i<len_; i+=3) {
        unsigned char offset=str_[i], length=str_[i+1], symbol=str_[i+2];
        
        if (offset>temp_pos) {
            printf("offset/length error.\n");
            free_str(&temp);
            return NULL;
        }

        unsigned char temp_length=0;
        while(temp_length<length) {
            temp[temp_pos]=temp[temp_pos-offset];
            temp_length++;
            temp_pos++;
        }
        temp[temp_pos++]=symbol;
    }
    *newlen_=true_length;

    return temp;
}
