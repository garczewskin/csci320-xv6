#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  // Test 1: provided tester, every page should be accessed
  char *pageTester;
  int accessBitmap = 0;
  pageTester = malloc(sizeof(int)*8*PGSIZE);
  for (int i = 0; i < sizeof(int)*8; ++i)
    pageTester[i*PGSIZE] = 1;
  if (pgaccess(pageTester, sizeof(int)*8, &accessBitmap) < 0)
    printf("error in pgaccess\n");
  else
    for (int i = 0; i < sizeof(int)*8; ++i)
      printf("Page %d, accessed: %d\n", i, (accessBitmap & (1<<i)) != 0);

  // Test 2: clearing works, and only touched pages are reported
  char *buf = malloc(8*PGSIZE);
  int mask = 0;
  for (int i = 0; i < 8; i++) buf[i*PGSIZE] = 1;
  pgaccess(buf, 8, &mask);
  printf("after touching all 8: 0x%x (expect 0xff)\n", mask);

  pgaccess(buf, 8, &mask);
  printf("immediately again:    0x%x (expect 0x0)\n", mask);

  buf[2*PGSIZE] = 1;
  buf[5*PGSIZE] = 1;
  pgaccess(buf, 8, &mask);
  printf("touched pages 2, 5:   0x%x (expect 0x24)\n", mask);

  // Test 3: bad arguments
  printf("33 pages: %d (expect -1)\n", pgaccess(buf, 33, &mask));
  printf("0 pages:  %d (expect 0)\n", pgaccess(buf, 0, &mask));
  exit(0);
}