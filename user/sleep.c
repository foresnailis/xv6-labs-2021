#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

int                                                                                                                                                                                                                                                                                                                                                                   
main(int argc,char *argv[]){
    if(argc!=2){
        printf("参数错误!\n");
        exit(1);
    }
    else{
        int number = atoi(argv[1]);
        sleep(number);
        exit(0);
    }
}