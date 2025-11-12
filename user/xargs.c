#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void run(char *argv[], int argc, char *line) {
  char *args[MAXARG];
  int i;
  for (i = 1; i < argc; i++) {
    args[i - 1] = argv[i];
  }
  int n = argc - 1;

  char *p = line;
  while (*p) {
    while (*p == ' ') p++;
    if (*p == 0) break;
    args[n++] = p;
    if (n >= MAXARG) break;
    while (*p && *p != ' ') p++;
    if (*p == ' ') *p++ = 0;
  }
  args[n] = 0;

  if (fork() == 0) {
    exec(args[0], args);
    fprintf(2, "xargs: exec %s failed\n", args[0]);
    exit(1);
  } else {
    wait(0);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "Usage: xargs command [args...]\n");
    exit(1);
  }

  char buf[512];
  int i = 0;
  char c;

  while (read(0, &c, 1) == 1) {
    if (c == '\n') {
      buf[i] = 0;      
      run(argv, argc, buf);
      i = 0;
    } else {
      buf[i++] = c;
      if (i >= sizeof(buf) - 1)
        i = sizeof(buf) - 2;  
    }
  }

  if (i > 0) {
    buf[i] = 0;
    run(argv, argc, buf);
  }

  exit(0);
}
