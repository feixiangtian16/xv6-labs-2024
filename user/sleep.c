#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if (argc < 2){
    fprintf(2,"Usage: sleep time..\n");
    exit(1);
  }

  int i = atoi(argv[1]);
  if(i<0) i=0;
  sleep(i);
  exit(0); 
}
