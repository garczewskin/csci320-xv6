#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/pstat.h"
#include "user/user.h"

// usage: monitor <interval_ticks> <snapshots>
static struct pstat prev, cur;

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    printf("usage: monitor <interval_ticks> <snapshots>\n");
    exit(1);
  }
  int interval = atoi(argv[1]);
  int snaps = atoi(argv[2]);
  int me = getpid();

  if (getpinfo(&prev) < 0)
    exit(1);
  for (int s = 1; s <= snaps; s++) {
    pause(interval);
    if (getpinfo(&cur) < 0)
      exit(1);

    int total = 0;
    for (int i = 0; i < NPROC; i++) {
      if (!cur.inuse[i] || cur.pid[i] == me)
        continue;
      if (!prev.inuse[i] || prev.pid[i] != cur.pid[i])
        continue;
      total += cur.runtime[i] - prev.runtime[i];
    }

    printf("--- snapshot %d, runs in interval %d, passTotal %d\n",
           s, total, cur.passTotal);
    for (int i = 0; i < NPROC; i++) {
      if (!cur.inuse[i] || cur.pid[i] == me)
        continue;
      if (!prev.inuse[i] || prev.pid[i] != cur.pid[i])
        continue;
      int d = cur.runtime[i] - prev.runtime[i];
      if (d > 0 && total > 0) {
        int sh = d * 1000 / total;
        printf("pid %d stride %d runtime %d delta %d share %d.%d%% pass %d\n",
               cur.pid[i], cur.stride[i], cur.runtime[i], d,
               sh / 10, sh % 10, cur.pass[i]);
      }
    }
    prev = cur;
  }
  exit(0);
}