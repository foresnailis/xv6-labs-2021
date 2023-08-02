#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

#define RD 0 //输入流
#define WR 1 //输出流

int new_proc(int prime){
    printf("prime %d\n",prime);
    int buf=0;
    int res_len=0;

    while((res_len = read(RD,&buf,sizeof(int))) == sizeof(int)){
        if (buf % prime != 0){
            break;//从左进程输入的数字中，出现了不能被本进程数字整除的数字，因此需要右进程
        }
    }
    if(res_len>0){ 
        //读到了结果，说明需要另外一个子进程
        int fds[2];
        pipe(fds);
        int pid=fork();
        if(pid == 0){
            close(RD);
            dup(fds[RD]);
            //设置子进程输入流
            close(fds[RD]);
            close(fds[WR]);
            new_proc(buf);
        }
        else{
            close(fds[RD]);
            while((res_len = read(RD,&buf,sizeof(int))) == sizeof(int)){
                if (buf % prime != 0){
                    write(fds[WR],&buf,sizeof(int));
                }
            }
            close(fds[WR]);
            wait(0);
        }
    }
    //到达此处，说明未读到结果，左进程的管道已经关闭。终止当前进程。
    exit(0);
}

int
main(int argc,char *argv[]){
    printf("prime %d\n",2);
    int fds[2];
    pipe(fds);
    int pid=fork();
    if(pid == 0){
        close(RD);
        dup(fds[RD]);
        //设置子进程输入流
        close(fds[RD]);
        close(fds[WR]);
        new_proc(3);
    }
    else{
        close(fds[RD]);
        for(int i=3;i<=35;++i){
            if (i % 2 != 0){
                write(fds[WR],&i,sizeof(int));
            }
        }
        close(fds[WR]);
        wait(0);
    }   
    exit(0);
}