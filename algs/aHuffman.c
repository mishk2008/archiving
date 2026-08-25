#include "alg.h"
#include "../alg.h"


void get_bit(bBuf *bitbuf, int bit)
{
    if (bitbuf==NULL) return;

    if ((*bitbuf).capacity==0) (*bitbuf).capacity=4096;

    if ((*bitbuf).bPos==0)
    {
        if ((*bitbuf).data==NULL || (*bitbuf).len>=(*bitbuf).capacity) {
            (*bitbuf).capacity*=2;
            realloc_str(&(*bitbuf).data, (*bitbuf).capacity);
            if ((*bitbuf).data==NULL) {
                printf("get_bit realloc error.\n");
                (*bitbuf).capacity=0;
                return;
            }
        }
        (*bitbuf).data[(*bitbuf).len]=0;
        (*bitbuf).len++;
        (*bitbuf).bPos=8;
    }
    (*bitbuf).bPos--;
    
    if (bit) (*bitbuf).data[(*bitbuf).len-1]|=(1<<(*bitbuf).bPos);
}

void get_bitet_tree(bBuf *bitbuf, Node *nodes)
{
    if (bitbuf==NULL || nodes==NULL) return;

    if ((*nodes).left==NULL && (*nodes).right==NULL) {
        get_bit(bitbuf, 1);
        for (int i=7; i>=0; i--)
            get_bit(bitbuf, ((*nodes).symbol>>i) & 1);
    } else {
        get_bit(bitbuf, 0);
        get_bitet_tree(bitbuf, (*nodes).left);
        get_bitet_tree(bitbuf, (*nodes).right);        
    }
}

void get_bitet_symbols(Code *codes, Node *nodes, unsigned int current_bits, unsigned char current_len)
{
    if (nodes==NULL) return;

    if ((*nodes).left==NULL && (*nodes).right==NULL) {
        codes[(*nodes).symbol].bits=current_bits;
        codes[(*nodes).symbol].len=current_len;
    } else {
        if ((*nodes).left!=NULL) get_bitet_symbols(codes, (*nodes).left, current_bits<<1, current_len+1);
        if ((*nodes).right!=NULL) get_bitet_symbols(codes, (*nodes).right, (current_bits<<1)|1, current_len+1);
    }
}

void free_tree(Node *nodes)
{

    if (nodes==NULL) return;

    free_tree((*nodes).left);
    free_tree((*nodes).right);

    free(nodes);
}

int read_bit(unsigned char *data, size_t *byte_pos, int *bit_pos, size_t data_len)
{
    if (data==NULL || (*byte_pos)>=data_len) return -1;

    int bit=(data[*byte_pos] >> (7-(*bit_pos))) & 1;
    (*bit_pos)++;

    if (*bit_pos>=8) {
        *bit_pos=0;
        (*byte_pos)++;
    }

    return bit;
}

Node* rebuild_tree(unsigned char *data, size_t *byte_pos, int *bit_pos, size_t data_len)
{
    if (data==NULL) return NULL;

    int bit=read_bit(data, byte_pos, bit_pos, data_len);
    if (bit==-1) return NULL;

    if (bit==1) {
        unsigned char symbol=0;
        for (int i=7; i>=0; i--) {
            int b=read_bit(data, byte_pos, bit_pos, data_len);
            if (b==-1) return NULL;
            if (b==1) symbol |= (1 << i); 
        }

        Node *node=(Node*)malloc(sizeof(Node));
        (*node).symbol=symbol;
        (*node).freq=0;
        (*node).left=NULL;
        (*node).right=NULL;
        return node;
    } else {
        Node *node=(Node*)malloc(sizeof(Node));
        (*node).symbol=0;
        (*node).freq=0;
        (*node).left=rebuild_tree(data, byte_pos, bit_pos, data_len);
        (*node).right=rebuild_tree(data, byte_pos, bit_pos, data_len);
        return node;    
    }
}


