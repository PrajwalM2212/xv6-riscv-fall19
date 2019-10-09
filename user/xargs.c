//
// Created by PRAJWAL M on 2019-10-08.
//

#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char **argv) {
    char line;
    while(read(0,&line,1)) {
        int pid = fork();
        if (pid == 0) {
            int i = 1;
            int n;
            printf("Child\n");

            while ((n = read(0, &line + i, 1)) == 1 && *(&line + i) != '\n') {
                i += 1;
            }
            *(&line + i) = 0;
            *(argv + argc) = &line;
            exec(argv[1], argv + 1);
            exit(0);
        } else {
            int estatus;
            wait(&estatus);
            *(argv + argc) = 0;
            printf("%d \n", estatus);
            printf("Completed %d\n");
        }
    }
    exit(0);
}