#include "alg.h"

int main()
{
    unsigned char* (*codec_[])(unsigned char* str0, size_t size0, size_t *new_size0)=
       {rle_encoder, rle_decoder, 
        lz77_encoder, lz77_decoder,        
        huf_encoder, huf_decoder,
        coders_test};

    FILE *choosed_file=NULL;
    unsigned char *fText=NULL;
    unsigned char file_path[100];
    unsigned char file_save[100];
    size_t fLength;
    unsigned char *fText_copy=NULL;
    size_t fNewLength=0;

    int MainCycle1=1;
    while (MainCycle1)
    {
        MainCycle1=0;
        int MainCycle2=1;

        printf("Choose file by path : ");
        scan_s(file_path, 100);
    
        
        File_open_rb(file_path, &choosed_file);
        if (choosed_file==NULL || (fLength=scan_File_size(choosed_file))==0) {
            MainCycle2=0;    

            printf("Error, choosed empty file.\n");
        } else {
            malloc_str(&fText, fLength);
            if (fText==NULL) {
                printf("Error, choosed to big file.\n");

                MainCycle2=0;
            }

            scan_File_text(fText, choosed_file);            
        }

        //print_str(fText, fLength);

        printf("\nEntropy of text == %.5LF\n", shannon_entropy(fText, fLength));

        while (MainCycle2)
        {
            MainCycle2=0;

            int task;
            printf("\nChoose task to do.\n\nRLE_ENCODER  == 0\nRLE_DECODER  == 1\n\nLZ77_ENCODER  == 2\nLZ77_DECODER  == 3\n\nHUF_ENCODER == 4\nHUF_DECODER == 5\n\nCODERS_TEST == 6\n\nInput : ");
            while (!scan_int(&task)) 
                printf("Wrong input for int. Rewrite : ");     

            if (task>=0 && task<=5) {
                unsigned char *fText_copy=NULL;
                fText_copy=codec_[task](fText, fLength, &fNewLength);

                if (fText_copy==NULL || fNewLength==0) {
                    printf("codec_ error.\n");
                    break;
                }

                //printf("\nFile text:\n");
                //print_str_HEX(fText_copy, fNewLength);

                printf("\nFile size characteristics:\n");
                printf("Size of input file == %s\n", size_param(fLength));
                printf("Size of output file == %s\n", size_param(fNewLength));

                int task_save;
                printf("Save file? ( y==1 / n==0 ) : ");
                while (!scan_int(&task_save)) 
                    printf("Wrong input for int. Rewrite : ");
                if (task_save==1) 
                {
                    printf("\nChoose file to save text : ");
                    scan_s(file_save, 100);

                    File_open_wb(file_save, &choosed_file);
                    if (choosed_file==NULL) {
                        free_str(&fText_copy);
                        break;
                    }

                    File_write_b(choosed_file, fText_copy, fNewLength);                    
                }

                free_str(&fText_copy);

            } else if (task==6) {
                unsigned char *fText_copy=NULL;
                fText_copy=codec_[task](fText, fLength, &fNewLength);

                if (fNewLength==0) {
                    printf("codec_ error.\n");
                    break;
                }                
            }
            else printf("Wrong task choose.\n");


            printf("Continue work with text of choosed file? ( y==1 / n==0 ) : ");
            while (!scan_int(&MainCycle2)) 
                printf("Wrong input for int. Rewrite : ");
        }


        File_close(&choosed_file);
        free_str(&fText);

        printf("Continue work? ( y==1 / n==0 ) : ");
        while (!scan_int(&MainCycle1))
            printf("Wrong input for int. Rewrite : ");
    }

    return 0;
}

// gcc -o k.exe *.c algs/*.c

//  C:\Users\muham\OneDrive\Desktop\kursovaya\code\the_code\text_tests\