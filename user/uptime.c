//
// Created by PRAJWAL M on 2019-10-06.
//

#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv){
    if(argc > 1)
        printf("Unnecessary args passed with uptime\n");

    printf("Uptime: %d\n", uptime());
    exit(0);
}
