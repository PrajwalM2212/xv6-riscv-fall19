//
// Created by PRAJWAL M on 2019-10-06.
//

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

void find(char *path, char *text) {

    int fd;
    char buf[512], *p;
    struct stat st;
    struct dirent de;

    if ((fd = open(path, 0)) < 0) {
        printf("%s cannot be opened\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        printf("Cannot stat %s \n", path);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_FILE:
            if(strcmp(path, text) == 0){
                printf("%s \n", path);
                return;
            }
            break;

        case T_DIR:
            if (strlen(path) + 2 + DIRSIZ > sizeof(buf)) {
                printf("ls: path too long\n");
                break;
            }

            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';

            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;

                if (stat(buf, &st) < 0) {
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }

                if(strcmp(p, text) == 0){
                    printf("%s \n", buf);
                    return;
                }

                if(st.type == T_DIR) {
                    if(strcmp(p, ".") != 0 && strcmp(p, "..") != 0) {
                        find(buf, text);
                    }
                }
            }
            break;
    }

    close(fd);
}


int main(int argc, char *argv[]) {

    if (argc != 3) {
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);

}

