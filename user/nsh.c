//
// Created by PRAJWAL M on 2019-10-13.
//
#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define EXEC 0
#define REDIR 1
#define PIPE 2
#define MAXARGS 10

struct cmd {
    int type;
};

struct execcmd {
    int type;
    int argc;
    char *argv[MAXARGS];
};

struct pipecmd {
    int type;
    struct cmd *left;
    struct cmd *right;
};

struct redircmd {
    int type;
    char *file;
    int fd;
    struct cmd *cmd;
};

int getcmd(char *buf, int nbuf);

char *strip(char *buf);

void parsecmd(char *s);
void runexeccmd(char *s);


int main(void) {

    static char buf[100];
    int fd;

    while ((fd = open("console", O_RDWR)) >= 0) {
        if (fd >= 3) {
            close(fd);
            break;
        }
    }

    while (getcmd(buf, sizeof(buf)) >= 0) {
        char *i = strip(buf);
        if (*i == 'c' && *(i + 1) == 'd' && *(i + 2) == ' ') {
            char *p = strip(i + 3);
            *(p + (strlen(p) - 1)) = 0;
            if (chdir(p) < 0) {
                printf("cannot cd %s\n", p);
            } else {
                printf("In %s\n", p);
            }
            continue;
        }
        buf[strlen(buf) - 1] = 0;
        //if(fork() == 0) {
            parsecmd(buf);
        //}
        wait(0);
    }
    exit(0);
}


int getcmd(char *buf, int buf_size) {
    fprintf(2, "@ ");
    memset(buf, 0, buf_size);
    gets(buf, buf_size);
    if (buf[0] == 0) {
        return -1;
    }
    return 0;
}

char *strip(char *s) {
    char *ps = s;
    while (strchr(" \n\t\r\v", *ps)) {
        ps++;
    }
    s = ps;
    return s;
}

int token_index(char *s, char *pattern) {
    char *es = s + strlen(s);
    int count = 0;
    while (!strchr(pattern, *s) && s < es) {
        s++;
        count++;
    }

    if (s < es) {
        return count;
    }
    return -1;
}


void parsecmd(char *buf) {
    char *pipe_char = "|";
    //char *redir_char = "<>";
    int index;
    char *s = buf;

    if ((index = token_index(s, pipe_char)) >= 0) {
        //printf("Pipe present at %d\n", index);
        char *left = buf;
        char *right = buf + index + 1;
        *(left + index) = 0;
        int p[2];
        if(pipe(p) < 0){
            printf("Error while pipe\n");
            return;
        }
        if(fork() == 0){
            close(1);
            dup(p[1]);
            close(p[0]);
            close(p[1]);
            runexeccmd(left);
        }

        if(fork() == 0){
            close(0);
            dup(p[0]);
            close(p[0]);
            close(p[1]);
            runexeccmd(right);
        }
        close(p[0]);
        close(p[1]);
        wait(0);
        wait(0);
        return;
    }
    /*
    s = buf;
    if ((index = token_index(s, redir_char)) >= 0) {
        printf("Redir present at %d\n", index);
        return 0;
    }
    */
    if(fork() == 0) {
        runexeccmd(buf);
    }
    wait(0);
}

void runexeccmd(char *buf) {
    //printf("Exec command\n");
    char *start = buf;
    int argc = 0;
    char *es = start + strlen(start);
    char arg;
    int i = 0;
    struct execcmd cmd;
    cmd.type = EXEC;
    while (start < es) {
        if (*start == ' ') {
            start++;
        } else {
            int j = i;
            while ((*start != ' ') && start < es) {
                *(&arg + i) = *start;
                i++;
                start++;
            }
            *(&arg + i) = '\0';
            argc++;
            cmd.argv[argc - 1] = &arg + j;
            i++;
        }
    }
    memset(cmd.argv+argc, 0, MAXARGS-argc);
    cmd.argc = argc;
    //printf("%d\n", argc);
    int k = 0;
    while(cmd.argv[k] && k < 10){
        //printf("%s\n", cmd.argv[k]);
        k++;
    }
    exec(cmd.argv[0], cmd.argv);
};
