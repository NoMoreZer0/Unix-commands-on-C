#include "functions.h"

int main (int argc, char** argv) 
{
    my_options* opt = get_options(argc, argv);   
    if (opt == NULL)
        return 1;
    if (opt -> args == NULL && opt -> is_c)
    {
        printf ("ERROR: cannot create an empty archive\n");
        return 2;
    }
    int fd_archive = get_fd(opt);
    if (fd_archive < 0)
    {
        printf ("ERROR: cannot open or create archive\n");
        return 2;
    }
    int ret = 0;
    if (is_create(opt))
        ret += options_create(opt, fd_archive);
    else
        ret += options_execute(opt, fd_archive);
    close(fd_archive);
    return ret;
}
