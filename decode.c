#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

Status read_and_validate_decode_args(int argc,char *argv[],DecodeInfo *decInfo){
    if(argc>2){
        if(strcmp(strchr(argv[2],'.'),".bmp")==0){
            decInfo->stego_image_fname=argv[2];
        }
        else{
            fprintf(stderr,"source file %s shold be .bmp",decInfo->stego_image_fname);
            return e_failure;
        }
    }
    else{
        fprintf(stderr,"arguments are missing \n");
        printf("For encoding : ./a.exe -e beautiful.bmp secret.txt stego.bmp\n");
        printf("For decoding : ./a.exe -d output.bmp\n");
        return e_failure;
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo){
    printf("INFO: opening required files\n");
    if(open_file(decInfo)==e_success){
        printf("INFO: Opened %s\n",decInfo->stego_image_fname);
        printf("INFO: Decoding Magic String Signature\n");
        char mag_str[sizeof(MAGIC_STRING)]={};  // declaring the empty string to collect the first 2 char's of stego.bmp
        if(decode_magic_string(mag_str,decInfo->fptr_stego_image)==e_success){
            printf("INFO: Decoding o/p_ext_file_size\n");
            if(decode_secret_file_extn_size(decInfo)==e_success){
                printf("INFO: Decoding o/p_file_ext\n");
                if(decode_secret_file_extn(decInfo->ext_filesize,decInfo)==e_success){
                    printf("INFO: Decoding o/p_file_size\n");
                    if(decode_secret_file_size(decInfo)==e_success){
                        printf("INFO: Decoding o/p_file_data\n");
                        if(decode_secret_file_data(decInfo->size_secret_file,decInfo)==e_success){
                            printf("DECODED WITH NO ERRORS\n");
                        }
                        else{
                           printf("INFO: error in decoding in o/p\n");
                           return e_failure;
                        }
                    }
                    else{
                        printf("INFO: error in decoding in o/p_file_ext\n");
                        return e_failure;
                    }
                }
                else{
                    printf("INFO: error in decoding ext_file_name\n");
                    return e_failure;
                }
            }
            else{
                printf("INFO: error in decoding size of ext.file_name\n");
                return e_failure;
            }
        }
        else{
            printf("INFO: error in decoding the magic string\n");
            return e_failure;
        }
    }
    else
    {
        printf("INFO: error in opening files\n");
        return e_failure;
    }
    return e_success;
}

Status open_file(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
    	return e_failure;
    }

    decInfo->fptr_out_secret = fopen(decInfo->output_fname,"w");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);
    	return e_failure;
    }
    // No failure return e_success
    return e_success;
}

int k=0;
Status decode_magic_string(char *mag_str,FILE *fptr_stego_img){
    fseek(fptr_stego_img,54,SEEK_SET);
    decode_image_to_data(mag_str,fptr_stego_img,strlen(MAGIC_STRING));
    if(strcmp(MAGIC_STRING,mag_str)==0){
        printf("INFO: magic string get matched\n");
        return e_success;
    }
    return e_failure;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo){
    int size=0;
    decode_size_to_data(decInfo,&size);
    decInfo->ext_filesize=size;
    return e_success;
}

Status decode_secret_file_extn(int ext_size, DecodeInfo *decInfo){
    k=0;
    char *ext_name=calloc(ext_size,sizeof(char));
    decode_image_to_data(ext_name,decInfo->fptr_stego_image,ext_size);
    strcpy(decInfo->output_fname,"out");
    strcat(decInfo->output_fname,ext_name);
    //opening file out.txt in write mode
    decInfo->fptr_out_secret=fopen(decInfo->output_fname,"w");
    if(decInfo->fptr_out_secret==NULL){
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
    	return e_failure;
    }
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo){
    int size=0;
    decode_size_to_data(decInfo,&size);
    decInfo->size_secret_file=size;
    return e_success;
}

Status decode_secret_file_data(long size,DecodeInfo *decInfo){
    k=0;
    char *st=calloc(size,sizeof(char));
    decode_image_to_data(st,decInfo->fptr_stego_image,size);
    printf("output name %s\n",st);
    fwrite(st,size,1,decInfo->fptr_out_secret);
    return e_success;
}

void decode_image_to_data(char *str,FILE *fptr_stego_img,int size){
    char arr[8];
    for(int i=0;i<size;i++){
        fread(arr,sizeof(arr),1,fptr_stego_img);
        decode_lsb_to_byte(str,arr);
    }
}

void decode_lsb_to_byte(char *str,char *arr){
    for(int i=0;i<8;i++){
        arr[i]=arr[i]&1;
        str[k]|=(arr[i]<<i);
    }
    printf("%c ",str[k]);
    k++;
}

void decode_size_to_data(DecodeInfo *decInfo,int *size){
    char arr[32];
    //printf("%ld ",ftell(decInfo->fptr_stego_image));
    fread(arr,32,1,decInfo->fptr_stego_image);
    for(int i=0;i<32;i++){
        arr[i]=arr[i]&1;
        *size|=(arr[i]<<i);
    }
    printf("extfile_size -> %d\n",*size);
}