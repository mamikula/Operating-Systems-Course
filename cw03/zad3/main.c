#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

void search_directories(char* directory_name, char* str, int depth){

    if(depth == 0) exit(0); //stop condition
    DIR* dir;
    FILE* file;
    struct dirent* dp;

//    struct dirent {
//        ino_t          d_ino;       /* inode number */
//        off_t          d_off;       /* offset to the next dirent */
//        unsigned short d_reclen;    /* length of this record */
//        unsigned char  d_type;      /* type of file; not supported
//                                   by all file system types */
//        char           d_name[256]; /* filename */
//    };

    char* path = calloc(sizeof(char), 256);
    char* eoi; //end of inscription
    bool contains;

    if(((dir = opendir(directory_name)) == NULL)){
        exit(1);
    }

    while((dp = readdir(dir))){
        if ((strcmp(dp->d_name, "..") == 0) || (strcmp(dp->d_name, ".") == 0)) continue;

        char* new_path = calloc(sizeof(char), strlen(directory_name) + strlen(dp->d_name));
        sprintf(new_path, "%s/%s", directory_name, dp->d_name);
        path = new_path;
        eoi = strchr(dp->d_name, '.');

        if(dp->d_type == DT_DIR && fork() == 0){
            search_directories(path, str, depth - 1);
            exit(0);
        }

        else if(eoi != NULL && strcmp(eoi, ".txt") == 0){

            file = fopen(path, "r");

            char* line = calloc(sizeof(char), 256);

            while(fgets(line, 256*sizeof(char), file)){
                if(strstr(line, str)) contains = true;
            }
            if(contains){
                printf("\npath %s\n", path);
                printf(" CHILD PID: %d, parent PID: %d\n", getpid(), getppid());
                contains = false;
            }
        }
        wait(NULL);
        free(path);
    }
    closedir(dir);
}

int main(int argc, char* argv[]){
    if(argc != 4) exit(1);
    char* directory_name = argv[1];
    char* str = argv[2];
    int depth = atoi(argv[3]);
    search_directories(directory_name, str, depth);
    return 0;
}