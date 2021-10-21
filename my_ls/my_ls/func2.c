#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <string.h>
#include <dirent.h>
#include "functions.h"

typedef struct s_files_array { 
    char* name;
    time_t time;
} files_array;

char* get_path(char* curpath, char* need){
    char* res;
    res = (char*)(malloc(55));
    for(int i = 0; i < 50; ++i) res[i] = '\0';
    for(int i = 0; i < strlen(curpath); ++i) res[i] = curpath[i];
    res[strlen(curpath)] = '/';
    int p = strlen(curpath) + 1;
    for(int i = 0; i < strlen(need); ++i) res[p] = need[i], ++p;
    return res;
}

int is_lower (char* a, char* b) { 
    // is a < b (lexi)
    for (int i = 0; i < strlen(a); ++i) { 
        if (a[i] > b[i]) return 1;
        else if (a[i] < b[i]) return 0;
        else if (i == strlen(b)) return 1;
    }
    return 0;
}

void lexSort (files_array* list, int n) { 
    for (int i = 0; i < n; ++i) { 
        for (int j = 1; j < n; ++j) { 
            if (is_lower(list[j - 1].name, list[j].name)) { 
                // swap
                files_array prev = list[j - 1];
                list[j - 1] = list[j];
                list[j] = prev;
            }
        }
    }
}

void timeSort (files_array* list, int n) {
    for (int i = 0; i < n; ++i) { 
        for (int j = 1; j < n; ++j) { 
            if (list[j - 1].time < list[j].time) { 
                // swap
                files_array prev = list[j - 1];
                list[j - 1] = list[j];
                list[j] = prev;
            }
        }
    }

    for (int i = 0; i < n; ++i) { 
        for (int j = 1; j < n; ++j) { 
            if (list[j - 1].time == list[j].time && is_lower (list[j - 1].name, list[j].name)) { 
                // swap
                files_array prev = list[j - 1];
                list[j - 1] = list[j];
                list[j] = prev;
            }
        }
    }
} 

void print_directory(DIR* dir, char* cur, int a_flag, int t_flag){
    files_array list[60];

    int pos = 0;
    for (struct dirent* d = readdir(dir); d != NULL; d = readdir(dir)) { 
        if (!a_flag && d -> d_name[0] == '.') continue;
        list[pos].name = d -> d_name;
        struct stat buff;
        stat (get_path(cur, d -> d_name), &buff);
        list[pos].time = buff.st_mtime;
        ++pos;
    }

    lexSort (list, pos);

    if (t_flag) { 
        timeSort (list, pos);
    }

    for (int i = 0; i < pos; ++i) { 
        printf ("%s  ", list[i].name);
    }
    printf ("\n");
}