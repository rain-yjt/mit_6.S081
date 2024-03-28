#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

#define ZERO '0' //可能的素数
#define ONE '1'  //一定不是素数
#define MAXSIZE 36
#define READ 0
#define WRITE 1

//筛选方法(埃氏筛):2~n,第一轮去掉2的倍数，2标记，第二轮去掉2后第一个没有标记的数的倍数..
void sieve(int pipe_read){
    int pi[2];
    if(pipe(pi) < 0){
        //fprintf(2,"pipe error\n");
        exit(0);
    }

    int val = 0;
    char buf[MAXSIZE];
    read(pipe_read,buf,MAXSIZE);
    for(int i = 0;i < MAXSIZE;++i){
        if(buf[i] == ONE){
            val = i;
            break;
        }
    }

    printf("prime %d\n",val);
    buf[val] = ZERO;
    for (int i = val * 2; i < MAXSIZE; i += val) {
        buf[i] = ZERO;
    }

    int pid = fork();
    if(pid < 0){
        fprintf(2,"fork error\n");
        exit(1);
    }else if(pid > 0){
        //parent
        close(pi[READ]);
        write(pi[WRITE],buf,MAXSIZE);
        wait(0);
        close(pipe_read);
        close(pi[WRITE]);
    }else{
        //child
        close(pi[WRITE]);
        sieve(pi[READ]);
    }

}

int main(int argc,char *argv[]){
    int fd[2];
    if(pipe(fd) < 0){
        fprintf(2,"pipe error\n");
        exit(1);
    }

    char buf[MAXSIZE];
    for(int i = 0;i < MAXSIZE;++i){
        buf[i] = ONE;
    }

    int pid = fork();
    if(pid < 0){
        fprintf(2,"fork error\n");
        exit(1);
    }else if(pid > 0){
        //parent
        close(fd[READ]);
        buf[0] = ZERO;
        buf[1] = ZERO; 
        write(fd[WRITE],buf,MAXSIZE);
        close(fd[WRITE]);
        wait(0);
    }else{
        //child
        close(fd[WRITE]);
        sieve(fd[READ]);
    }
    exit(0);

}