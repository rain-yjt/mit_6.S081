#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

#define READ_END 0
#define WRITE_end 1

//Function to process prime numbers: Read data from the rfd filter the data, and write the results to the wfd.
void sieve_worker(int rfd,int wfd,int prime){
    int num;
    while(read(rfd,&num,sizeof(int))){
        if(num % prime != 0){
            printf("%d\n",num);
            write(wfd,&num,sizeof(int));
        }
    }

    close(rfd);
    close(wfd);

    exit(0);
}

//// Function to create pipe processes and handle filtering chain
void create_sieve(int rfd,int prime){
    int pfd[2];
    
    pipe(pfd);

    int pid = fork();
    if(pid < 0){
        fprintf(2,"fork error\n");
        exit(0);
    }else if(pid == 0){
        //parent
        close(pfd[WRITE_end]);
        sieve_worker(rfd,pfd[READ_END],prime);
    }else{
        //child
        close(pfd[READ_END]);
        if(prime <= 35)
            create_sieve(pfd[READ_END],prime+1);
        close(pfd[WRITE_end]);
        wait(0);
    }
}

int main(char argc,char *argv[]){
    int pfd[2];

    pipe(pfd);

    int pid = fork();
    if(pid < 0){
        fprintf(2,"fork error\n");
        exit(-1);
    }else if(pid == 0){
        //child
        close(pfd[READ_END]);
        for(int i = 2;i <= 35;++i)
            write(pfd[WRITE_end],&i,sizeof(int));
        close(pfd[READ_END]);
        exit(0);
    }else{
        //parent
        close(pfd[WRITE_end]);
        create_sieve(pfd[READ_END],2);
        close(pfd[READ_END]);
        wait(0);

        exit(0);
    }
}