#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


#ifndef alg_h
#define alg_h

#define ASCII 256

bool clear_num();

bool scan_zu(size_t *num0);
bool scan_int(int *num1);

// string operations
void scan_s(unsigned char *str, size_t size);
void free_str(unsigned char **str);
void malloc_str(unsigned char **str, size_t size);
void realloc_str(unsigned char **str, size_t size);
void print_str(unsigned char const *strr, size_t len_);
void print_str_HEX(unsigned char const *str, size_t len_);
char *size_param(size_t size);
////

long double shannon_entropy(unsigned const char *s, size_t length);

// file operations
void File_open_wb(unsigned char const *pth_fl ,FILE **f);
void File_open_rb(unsigned char const *pth_fl ,FILE **f);
void File_close(FILE **f0);

unsigned char const* scan_File_name(unsigned char const *pth);
void scan_File_text(unsigned char *s_str, FILE *fl);
size_t scan_File_size(FILE *f);

void File_write_b(FILE *f, unsigned char *str, size_t str_len);
////

// RLE archiving algorithms
unsigned char* rle_encoder(unsigned char *str_, size_t len_, size_t *newlen_);
unsigned char* rle_decoder(unsigned char *str_, size_t len_, size_t *newlen_);
////

// Huffman archiving algorithms
typedef struct Node {
    unsigned char symbol;
    int freq;
    struct Node *left, *right;
} Node;
typedef struct bBuf {
    size_t len;
    size_t capacity;
    int bPos;
    unsigned char *data;
} bBuf;
typedef struct Code {
    unsigned int bits;
    int len;
} Code;
unsigned char* huf_encoder(unsigned char *str_, size_t len_, size_t *newlen_);
unsigned char* huf_decoder(unsigned char *str_, size_t len_, size_t *newlen_);
////

// LZ77 archiving algorithms
unsigned char* lz77_encoder(unsigned char *str_, size_t len_, size_t *newlen_);
unsigned char* lz77_decoder(unsigned char *str_, size_t len_, size_t *newlen_);
////
unsigned char* coders_test(unsigned char *str_, size_t len_, size_t *newlen_);

#endif