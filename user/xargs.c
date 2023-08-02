#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

int
main(int argc ,char *argv[]){
    char ch;
    char line[512];
    char* p=line;
    //存放每行命令
    while(read(0,&ch,sizeof(char))==sizeof(char)){
        *p++=ch;
        if(ch=='\n'){
            if(p==line+1)continue;
            *p++='\0';
            
            if(fork()==0){
                //子进程
                p=line;
                int flag=0;
                char* arg=0;
                
                while(*p!='\n'&&*p!='\0'){
                    if(*p==' '){
                        if(flag==1){
                            //新的参数读取结束
                            *p='\0';
                            flag=0;
                            argv[ argc++]=arg;
                        }
                    }
                    else{
                        if(flag==0){
                            //新的参数
                            arg=p;
                            flag=1;
                        }
                    }
                    p++;
                }
                if(flag==1){
                    //参数读取未结束
                    *p='\0';
                    argv[ argc++]=arg;
                }
                argv[ argc]=0;
                exec(argv[1],argv+1);
                break;
            }
            else{
                wait(0);
                p=line;
            }
        }
    }
    
    exit(0);
}