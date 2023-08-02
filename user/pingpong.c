#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

#define RD 0 //输入流
#define WR 1 //输出流

int
main(int argc,char *argv[]){
    int fds[2];
    pipe(fds);
    int fds_2[2];
    pipe(fds_2);
    int pid = fork();
    if(pid==0){
        //子进程
        close(fds[WR]);
        close(fds_2[RD]);
        char ch;
        read(fds[RD],&ch,sizeof(char));
        if(ch=='A'){
            printf("%d: received ping\n",getpid());
            write(fds_2[WR],&ch,sizeof(char));
        }
        else{
            printf("%c\n",ch);
        }
        close(fds[RD]);
        close(fds_2[WR]);
    }
    else{
        //父进程
        close(fds[RD]);
        close(fds_2[WR]);
        char ch='A';
        write(fds[WR],&ch,sizeof(char));
        ch='B';
        read(fds_2[RD],&ch,sizeof(char));
        if(ch=='A'){
            printf("%d: received pong\n",getpid());
        }
        else{
            printf("%c\n",ch);
        }
        close(fds[WR]);
        close(fds_2[RD]);

    }
    exit(0);
}

