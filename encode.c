#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

Status read_and_validate_encode_args(int argc,char *argv[],EncodeInfo *encInfo){
    if(argc>2){
        if(strcmp(strchr(argv[2],'.'),".bmp")==0){
            encInfo->src_image_fname=argv[2];
        }
        else{
            fprintf(stderr,"source image %s shoulde be .bmp\n",encInfo->src_image_fname);
            return e_failure;
        } 
    }
    else{
        fprintf(stderr,"arguments are missing \n");
        printf("For encoding : ./a.exe -e beautiful.bmp secret.txt stego.bmp\n");
        printf("For decoding : ./a.exe -d output.bmp\n");
        return e_failure;
    }
    if(argc>3){
        if(strcmp(strchr(argv[3],'.'),".txt")==0){
            encInfo->secret_fname=argv[3];
        }
        else{
            fprintf(stderr,"secret file %s shoulde be .txt\n",encInfo->secret_fname);
            return e_failure;
        } 
    }
    else{
        fprintf(stderr,"arguments are missing \n");
        printf("For encoding : ./a.exe -e beautiful.bmp secret.txt stego.bmp\n");
        printf("For decoding : ./a.exe -d output.bmp\n");
        return e_failure;
    }
    if(argc>4){
        if(strcmp(strchr(argv[4],'.'),".bmp")==0){
            encInfo->stego_image_fname=argv[4];
        }
        else{
            fprintf(stderr,"dest. image %s shoulde be .bmp\n",encInfo->stego_image_fname);
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

Status do_encoding(EncodeInfo *encInfo){
    //STEP 1: call open_files(encoInfo)
    if(open_files(encInfo)==e_success){
        printf("INFO: opening files successfully\ncheck size of %s\n",encInfo->secret_fname);
        
        if(check_capacity(encInfo)==e_success){
            printf("INFO: Files have enough storage proceed to next step...\n");
            
            if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success){
                printf("INFO: Bmp header copied successfully.\n");
                
                if(encode_magic_string(MAGIC_STRING,encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success){
                    printf("INFO: magic string encoded successfully\n");
                    
                    if(encode_secret_file_extn_size(encInfo->ext_filesize,encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success){
                        printf("INFO: secret file ext size encoded \n");
                        
                        if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success){
                            printf("INFO: secret file ext encoded \n");
                            
                            if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success){
                                printf("INFO: secret file size encoded \n");
                                
                                if(encode_secret_file_data(encInfo->fptr_secret,encInfo)==e_success){
                                    printf("INFO: secret file data encoded successfully \n");
                                    
                                    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success){
                                        printf("INFO: remaining data copied to dest. bmp file successfully\n");
                                    }
                                    else{
                                        printf("INFO: remaining data not encoded yet\n");
                                        return e_failure;
                                    }
                                }
                                else{
                                    printf("INFO: secret file data not encoded yet\n");
                                    return e_failure;
                                }
                            }
                            else{
                                printf("INFO: secret file size not yet encoded\n");
                                return e_failure;
                            }
                        }
                        else{
                            printf("INFO: secret file ext not encoded yet\n");
                            return e_failure;
                        }
                    }
                    else{
                        printf("INFO: secret file ext size doesn't encoded yet\n");
                        return e_failure;
                    }
                }
                else{
                    printf("INFO: magic string doesn't encoded yet\n");
                    return e_failure;
                }
            }
            else{
                printf("INFO: BMP header not copied yet\n");
                return e_failure;
            }
        }
        else{
            printf("INFO : error in opening File ... \n");
            return e_failure;
        }
    }
    else{
        printf("INFO : error in opening File ... \n");
        return e_failure;
    }
    return e_success;
    
    //STEP 2: Check returned success or failure
    //STEP 3: FAILURE -> print error and return e_failure
    //STEP 4: SUCCESS -> GOTO STEP5
    //STEP 5: check_capacity(encInfo)
    //STEP 6: Check returned success or failure
    //STEP 7: FAILURE -> print error and return e_failure 
    //STEP 8: SUCCESS -> GOTO STEP 9
    //STEP 9: call status copy_bmp_header(fptr_src_img,fptr_dest_img);
    //STEP 10: Check returned success or failure
    //STEP 11: FAILURE -> print error return e_failure
    //STEP 12: SUCCESS -> GOTO STEP 13
    //STEP 13: call status encode_magic_string(MAGIC_STRING,&fptr_src_ing,&fptr_stego_img);
    //STEP 14: call encode_secret_file_extn_size(ext_size ,fptr_src_image,fptr_stego_image);
    //STEP 15: call encode_secret_file_size(file_size,encInfo); //for size of secret data
    //STEP 16: call encode_secret_file_data(encInfo);
    //STEP 17: call copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image);
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

char s[1000]; 
Status check_capacity(EncodeInfo *encInfo){
    //STEP 1: Find the size of source img
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    //STEP 2: Find the size of secret file
    /*STEP 3: 16(size of magic string) + 
              32(size of secret file i.e, integer) +
              32(size of extension (eg. .txt -> 32 .c -> 16)) +
              32(size of secret file is an integer ) +
              size of secret file * 8;
    */
   if(encInfo->image_capacity >= ( 54 +(strlen(MAGIC_STRING) + 4 + get_ext_file_size(encInfo)
          + 4 +  encInfo->size_secret_file )*8)){
            return e_success;
          }
   //STEP 4: compare total size & source file(.bmp)
   //STEP 5: if size of .bmp > total size return e_success
   //STEP 6: else return e_failure
   return e_failure;
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_ext_file_size(EncodeInfo *encInfo){
    char *ptr=strchr(encInfo->secret_fname , '.');
    strcpy( encInfo->extn_secret_file , ptr); // copying .txt to (encInfo->extn_secret_file) in strucure by dereferencing
    encInfo->ext_filesize=strlen(ptr); // copy the integer size of ext file 
    return strlen(ptr);
}


uint get_file_size(FILE *fptr){
    uint i;
    for(i=0;fgetc(fptr)!=EOF;i++);
    return i;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
    char buf[54];
    int flag=1;
    rewind(fptr_src_image);
    rewind(fptr_dest_image);
    //STEP 1: Read 54 bytes from src img and store it to buf
    if(fread(buf,sizeof(buf),MAX_SECRET_BUF_SIZE,fptr_src_image)==4){
        flag=0;
    }
    //STEP 2: Write buf data to stego.bmp
    if (fwrite(buf,sizeof(buf),MAX_SECRET_BUF_SIZE,fptr_dest_image)==4){
        flag=1;
    }
    if(flag) return e_success;
    else return e_failure;
}

Status encode_magic_string(const char *magic_string,FILE *fptr_src_image,FILE *fptr_stego_image){
    if(encode_data_to_image(magic_string,strlen(magic_string),fptr_src_image,fptr_stego_image)==e_success){
        return e_success;
    }
    return e_failure;
}

Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image){
    char arr[8];
    //int s=54;
    printf("fptr_src_image -> %ld, fptr_dest_image -> %ld size -> %d \n", ftell(fptr_src_image),ftell(fptr_stego_image),size);
    //run for loop for strlen(magic string) times
    int count=0;
    for(int i=0;i<size;i++){
        //Read 8 bytes of data from src_image and store into arr
        fread(arr,8,1,fptr_src_image);
        if(encode_byte_to_lsb(data[i],arr)==e_success){
            fwrite(arr,8,1,fptr_stego_image);
        }
        else{
            printf("error in reading fptr_src_img in data to image function\n");
        }
        //write encoded array to dast_file
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer){
    //STEP 1: get a bit from pos
    for(int i=0;i<8;i++){
        char n = ((data & (1<<i)) >>i);
        image_buffer[i]=image_buffer[i] & (~1);
        image_buffer[i]=image_buffer[i]|n;
    }
    return e_success;
    //STEP 2: Replace LSB bit of arr[pos]
    //STEP 3: OR operation between arr[pos] | bit(pos)
}

Status encode_secret_file_extn_size(int ext_size ,FILE *fptr_src_image,FILE *fptr_stego_image){
    char arr[32];int flag=0;
    //STEP 1: Read 32 bytes of data from src image and store into arr(for int size no of chars -> .txt file)
    printf("fptr_src_image -> %ld, fptr_dest_image -> %ld ext_size->%d\n", ftell(fptr_src_image),ftell(fptr_stego_image),ext_size);
    if(fread(arr,sizeof(arr),1,fptr_src_image)==4){
        flag=1;
    }
    encode_size_to_lsb(arr,ext_size);
    if(fwrite(arr,sizeof(arr),1,fptr_stego_image)==4){
        flag=0;
    }
    printf("fptr_src_image -> %ld, fptr_dest_image -> %ld ext_size->%d\n",ftell(fptr_src_image),ftell(fptr_stego_image),ext_size);
    if(!flag) return e_success;
    else return e_failure;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo){
    //encode each character and store each char in lsb's of bmp file
    if(encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success){
        return e_success;
    }
    return e_failure;
}

void encode_size_to_lsb(char *arr,int ext_size){
    //STEP 1: Get a bit from pos from size
    //STEP 2: Replace the bit in arr[pos]
    //STEP 3: REpeat 32 times
    for(int i=0;i<32;i++){
        char n = ((ext_size & (1<<i)) >> i) ;
        arr[i]=arr[i] & (~1);
        arr[i]|=n;
    }
}
 
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo){
    // Read 32 bytes from source file and store it in a buffer[32]
    // write the each buffer[i] to the destination file
    char arr[32];int flag=0;
    printf("fptr_src_image -> %ld, fptr_dest_image -> %ld\n", ftell(encInfo->fptr_src_image),ftell(encInfo->fptr_stego_image));
    if(fread(arr,sizeof(arr),1,encInfo->fptr_src_image)==4){
        flag=1;
    }
    encode_size_to_lsb(arr,file_size);
    if(fwrite(arr,sizeof(arr),1,encInfo->fptr_stego_image)==4){
        flag=0;
    }
    printf("fptr_src_image -> %ld, fptr_dest_image -> %ld ext_size->%ld\n", ftell(encInfo->fptr_src_image),ftell(encInfo->fptr_stego_image),file_size);
    if(!flag) return e_success;
    else return e_failure;
}

Status encode_secret_file_data(FILE *fp,EncodeInfo *encInfo){
    fp=fopen(encInfo->secret_fname,"r");
    char c;
    int ind=0;
	for(;(c=fgetc(fp))!=EOF;){
		s[ind++]=c;
	}
	s[ind] = '\0';
    printf("%s \n",s);
    if(encode_data_to_image(s,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success){
        return e_success;
    }
    return e_failure;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest){
    printf("fptr_src_image -> %ld, fptr_dest_image -> %ld \n", ftell(fptr_src),ftell(fptr_dest));
    int ch;
    while((ch=getc(fptr_src))!=EOF){
        putc(ch,fptr_dest);
    }
    return e_success;
}