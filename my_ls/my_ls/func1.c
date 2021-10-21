#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <string.h>
#include "functions.h"


void print_directories(DIR* dir, char* path, char* outputPath, int a_flag, int R_flag, int t_flag, int multi_flag){
    // printing current directory
    void* ret = dir = opendir(path);    
    
    if (ret == NULL){
        printf("my_ls: cannot access '%s': No such file or directory\n", path);
        return;
    }

    if (R_flag || multi_flag){
        printf("%s:\n", outputPath);
    }

    print_directory(dir, path, a_flag, t_flag);
    closedir(dir);

    if (R_flag){
        // making recursion
        dir = opendir(path);
        for(struct dirent* d = readdir(dir); d != NULL; d = readdir(dir)){
            if (d -> d_name[0] == '.') continue;
            struct stat buf;
            stat(get_path(path, d -> d_name), &buf);
            if (!S_ISREG(buf.st_mode)){
                DIR* dir2 = dir;
                printf("\n");
                print_directories(dir, get_path(path, d->d_name), get_path(outputPath, d->d_name), a_flag, R_flag, t_flag, multi_flag);
                dir = dir2;
            }
        }
        closedir(dir);
    }
}