#include "kernel/types.h"
#include "user.h"

#define READ 0
#define WRITE 1

int main(int argc,char *argv[]){
    //pfd : parent pipe,cfd : child pipe
    int pfd[2],cfd[2],pid;
    char buf[1];

    if(pipe(pfd) < 0 || pipe(cfd) < 0){
        fprintf(2,"pipe error.\n");
        exit(1);
    }

    if((pid = fork()) < 0){
        fprintf(2,"fork error.\n");
        exit(1);
    }else if(pid == 0){
        //child process
        int self_pid = getpid();
        close(pfd[WRITE]);
        close(cfd[READ]);

        read(pfd[READ], buf, 1);
        printf("%d: received ping\n",self_pid);
        write(cfd[WRITE]," ",1);
        close(pfd[READ]);
        close(cfd[WRITE]);
        exit(0);
    }else{
        //parent process
        int self_pid = getpid();
        close(pfd[READ]);
        close(cfd[WRITE]);
        write(pfd[WRITE]," ",1);
        read(cfd[READ], buf, 1);
        printf("%d: received pong\n",self_pid);
        close(pfd[WRITE]);
        close(cfd[READ]);
        exit(0);
    }
}