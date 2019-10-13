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
    char **argv;
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

struct cmd *parsecmd(char *s);


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
        parsecmd(buf);
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


struct cmd *parsecmd(char *buf) {
    char *pipe_char = "|";
    char *redir_char = "<>";
    int index;
    char *s = buf;

    if ((index = token_index(s, pipe_char)) >= 0) {
        printf("Pipe present at %d\n", index);
        struct cmd *lcmd, *rcmd;
        char *left =  buf;
        char *right = buf + index + 1;
        *(left + index) = 0;
        lcmd = parsecmd(left);
        rcmd = parsecmd(right);
        printf("%d %d\n", lcmd->type, rcmd->type);
        return 0;
    }
    s = buf;
    if ((index = token_index(s, redir_char)) >= 0) {
        printf("Redir present at %d\n", index);
        return 0;
    }

    printf("Exec command\n");
    char *start = buf;
    int argc = 0;
    char *argv[MAXARGS];
    char whitespace[] = " \t\r\n\v";
    char *es = start + strlen(start);
    char arg;
    int i = 0;
    //printf("%d\n", strlen(start));
    while (start < es) {
        if (strchr(whitespace, *start)) {
            start++;
            continue;
        } else {
            int j = i;
            argc++;
            printf("%d", argc);
            while (strchr(whitespace, *start) == 0 && start < es) {
                *(&arg + i) = *start;
                i++;
                start++;
            }
            *(&arg + i) = 0;
            argv[argc-1] = &arg+j;
            i++;
            printf("%s\n", &arg);
        }
    }

    for (int i = 0; i < argc; i++) {
        printf("%s\n", *(argv+i));
    }
    struct execcmd *cmd;
    cmd->type = EXEC;
    cmd->argc = argc;
    cmd->argv = argv;
    return (struct cmd *)cmd;
}
