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
        if(lcmd->type == EXEC){
            struct execcmd *ecd =  (struct execcmd*)lcmd;
            printf("%d \n", ecd->argc);
            printf("%s\n", ecd->argv[(ecd->argc)-1]);
        }
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
    char *es = start + strlen(start);
    char arg;
    int i = 0;
    struct execcmd *cmd;
    cmd = malloc(sizeof(*cmd));
    cmd -> type = EXEC;
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
            *(&arg + i) = 0;
            argc++;
            cmd->argv[argc-1] = &arg+j;
            i++;
        }

    }

    cmd->argc = argc;
    for(int k =0; k<argc; k++){
        printf("%s\n", cmd->argv[k]);
    }
    return (struct cmd*)cmd;
}
