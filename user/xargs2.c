#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    sleep(10);
    
    if (argc < 2) {
        fprintf(2, "Usage: xargs command [args...]\n");
        exit(1);
    }

    char buf[MAXARG];
    char *args[MAXARG];
    int xargc = 0;

    read(0,&buf,MAXARG);
    for (int i = 1; i < argc; i++) {
        args[xargc++] = argv[i];
    } 

    char *p = buf;
    for(int i = 0;i < MAXARG;++i){
        if(buf[i] == '\n'){
            int pid = fork();
            if(pid < 0){
                fprintf(2,"fork error\n");
                exit(0);
            }else if(pid > 0){
                //parent
                p = &buf[i + 1];
                wait(0);
            }else{
                //child
                buf[i] = 0;
                args[xargc++] = p;
                args[xargc++] = 0;

                exec(args[0],args);
                exit(0);
            }
        }
    }

    
}