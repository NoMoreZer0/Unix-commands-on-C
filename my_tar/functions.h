#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
# include <stdarg.h>
# include <fcntl.h>

# define BLOCKSIZE 512
# define OCTAL 8
# define DEC 10
#define RECORDSIZE      10240

# define REGTYPE  '0'            /* regular file */
# define AREGTYPE '\0'           /* regular file */
# define LNKTYPE  '1'            /* link */
# define SYMTYPE  '2'            /* reserved */
# define CHRTYPE  '3'            /* character special */
# define BLKTYPE  '4'            /* block special */
# define DIRTYPE  '5'            /* directory */
# define FIFOTYPE '6'            /* FIFO special */
# define CONTTYPE '7'            /* reserved */

typedef struct s_args
{
    char* name;
    struct s_args* next;
}my_args;

typedef struct s_my_options
{
    int is_c;
    int is_r;
    int is_u;
    int is_x;
    int is_t;
    char* archive_name;
    struct s_args* args;
}my_options;

//get option
my_options* get_options(int argc, char** argv);
my_options* get_cur_opt(my_options* opt, char* argv, bool* name_flag);
my_options* init_my_opt();
my_args* add_args(my_args* head, char* name);
bool is_extract(my_options* opt);
bool is_create(my_options* opt);
bool is_error(my_options* opt);

//other functions
int get_fd(my_options* opt);
void convert(char* res, long int num, int sz, int BASE);
long int from_oct_to_dec(char* num);
int check(char *a, char *b);
typedef struct s_my_file
{   
    unsigned int st;
    union {       
        struct  {
                            /* byte offset */
        char name[100];               /*   0 */
        char mode[8];                 /* 100 */
        char uid[8];                  /* 108 */
        char gid[8];                  /* 116 */
        char size[12];                /* 124 */
        char mtime[12];               /* 136 */
        char chksum[8];               /* 148 */
        char typeflag;                /* 156 */
        char linkname[100];           /* 157 */
        char magic[6];                /* 257 */
        char version[2];              /* 263 */
        char uname[32];               /* 265 */
        char gname[32];               /* 297 */
        char devmajor[8];             /* 329 */
        char devminor[8];             /* 337 */
        char prefix[155];             /* 345 */
                                    /* 500 */
        };
        char block[512];
    };
    struct s_my_file *next;
}file_data;
 
typedef struct s_my_tar
{
    char* archive_name;
    bool creation_flag;
    struct s_my_file* next;
}my_tar;
void make_ready(char *rec, unsigned int mode);
int mn(int a, int b);
void make_good(int fd, file_data** archive, int p);
void my_copy(char *a, char *b);
// create option
int options_create(my_options* opt, int fd_archive);
file_data* get_file_data(char* file_name);
char get_typeflag(mode_t st_mode);
file_data* init_file_data();
int get_my_size(char* my_name);

// execute option
int options_execute(my_options* opt, int fd_archive);