#include <fcntl.h>
#include <sys/sysmacros.h>
#include "functions.h"

file_data* init_file_data()
{
    file_data* res = (file_data*)malloc(BLOCKSIZE);
    if (res != NULL)
        bzero(res, BLOCKSIZE);
    return res;
}

char get_typeflag(mode_t st_mode)
{
    if (S_ISREG(st_mode))
        return REGTYPE;
    if (S_ISLNK(st_mode))
        return LNKTYPE;
    if (S_ISCHR(st_mode))
        return CHRTYPE;
    if (S_ISBLK(st_mode))
        return BLKTYPE;
    if (S_ISDIR(st_mode))
        return DIRTYPE;
    if (S_ISFIFO(st_mode))
        return FIFOTYPE;
    return '\0';
}

file_data* get_file_data(char* file_name)
{
    struct stat buff;
    if (stat(file_name, &buff) < 0)
    {
        printf ("EROOR: cannot open file %s\n", file_name);
        return NULL;
    }
    file_data* res = init_file_data();
    memset(res -> name, 0, 100);
    strncpy(res -> name, file_name, 100);
    convert(res -> mode, buff.st_mode, 8, OCTAL);
    convert(res -> uid, 125, 8, OCTAL);
    convert(res -> gid, 125, 8, OCTAL);
    convert(res -> size, buff.st_size, 12, OCTAL);
    convert(res -> mtime, buff.st_mtime, 12, OCTAL);
    res -> typeflag = get_typeflag(buff.st_mode);
    convert(res -> devmajor, major(buff.st_rdev), 8, OCTAL);
    convert(res -> devminor, minor(buff.st_rdev), 8, OCTAL);
    if (res -> typeflag == LNKTYPE) { 
        if (readlink(file_name, res -> name, 100) < 0) { 
            return NULL;
        }
    }
    memset(res -> chksum, ' ', 8);
    unsigned int sum = 0;
    for (int i = 0; i < 512; ++i) {
        sum += (unsigned char)res -> block[i];
    }
    snprintf(res -> chksum, sizeof(res -> chksum), "%06o0", sum);
    res -> chksum[6] = '\0';
    res -> chksum[7] = ' ';
    return res;
}

int get_file_sz(int fd_file)
{
    int sz = 0, p = 0;
    char buff[BLOCKSIZE];
    while ((p = read(fd_file, buff, BLOCKSIZE)) > 0)
        sz += p;
    if (sz % BLOCKSIZE != 0)
        sz += (BLOCKSIZE - (sz % BLOCKSIZE));
    lseek(fd_file, 0, SEEK_SET);
    return sz;
}

int write_file_content(int fd_archive, char* file_name, int type)
{
    int fd_file = open(file_name, O_RDONLY);
    if (fd_file == -1)
    {
        printf ("ERROR: cannot open file");
        return 1;
    }
    int sz = get_file_sz(fd_file);
    if (sz == -1)
        return 1;
    if (type == 2)
        lseek(fd_archive, 0, SEEK_END - (BLOCKSIZE * 2));
    char buff[sz];
    memset(buff, '\0', sz);
    int ret = 0;
    ret += read(fd_file, buff, sz);
    ret += write(fd_archive, buff, sz);
    close(fd_file);
    if (ret < 0)
        ret = 1;
    return ret;
}

int get_archive_size(int fd_archive)
{
    struct stat buff;
    fstat(fd_archive, &buff);
    return (int)buff.st_size;
}

time_t get_my_time(char* my_name)
{
    struct stat buff;
    stat(my_name, &buff);
    return buff.st_mtime;
}

int get_my_size(char* my_name)
{
    struct stat buff;
    int sz = 0;
    if (stat(my_name, &buff) < 0)
        sz = -1;
    else 
        sz = (int)buff.st_size;
    return sz;
}

// 2 empty blocks 
void archive_end(int fd_archive)
{
    char buff[BLOCKSIZE * 2];
    memset(buff, '\0', BLOCKSIZE * 2);
    write(fd_archive, buff, BLOCKSIZE * 2);
}

int options_create(my_options* opt, int fd_archive)
{
    int ret = 0;
    my_args* p = opt -> args;
    while (p != NULL)
    {
        file_data* cur = get_file_data(p -> name);
        if (cur != NULL)
        {
            if (opt -> is_c || opt -> is_r)
            {
                write(fd_archive, cur, BLOCKSIZE);
                int type = 0;
                if (opt -> is_c)
                    type = 1;
                if (opt -> is_r)
                    type = 2;
                ret += write_file_content(fd_archive, p -> name, type);
            }
            else if (opt -> is_u)
            {
                int sz = get_archive_size(fd_archive);
                if (sz == 0) 
                {
                    write(fd_archive, cur, BLOCKSIZE);
                    ret += write_file_content(fd_archive, cur -> name, 2);
                }
                else
                {
                    bool bad = 0;
                    int indx = 0, isfile = 1;
                    time_t my_time = get_my_time(p -> name);
                    char res[BLOCKSIZE];
                    lseek(fd_archive, 0, SEEK_SET);
                    while ((indx = read(fd_archive, res, BLOCKSIZE)) > 0)
                    {
                        if (isfile)
                        {
                            time_t indx_time = get_my_time(res);
                            if (my_time <= indx_time)
                                bad = 1;
                        }
                        if (isfile)
                        {
                            int my_sz = get_my_size(res);
                            if (my_sz != 0)
                                isfile = 0;
                        }
                        else 
                            isfile = 1;
                    }
                    if (!bad)
                    {
                        write(fd_archive, cur, BLOCKSIZE);
                        ret += write_file_content(fd_archive, cur -> name, 2);
                    }
                }
            }
        }
        else 
            ret++;
        p = p -> next;
    }
    archive_end(fd_archive);
    return ret;
}
