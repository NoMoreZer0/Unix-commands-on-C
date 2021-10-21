#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define READLINE_READ_SIZE 7

char* concate (char* a, char* b) { 
    char* c;
    c = (char*)(malloc(strlen(a) + strlen(b) + 1));

    int p = 0;
    for (int i = 0; i < strlen(a); ++i) { 
        c[p] = a[i], ++p;
    }
    for (int i = 0; i < strlen(b); ++i) { 
        c[p] = b[i], ++p;
    }

    return c;
}

char* my_readline (int fd) { 
    static int end = 0, spos = 0, cnt = 0;
    static char* buff = "";

    if (cnt == 0) { 
        int ret;
        char* cur;
        cur = (char*)(malloc(READLINE_READ_SIZE));

        int flag = 0;
        if (fd != 0) { 
            while ((ret = read(fd, cur, READLINE_READ_SIZE)) > 0) { 
                buff = concate (buff, cur);
                cur = (char*)(malloc(READLINE_READ_SIZE));
                flag = 1;
            }
        } else {
            ret = read(fd, cur, READLINE_READ_SIZE);
            buff = concate (buff, cur);
            flag = 1;
        }

        if (ret == -1) return NULL;
        if (!flag) end = 1;
        cnt = 1;
    }

    char* res;
    res = (char*)(malloc(strlen(buff)));

    if (!end) {     
        for (int i = spos, p = 0; i < strlen(buff); ++i, ++p) { 
            if (buff[i] == '\n') { 
                if (i == strlen(buff) - 1) { 
                    end = 1;
                }
                spos = i + 1;
                break;
            }
            res[p] = buff[i];
            if (i == strlen(buff) - 1) { 
                end = 1;
            }
        }
        return res; 
    }
    else { 
        return NULL;
    }
}