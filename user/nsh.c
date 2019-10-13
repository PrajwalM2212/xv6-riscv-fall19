//
// Created by PRAJWAL M on 2019-10-13.
//
#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int getcmd(char *buf, int nbuf);
char* strip(char *buf);


int main(void){

    static char buf[100];
    int fd;

    while((fd = open("console", O_RDWR)) >= 0){
        if(fd >= 3){
            close(fd);
            break;
        }
    }

    while(getcmd(buf, sizeof(buf)) >= 0){
        char *i = strip(buf);
        if(*i == 'c' && *(i+1) == 'd' && *(i+2) == ' '){
            char *p = strip(i+3);
            *(p + (strlen(p) - 1)) = 0;
            if(chdir(p) < 0){
                printf("cannot cd %s\n", p);
            }else{
                printf("In %s\n", p);
            }
            continue;
        }
    }
    exit(0);
}


int getcmd(char *buf, int buf_size){
    fprintf(2, "@ ");
    memset(buf, 0, buf_size);
    gets(buf, buf_size);
    if(buf[0] == 0){
        return -1;
    }
    return 0;
}

char* strip(char *s){
    char *ps = s;
    while(strchr(" \n\t\r\v", *ps)){
        ps++;
    }
    s = ps;
    return s;
}