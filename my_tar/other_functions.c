#include "functions.h"

int check(char *a, char *b){
    int len = 0;
    while(*a != '\0' && *b != '\0'){
        if(*a > *b){
            return 0;
        }
        else if(*a < *b){
            return 1;
        }
        a++;
        b++;
    }
    a -= len;
    b -= len;
    int len3 = strlen(a), len2 = strlen(b); 
    if(len3 > len2){
        return 0;
    }
    else if(len2 > len3){
        return 1;
    }
    return 2;
}
// fd for archive
bool is_zero(char buf[512]){
    for(int i = 0; i < BLOCKSIZE; i++){
        if(buf[i]){
            return 0;
        }
    }
    return 1;
}
void make_good(int fd, file_data** archive, int p){
    int can = 1;
    file_data** cop = archive;
    while(1){
        *cop = malloc(sizeof(file_data));
        if(can){
            read(fd, (*cop) -> block, BLOCKSIZE);
        }
        can = 1;
        if(is_zero((*cop) -> block)){
            read(fd, (*cop) -> block, BLOCKSIZE);
            if(is_zero((*cop) -> block)){
                *cop = NULL;
                break;
            }
            can = 0;
        }
        (*cop) -> st = p;
        unsigned int skip = from_oct_to_dec((*cop) -> size);
        if(skip % BLOCKSIZE != 0){
            skip += 512 - (skip % BLOCKSIZE);
        }
        p += skip + 512;
        lseek(fd, skip, SEEK_CUR);
        cop = &((*cop) -> next); 
    }
}
int get_fd(my_options* opt)
{
    int fd_archive;
    int flags;
    const mode_t perm = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    if (is_create(opt))
    {
        if (opt -> is_c)
           flags = O_CREAT | O_WRONLY | O_TRUNC;
        else if (opt -> is_r)
            flags = O_CREAT | O_WRONLY | O_APPEND;
        else 
            flags = O_CREAT | O_APPEND | O_RDWR;
    }
    else
        flags = O_RDONLY;
    fd_archive = open(opt -> archive_name, flags, perm);
    return fd_archive;
}

// converter
void convert(char* res, long int num, int sz, int BASE)
{
    memset(res, '0', sz);
    char* alpha = "0123456789";
    res[sz - 1] = '\0';
    sz--;
    while (num > 0)
    {
        int mod = num % BASE;
        res[--sz] = alpha[mod];
        num /= BASE;
    }
}
int mn(int a, int b){
    if(a > b){
        return b;
    }
    return a;
}

// from oct to dec
long int from_oct_to_dec(char* num)
{
    long int res = 0, lg = 1;
    for (int i = strlen(num) - 1; i >= 0; --i)
    {
        int x = (int)(num[i] - '0');
        res = res + lg * x;
        lg = lg * 8;
    }
    return res;
}
void my_copy(char *a, char *b){
    int len = 0;
    while(*b != '\0'){
        *a = *b;
        a++;
        b++;
    }
    b -= len;
    a -= len;
    return;
}
void make_ready(char *rec, unsigned int mode){
    int len = strlen(rec);
    if(!len){
        return;
    }
    char *cop = malloc(len*sizeof(char));
    if(!mode){
        mode = (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    }
    for(int i = 0;  i < len; i++){
        cop[i] = rec[i];
        if(cop[i] == '/'){
            cop[i] = '\0';
            mkdir(cop, mode);
            cop[i] = '/';
        }
    }
    if(rec[len - 1] != '/'){
        cop[len] = '\0';
        mkdir(cop, mode);
    }
}
/*char* get_archive_content(int fd_archive)
{

}*/