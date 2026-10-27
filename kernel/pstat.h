#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"

struct pstat {
  int inuse[NPROC];    // whether this slot of the process table is in use (1 or 0)
  int pid[NPROC];      // the PID of each process
  int runtime[NPROC];  // the number of times each process has been chosen to run
  int pass[NPROC];
  int stride[NPROC];
  int passTotal;
};

#endif // _PSTAT_H_