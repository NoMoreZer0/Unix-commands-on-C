#include "functions.h"

int options_execute(my_options* opt, int fd_archive)
{
    lseek(fd_archive, 0, SEEK_SET);
    file_data* a = NULL;
    make_good(fd_archive, &a, 0);
    int ret = 0;
    if (opt -> is_t)
    {
        while(a){
            printf("%s\n", a -> name);
            a = a -> next;
        }
    }
    else 
    {
        while(a){
            struct s_args* ar = opt -> args;
            while(ar){
                if(check(ar -> name, a -> name) == 2){
                    if(a -> typeflag == DIRTYPE){
                        make_ready(a -> name, from_oct_to_dec(a -> mode));
                    }
                    else{
                        int len = strlen(a -> name);
                        char *rec = malloc(len * sizeof(char));
                        my_copy(rec, a -> name);
                        while(len > 0 && rec[len] != '/'){
                            len--;
                        }
                        rec[len] = '\0';
                        make_ready(rec, 0);
                        int fd = open(a -> name, O_WRONLY | O_CREAT | O_TRUNC, from_oct_to_dec(a -> mode));
                        unsigned int sz = from_oct_to_dec(a -> size);
                        char buf[512];
                        unsigned int hv = 0;
                        lseek(fd_archive, (a -> st + 512), SEEK_SET);
                        while(hv < sz){
                            int op = read(fd_archive, buf, mn(512, (sz - hv)));
                            write(fd, buf, op);
                            hv += op;
                        }
                        close(fd);
                    }
                }
                ar = ar -> next;
            }
            a = a -> next;
        }
    }
    return ret;
}