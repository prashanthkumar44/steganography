#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 5

typedef struct _DecodeInfo
{
    /* Source Image (stego.bmp ) info */
    char *stego_image_fname;  // source img filename
    FILE *fptr_stego_image;   // address of source img file
    uint image_capacity;
    
/* Secret File Info */
    char output_fname[10]; //secret data file name
    FILE *fptr_out_secret; // address of secret file
    uint ext_filesize; //secret ext file size
    char extn_secret_file[MAX_FILE_SUFFIX]; // extension of secret file Eg : .pdf,.txt etc.,
    // char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;  // size of secret file

} DecodeInfo;

/* Encoding function prototype */

/* read and validate decode args */
Status read_and_validate_decode_args(int argc,char *argv[],DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get file pointers i/p img (bmp) file */
Status open_file(DecodeInfo *decInfo);

/* Decoding magic string */
Status decode_magic_string(char *mag_str,FILE *fptr_stego_img);

/* Decode o/p file extenstion size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(int ext_size, DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(long int,DecodeInfo *decInfo);

/* Decode function for image to data */
void decode_image_to_data(char *mag_str,FILE *fptr_stego_img,int size);

/* Decoding for lsb to byte */
void decode_lsb_to_byte(char *mag_str,char *arr);

/* Decoding ext_file_size */
void decode_size_to_data(DecodeInfo *decInfo,int *size);

#endif