#if 0
#include<stdio.h>
#include "encode.h"
#include "types.h"
OperationType check_operation_type(char *argv){
    // STEP 1: check argv is -e or not
    // STEP 2: if yes -> return e_encode
    // STEP 3: else -> goto STEP 4

    // STEP 4: check argv is -D or not
    // STEP 5: if yes -> return e_decode
    // STEP 6: else -> goto STEP 4
}
int main(int argc,char *argv[]){
    check_operation_type(argv[1]);
}
#endif
#if 0
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(){
    /*char *s=strchr("text.c",'.');
    printf("%s ",s);
    printf("%d ",strlen(s));*/
    char ch='#';
    char *hex;
    printf("%d ",ch);
    sprintf(hex,"%x",ch);
    printf("%s ",hex);
    int num=atoi(hex);
    printf("%d ",num);
    return 0;
}
#endif