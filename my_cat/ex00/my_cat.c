#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char** argv){
    int ch;
    for(int i = 1; i < argc; ++i){
        int fd = open(argv[i], O_RDONLY);
        while(read(fd, &ch, 1)) write(STDOUT_FILENO, &ch, 1);
        close(fd);
    }
    return 0;
}