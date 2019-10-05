//
// Created by PRAJWAL M on 2019-10-06.
//
#include "kernel/types.h"
#include "user/user.h"


void callSleep(int n) {

    int output;
    if ((output = sleep(n)) < 0) {
        printf("Error sleeping");
        exit(1);
    }

}

int main(int argc, char **argv) {

    if (argc <= 1 || argc > 2) {
        printf("Enter the amount of time to sleep \n");
        callSleep(0);
    }
    callSleep(atoi(argv[1]));
    exit(0);

}

