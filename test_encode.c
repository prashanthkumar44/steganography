#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

OperationType check_operation_type(char *argv){
    // STEP 1: check argv is -e or not
    // STEP 2: if yes -> return e_encode
    if(!strcmp(argv,"-e")){
        return e_encode;
    }
    // STEP 3: else -> goto STEP 4
    // STEP 4: check argv is -D or not
    else if(!strcmp(argv,"-d")){
        return e_decode;
    }
    // STEP 5: if yes -> return e_decode
    else{
        return e_unsupported;
    }
    // STEP 6: else -> return e_unsupported
}

int main(int argc,char *argv[])
{
    // select encode or decode by giving -e or -d
    // ./a.exe -e beautiful.bmp secret.txt stego.bmp
    // ./a.exe -d stego.bmp 
    EncodeInfo encInfo; // structure declaration
    DecodeInfo decInfo;
    int val=check_operation_type(argv[1]);

    if(val == e_encode){
        printf("encoding started !\n");
        // print encode is started
        if(read_and_validate_encode_args(argc,argv,&encInfo)==e_success){
            printf("INFO: read_and_validate_encode_arguments done \n");
            if(do_encoding(&encInfo)==e_success){ // pass the address of the structure
                printf("INFO : Files are encoded successfully ... \n");
                fclose(encInfo.fptr_src_image);
                fclose(encInfo.fptr_secret);
                fclose(encInfo.fptr_stego_image);
            }
            else{
                printf("INFO : error in encoding in STEP 1... \n");
                return e_failure;
            }
        }
        else
        {
            fprintf(stderr,"error in read and validation in encoding\n");                
            return e_failure;
        }
    }
    else if(val == e_decode){
        printf("decoding has just started\n");
        if(read_and_validate_decode_args(argc,argv,&decInfo)==e_success){
            printf("INFO: read_and_validate_encode_arguments done \n");
            if(do_decoding(&decInfo)==e_success){ // pass the address of the structure
                printf("INFO : Files are decoded successfully ... \n");
                fclose(decInfo.fptr_stego_image);
                fclose(decInfo.fptr_out_secret);
            }
            else{
                printf("INFO : error in decoding in STEP 1... \n");
                return e_failure;
            }
        }
        else
        {
            fprintf(stderr,"error in read and validation in decoding\n");
            return e_failure;
        }
    }
    else{
        printf("unsupported !\n");
        return e_failure;
    }
    return 0;
}