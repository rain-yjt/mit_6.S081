#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user.h"

#define MAX_PATH 256

void find(char *path,char *target);
char* fmtname(char *path);

int main(char argc,char *argv[]){
    if(argc != 3){
        fprintf(2,"Usage: find <directory><target_name>\n");
        exit(1);
    }
    find(argv[1],argv[2]);
    exit(0);
}

char* fmtname(char *path) {
    char *p;
	for(p = path + strlen(path);p >= path && *p != '/';p--)
		;
	p++;
	
	return p;
}

void find(char *path,char *target){
    char buf[MAX_PATH],*p;
    struct dirent di;
    struct stat st;
    int fd;

    if((fd = open(path,0)) < 0){
        fprintf(2,"find：cannot open %s\n",path);
        return;
    }

    if(fstat(fd,&st) < 0){
        fprintf(2,"find:cannot fstat %s\n",path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE: //file
            if(strcmp(fmtname(path),target) == 0){
                printf("%s\n",path);
            }
            break;
        case T_DIR: //directory, 1:string end flag '\0'
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){            
                fprintf(2,"find: path too long\n");
                break;
            }
            strcpy(buf,path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd,&di,sizeof(di)) == sizeof(di)){
                if (di.inum == 0 || strcmp(di.name, ".") == 0 || strcmp(di.name, "..") == 0)
                    continue;
                memmove(p,di.name,DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf,&st) < 0){
                    fprintf(2,"find: cannot stat\n");
                    continue;
                }
                find(buf,target);
            }
            break;
    }
    close(fd);


    
}


//another method
// void find(char *path, char *target) {
//     char buf[512];
//     int fd;
//     struct dirent de;
//     struct stat st;
    
//     if ((fd = open(path, 0)) < 0) {
//         fprintf(2, "find: cannot open %s\n", path);
//         return;
//     }
    
//     if (fstat(fd, &st) < 0) {
//         fprintf(2, "find: cannot stat %s\n", path);
//         close(fd);
//         return;
//     }
    
//     switch (st.type) {
//         case T_DIR:
//             if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
//                 printf("find: path too long\n");
//                 break;
//             }
//             strcpy(buf, path);
//             char *p = buf + strlen(buf);
//             *p++ = '/';
            
//             while (read(fd, &de, sizeof(de)) == sizeof(de)) {
//                 if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
//                     continue;
//                 }
                
//                 memmove(p, de.name, DIRSIZ);
//                 p[DIRSIZ] = 0;
                
//                 if (stat(buf, &st) < 0) {
//                     printf("find: cannot stat %s\n", buf);
//                     continue;
//                 }
                
//                 if (st.type == T_FILE && strcmp(de.name, target) == 0) {
//                     printf("%s/%s\n", path, de.name);
//                 }
                
//                 if (st.type == T_DIR) {
//                     find(buf, target); // recursively find in subdirectory
//                 }
//             }
//             break;
//     }
    
//     close(fd);
// }


