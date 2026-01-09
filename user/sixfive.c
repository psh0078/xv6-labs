#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[512];

void
sixfive(int fd)
{
  int n;
  int i;
  int value = 0;
  int in_number = 0;
  int prev_is_sep = 1;

  while((n = read(fd, buf, sizeof(buf))) > 0) {
    i = 0;
    while(i < n) {
      if(strchr(" -\r\t\n./,", buf[i]) != 0) { // if is a separator
        if (in_number) {
          if (value % 5 == 0 || value % 6 == 0){
            printf("%d\n", value);
          }
          value = 0;
          in_number = 0;
        }
        prev_is_sep = 1;
      } else if ('0' <= buf[i] && '9' >= buf[i]) {
        if (prev_is_sep) {
          in_number = 1;
          value = buf[i] - '0';
          prev_is_sep = 0;
        } else if (in_number) {
          value = value * 10 + ( buf[i] - '0' );
        } else {

        }
      } else {
        value = 0;
        in_number = 0;
        prev_is_sep = 0;
      }
      i++;
    }
  }
  if(n < 0){
    fprintf(2, "sixfive: read error\n");
    exit(1);
  }
  if (in_number) {
    if (value % 5 == 0 || value % 6 == 0)
      printf("%d\n", value);
  }

}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    sixfive(0);
    exit(0);
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], O_RDONLY)) < 0){
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      exit(1);
    }
    sixfive(fd);
    close(fd);
  }
  exit(0);
}

