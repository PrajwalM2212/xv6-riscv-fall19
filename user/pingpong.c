//
// Created by PRAJWAL M on 2019-10-06.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void pingpong(){
    int p_fd[2];
    int c_fd[2];

    pipe(p_fd);
    pipe(c_fd);

    int pid =  fork();
    char buf[1] = {'b'};
    char bufc[1];
    char bufd[1];

    if (pid == 0){
        int n = read(p_fd[0], bufc, 1 );

        if(n == 1){
            printf("%d : received ping\n", getpid());
            write(c_fd[1], bufc, 1);
        }

    }else{
        write(p_fd[1], buf, 1);
        int n = read(c_fd[0], bufd, 1);
        if(n == 1){
            printf("%d : received pong\n", getpid());
        }
    }


}


int main(int argc, char *argv[]){
    pingpong();
    exit(0);
}

