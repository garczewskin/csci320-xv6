#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/memlayout.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  volatile struct usyscall *u = (struct usyscall *)USYSCALL;

  // Test 1: the page holds this process's pid
  printf("usyscall pid %d, getpid() %d (expect equal)\n", u->pid, getpid());

  // Test 2: a child gets its own page with its own pid
  int child = fork();
  if(child == 0){
    printf("child: usyscall pid %d, getpid() %d (expect equal)\n",
           u->pid, getpid());

    // Test 3: the page is read-only, so this write should fault
    printf("child: writing to the read-only page...\n");
    u->pid = 999;
    printf("child: write succeeded (BUG, page is writable)\n");
    exit(0);
  }
  wait(0);
  printf("parent: pid still %d after child ran\n", u->pid);
  exit(0);
}