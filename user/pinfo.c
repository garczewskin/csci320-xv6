#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(void)
{
  struct pstat ps;

  if (getpinfo(&ps) < 0) {
    printf("pinfo: getpinfo failed\n");
    exit(1);
  }
  printf("PID\tRUNTIME\n");
  for (int i = 0; i < NPROC; i++) {
    if (ps.inuse[i])
      printf("%d\t%d\n", ps.pid[i], ps.runtime[i]);
  }
  exit(0);
}