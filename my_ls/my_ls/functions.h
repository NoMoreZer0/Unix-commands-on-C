#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <string.h>
#include <dirent.h>

void print_directories(DIR* dir, char* path, char* outputPath, int a_flag, int R_flag, int t_flag, int multi_flag);
void print_directory(DIR* dir, char* cur, int a_flag, int t_flag);
char* get_path(char* curpath, char* need);