#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 5

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname;  // source img filename 
    FILE *fptr_src_image;   // address of source img file
    uint image_capacity;    // size of source img
    //uint bits_per_pixel;
    //char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname; //secret data file name 
    FILE *fptr_secret; // address of secret file
    uint ext_filesize; //secret ext file size
    char extn_secret_file[MAX_FILE_SUFFIX]; // extension of secret file Eg : .pdf,.txt etc.,
    // char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;  // size of secret file

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get extension size */
uint get_ext_file_size(EncodeInfo *encInfo); // gives the no of chars in extension i.e, 4 -> .txt

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(const char *magic_string, FILE *fptr_src_ing,FILE *fptr_stego_img);

/* Encode secret file extenstion size */
Status encode_secret_file_extn_size(int ext_size ,FILE *fptr_src_image,FILE *fptr_stego_image);

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(FILE *fileptr,EncodeInfo *encInfo);

/* Encodes each byte in size into LSB of image data array */
void encode_size_to_lsb(char *arr,int ext_size);

/* Encode function, which does the real encoding */
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
