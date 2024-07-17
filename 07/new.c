#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


int main(){
    if(argc!=2){
        printf("usage: %s <file> \n",argv[0])
        return -1;
    }
    char buffer[10];
    char buffer2[10];
    int fd=open(argv[1],O_RDONLY);
    int fd2 = open(argv[1], O_RDONLY);
    printf("fd = %d\n", fd);
    printf("fd2 = %d\n", fd2);
    while(1){
        sleep(100);
    }
    return 0;
}