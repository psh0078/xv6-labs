#include "kernel/types.h"
#include "user/user.h"

#define HZ 100

int
main(int argc, char *argv[])
{
  int p1[2];
  int p2[2];
  pipe(p1);
  pipe(p2);
  int pid = fork();
  int N = 1000;

  if (pid == 0) { // child
    close(p1[1]);
    close(p2[0]);
    char ball[1];
    for (int i=0; i<N; i++) {
        read(p1[0], ball, 1);
        write(p2[1], ball, 1);
    }
    close(p1[0]);
    close(p2[1]);
    exit(0);

  } else { // parent
    close(p1[0]);
    close(p2[1]);
    char ball[1] = "Z";

    int start = uptime();
    for (int j=0; j<N; j++) {
        write(p1[1], ball, 1);
        read(p2[0], ball, 1);
    }
    int end = uptime();

    wait(0);
    close(p1[1]);
    close(p2[0]);

    int dt = end - start;   // ticks
    if (dt <= 0) dt = 1;

    long long total_us = (long long)dt * 1000000LL / HZ;
    long long avg_us = total_us / (long long)N;
    long long ex_per_sec = (long long)N * HZ / (long long)dt;

    printf("ticks=%d (HZ=%d), N=%d\n", dt, HZ, N);
    printf("average RTT: %lld.%03lld ms\n", avg_us / 1000LL, avg_us % 1000LL);
    printf("exchanges per second: %lld\n", ex_per_sec);
  }
  return 0;
}

