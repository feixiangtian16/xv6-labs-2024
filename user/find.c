#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"

void find(char *path, char *filename) {
    int fd;
    struct stat st;
    struct dirent de;
    char buf[512];
    char *p;
    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type) {
        case T_DIR:
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) {
                    continue;
                }
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                    continue;
                }

                int i = 0;
                while (path[i] != '\0') {
                    buf[i] = path[i];
                    i++;
                }
                buf[i++] = '/';
                int j = 0;
                while (de.name[j] != '\0') {
                    buf[i++] = de.name[j];
                    j++;
                }
                buf[i] = '\0';
                
                find(buf, filename);
            }
            break;
        case T_FILE:
            p = path;
            for(char *s = path; *s; s++) {
                if (*s == '/') {
                    p = s + 1;
                }
            }
            if (strcmp(p, filename) == 0) {
                printf("%s\n", path);
            }
            break;
    }
    close(fd);
}   

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: find <directory> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}