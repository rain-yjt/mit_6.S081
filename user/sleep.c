#include "kernel/types.h" /* the user.h will use this too */
#include "user.h"

int main(int argc,char *argv[]){
    if(argc < 2){
        fprintf(2,"Usage: sleep int\n");
        exit(1);
    }

    sleep(atoi(argv[1]));
    exit(0);
}