unsigned char* huf_encoder(unsigned char *str_, size_t len_, size_t *newlen_)
{
    if (str_==NULL || len_==0 || newlen_==NULL) return NULL;

    int freq[ASCII]={0};
    for (size_t i=0; i<len_; i++)
        freq[(unsigned char)str_[i]]++;

    Node *nodes[ASCII];
    size_t uniqSymbols=0;
    for (size_t i=0; i<ASCII; i++)
        if (freq[i]>0)
        {
            Node *parent=(Node*)malloc(sizeof(Node));
            (*parent).symbol=(unsigned char)i;
            (*parent).freq=freq[i];
            (*parent).left=NULL;
            (*parent).right=NULL;
            nodes[uniqSymbols++]=parent;
            //printf("%c -> %d \n", i, freq[i]);
        }

    if (uniqSymbols==1) {
        Node *parent=(Node*)malloc(sizeof(Node));
        (*parent).freq=(*nodes[0]).freq;
        (*parent).symbol=0;
        (*parent).left=nodes[0];
        (*parent).right=NULL;
        nodes[0]=parent;
    } else {
        while (uniqSymbols>1)
        {
            size_t min1=0;
            for (size_t i=1; i<uniqSymbols; i++)
                if ((*(nodes[i])).freq<(*(nodes[min1])).freq) min1=i;

            size_t min2=(min1==0)?1:0;
            for (size_t i=0; i<uniqSymbols; i++)
                if (i!=min1 && (*(nodes[i])).freq<(*(nodes[min2])).freq) min2=i;

            Node *parent=(Node*)malloc(sizeof(Node));
            (*parent).freq=(*nodes[min1]).freq+(*nodes[min2]).freq;
            (*parent).symbol=0;
            (*parent).left=nodes[min1];
            (*parent).right=nodes[min2];

            nodes[min1]=parent;
            nodes[min2]=nodes[--uniqSymbols];
        }
    }

    bBuf bitet_text={0, 4096, 0, NULL};
    get_bitet_tree(&bitet_text, nodes[0]);

    while (bitet_text.bPos!=0) get_bit(&bitet_text, 0);
    size_t tree_size=bitet_text.len;

    Code codes[ASCII]={0};
    get_bitet_symbols(codes, nodes[0], 0, 0);

    /*
    for (size_t i=0; i<ASCII; i++) 
        if (codes[i].len>0) {
            printf("%c -> ", i);
            for(size_t j=0; j<codes[i].len; j++) {
                int bit=(codes[i].bits>>(codes[i].len-1-j))&1;
                if (bit==1) printf("1"); else printf("0");
            }            
            printf("\n");
        }
    */

            
    for (size_t i=0; i<len_; i++)
    {
        Code code=codes[(unsigned char)str_[i]];
        for (int j=0; j<code.len; j++) {
            int bit=(code.bits>>(code.len-1-j))&1;
            get_bit(&bitet_text, bit);
        }
    }
    while (bitet_text.bPos!=0) get_bit(&bitet_text, 0);

    size_t num_symbols=0;
    for (size_t i=0; i<ASCII; i++) 
        if (freq[i]!=0) num_symbols+=freq[i];

    unsigned char *temp=NULL;
    malloc_str(&temp ,bitet_text.len+2*sizeof(size_t));
    if (temp==NULL) return NULL;

    memcpy(temp, &tree_size, sizeof(size_t));
    memcpy(temp+sizeof(size_t), &num_symbols, sizeof(size_t));

    memcpy(temp+2*sizeof(size_t), bitet_text.data, bitet_text.len);

    *newlen_=bitet_text.len+2*sizeof(size_t);             

    free(bitet_text.data);
    free_tree(nodes[0]);

    return temp;
}

unsigned char* huf_decoder(unsigned char *str_, size_t len_, size_t *newlen_)
{
    if (str_==NULL || len_==0 || newlen_==NULL) return NULL;

    size_t tree_size=0;
    size_t num_symbols=0;
    memcpy(&tree_size, str_, sizeof(size_t));
    memcpy(&num_symbols, str_+sizeof(size_t), sizeof(size_t));

    if (tree_size>4*10e9) {
        printf("tree_size error.\n");
        return NULL;
    }

    size_t byte_pos=0;
    int bit_pos=0;
    Node *nodes=rebuild_tree(str_+2*sizeof(size_t), &byte_pos, &bit_pos, tree_size);
    if (nodes==NULL) {
        printf("rebuild_tree error.\n");
        return NULL;
    } 

    unsigned char *encoded_data=str_+2*sizeof(size_t)+tree_size;
    size_t encoded_len=len_-(2*sizeof(size_t)+tree_size);

    unsigned char *result=malloc(num_symbols);
    if (result==NULL) {
        free_tree(nodes);
        return NULL;
    }

    size_t result_len=0;
    Node *current=nodes;

    byte_pos=0;
    bit_pos=0;

    while (result_len<num_symbols && byte_pos<encoded_len) {
        int direction=read_bit(encoded_data, &byte_pos, &bit_pos, encoded_len);
        if (direction==-1) break;
        
        if (direction==0) current=(*current).left;
        else current=(*current).right;

        if (current!=NULL && (*current).left==NULL && (*current).right==NULL) {
            result[result_len++]=(*current).symbol;
            current=nodes;
        }

        if (current==NULL) {
            printf("NULL tree pointer.\n");
            free_tree(nodes);
            free(result);
            return NULL;
        }
    }

    if (result_len==0) {
        printf("decode error.\n");
        return NULL;
    }

    *newlen_=result_len;
    free_tree(nodes);
    return result;
}
