//
// Created by PRAJWAL M on 2019-10-08.
//

#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char **argv) {
    //printf("%s %s\n", *(argv+2), *(argv+3));
    //exec(argv[1], argv+1);
    int count = 0;
    while(count < 10) {
        count += 1;
        int pid = fork();
        if (pid == 0) {
            char line;
            int i = 0;
            int n;
            //printf("Child\n");

            while ((n = read(0, &line + i, 1)) == 1 && *(&line + i) != '\n') {
                i += 1;
            }
            *(&line + i) = 0;
            //i = 0;
            //while (*(argv + i) != 0) {
            //printf("%s\n", *(argv+i));
            //    i += 1;
            //}
            //argv[i] = &line;
            //printf("%s\n", &line);
            *(argv + argc) = &line;
            exec(argv[1], argv + 1);
            exit(0);
        } else {
            int estatus;
            //while(1) {
            wait(&estatus);
            //}
            *(argv + argc) = 0;
            printf("%d \n", estatus);
            printf("Completed %d\n");
        }
    }
    exit(0);
}