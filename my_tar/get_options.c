#include "functions.h"

my_options* init_my_opt()
{
    my_options* opt = malloc(sizeof(my_options));
    opt -> is_c = opt -> is_r = opt -> is_u = opt -> is_x = opt -> is_t = 0;
    opt -> archive_name = NULL;
    opt -> args = NULL;
    return opt;
}

my_options* get_cur_opt(my_options* opt, char* argv, bool* name_flag)
{
    for (int i = 1; i < strlen(argv); ++i)
    {
        if (argv[i] == 'c')
            opt -> is_c = 1;
        else if (argv[i] == 'r')
            opt -> is_r = 1;
        else if (argv[i] == 'u')
            opt -> is_u = 1;
        else if (argv[i] == 'x')
            opt -> is_x = 1;
        else if (argv[i] == 't')
            opt -> is_t = 1;
        else if (argv[i] == 'f')
            *name_flag = true;
    }
    return opt;
}

my_args* add_args(my_args* head, char* name)
{
    my_args* tmp;
    tmp = (my_args*)malloc(sizeof(my_args));
    tmp -> name = name;
    tmp -> next = NULL;
    if (head == NULL)
        head = tmp;
    else
    {
        my_args* p = head;
        while (p -> next != NULL)
            p = p -> next;
        p -> next = tmp;
        head = p;
    }
    return head;
}

bool is_create(my_options* opt)
{
    if (opt -> is_c || opt -> is_r || opt -> is_u)
        return 1;
    return 0;
}

bool is_extract(my_options* opt)
{
    if (opt -> is_x || opt -> is_t)
        return 1;
    return 0;
}

bool is_error(my_options* opt)
{
    if (!is_create(opt) && !is_extract(opt)){
        printf ("ERROR: you must handle at least one option\n");
        return 1;
    }
    if (opt -> archive_name == NULL){
        printf ("ERROR: -f option required\n");
        return 1;
    }
    if (is_create(opt) && is_extract(opt)){
        printf ("ERROR: you cannot handle both creation and extraction\n");
        return 1;
    }
    int create_sum = opt -> is_c + opt -> is_r + opt -> is_u;
    int ex_sum = opt -> is_x + opt -> is_t;
    if (create_sum > 1 || ex_sum > 1)
    {
        printf ("ERROR: you cannot handle more than one option of creation or extraction\n");
        return 0;
    } 
    return 0;
}

my_options* get_options(int argc, char** argv)
{
    my_options* opt = init_my_opt();
    bool name_flag = false;
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-'){
            opt = get_cur_opt(opt, argv[i], &name_flag);
        }
        else
        {
            if (name_flag)
            {
                opt -> archive_name = argv[i];
                name_flag = 0;
            }
            else
                opt -> args = add_args(opt -> args, argv[i]);
        }
    }
    if (is_error(opt))
        return NULL;
    return opt;
}