#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

#define READ 0
#define WRITE 1

void sieve(int rfd){
    int fd[2];
    
    if(pipe(fd) < 0){
        fprintf(2,"pipe error\n");
        exit(0);
    }

    int ret = 0,num = 0, prime = 0;
    ret = read(rfd,&num,sizeof(int));
    if(ret > 0){
        prime = num;
        printf("prime %d\n",prime);
    }else{
        exit(0);
    }

    int pid = fork();
    if(pid == 0){
        //child
        close(fd[WRITE]);
        sieve(fd[READ]);
    }else{
        //parent
        close(fd[READ]);
        while(ret > 0){
            if(num % prime){
                write(fd[WRITE],&num,sizeof(int));
            }
            ret = read(rfd,&num,sizeof(int));   //read next prime
        }
        close(rfd);
        close(fd[WRITE]);

    }
}

int main(){
    int fd[2];
    if(pipe(fd) < 0){
        fprintf(2,"pipe error\n");
        exit(1);
    }

    int pid = fork();
    if(pid < 0){
        fprintf(2,"fork error\n");
    }else if(pid == 0){
        //child
        close(fd[WRITE]);
        sieve(fd[READ]);
    }else{
        //parent
        close(fd[READ]);
        printf("prime 2\n");
        for(int i = 2;i <= 35;i++){
            if(i % 2){
                write(fd[WRITE],&i,sizeof(int));
            }
        }
        close(fd[WRITE]);
    }

    wait(0);
    exit(0);
}