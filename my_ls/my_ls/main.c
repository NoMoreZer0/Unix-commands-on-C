#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

int main(int argc, char** argv){
    int a_flag = 0, R_flag = 0, t_flag = 0;

    for (int i = 1; i < argc; ++i) { 
        if (argv[i][0] == '-') { 
            for (int j = 1; j < strlen(argv[i]); ++j) { 
                if (argv[i][j] == 'a' || argv[i][j] == 'R' || argv[i][j] == 't') { 
                    if (argv[i][j] == 'a') a_flag = 1;
                    if (argv[i][j] == 'R') R_flag = 1;
                    if (argv[i][j] == 't') t_flag = 1;
                }
                else { 
                    printf ("my_ls: invalid option -- '%c'\n", argv[i][j]);
                    return 0;
                }
            }
        }
    }

    int cnt = 0;
    char* f;
    for (int i = 1; i < argc; ++i) { 
        if (argv[i][0] != '-') { 
            ++cnt;
            f = argv[i];
        }
    }

    if (!cnt) { 
        print_directories (NULL, ".", ".", a_flag, R_flag, t_flag, 0);
        return 0;
    }

    if (cnt == 1) { 
        print_directories (NULL, get_path(".", f), f, a_flag, R_flag, t_flag, 0);
    } else { 
        for (int i = 1; i < argc; ++i) { 
            if (argv[i][0] != '-') { 
                print_directories (NULL, get_path(".", argv[i]), argv[i], a_flag, R_flag, t_flag, 1);
                cnt--;
                if (cnt != 0) printf ("\n");
            }
        }
    }

    return 0;
}