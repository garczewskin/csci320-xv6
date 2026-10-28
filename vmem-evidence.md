# Virtual Memory Fun: Evidence

Test programs: `user/pgtest.c` (Part 2) and `user/usystest.c` (Part 3).

## Part 1: vmprint()

Added `vmprint()` to `kernel/vm.c`. It recursively prints every valid page table entry, and `exec.c` calls it for pid 1. The output below matches the sample's indices and tree shape. Entry 509 is absent because Part 3 was not yet done.

```
page table 0x0000000087f52000
 ..0: pte 0x0000000021fd3801 pa 0x0000000087f4e000
 .. ..0: pte 0x0000000021fd3401 pa 0x0000000087f4d000
 .. .. ..0: pte 0x0000000021fd3c1b pa 0x0000000087f4f000
 .. .. ..1: pte 0x0000000021fd3017 pa 0x0000000087f4c000
 .. .. ..2: pte 0x0000000021fd2c07 pa 0x0000000087f4b000
 .. .. ..3: pte 0x0000000021fd2817 pa 0x0000000087f4a000
 ..255: pte 0x0000000021fd4401 pa 0x0000000087f51000
 .. ..511: pte 0x0000000021fd4001 pa 0x0000000087f50000
 .. .. ..510: pte 0x0000000021fd5807 pa 0x0000000087f56000
 .. .. ..511: pte 0x000000002000180b pa 0x0000000080006000
init: starting sh
```

## Part 2: pgaccess()

Added `PTE_A` (bit 6) and a `pgaccess` system call. For each page it checks and clears the PTE's access bit, builds a bitmask, and copies it to the user. `pgtest.c` has the Gary's tester plus tests that call `pgaccess` repeatedly. The second call returning `0x0` shows the bits are cleared, and `0x24` shows only pages 2 and 5 were reported.

```
$ pgtest
Page 0, accessed: 1
Page 1, accessed: 1
Page 2, accessed: 1
Page 3, accessed: 1
Page 4, accessed: 1
Page 5, accessed: 1
Page 6, accessed: 1
Page 7, accessed: 1
Page 8, accessed: 1
Page 9, accessed: 1
Page 10, accessed: 1
Page 11, accessed: 1
Page 12, accessed: 1
Page 13, accessed: 1
Page 14, accessed: 1
Page 15, accessed: 1
Page 16, accessed: 1
Page 17, accessed: 1
Page 18, accessed: 1
Page 19, accessed: 1
Page 20, accessed: 1
Page 21, accessed: 1
Page 22, accessed: 1
Page 23, accessed: 1
Page 24, accessed: 1
Page 25, accessed: 1
Page 26, accessed: 1
Page 27, accessed: 1
Page 28, accessed: 1
Page 29, accessed: 1
Page 30, accessed: 1
Page 31, accessed: 1
after touching all 8: 0xFF (expect 0xff)
immediately again:    0x0 (expect 0x0)
touched pages 2, 5:   0x24 (expect 0x24)
33 pages: -1 (expect -1)
0 pages:  0 (expect 0)
```

## Part 3: USYSCALL

Added a read-only page at `USYSCALL` (one page below the trapframe) holding the process's pid, allocated in `allocproc()` and mapped with `PTE_R | PTE_U`. `usystest.c` checks that the page matches `getpid()` in a parent and a child, and that writing to it faults. The pids match, and the write caused a store page fault (`scause 0xf`) at the `USYSCALL` address.

```
$ usystest
usyscall pid 3, getpid() 3 (expect equal)
child: usyscall pid 4, getpid() 4 (expect equal)
child: writing to the read-only page...
usertrap(): unexpected scause 0xf pid=4
            sepc=0x64 stval=0x3fffffd000
parent: pid still 3 after child ran
```

`vmprint()` after Part 3 shows the new entry 509 with flags ending in `13` (valid, readable, user-accessible, not writable):

```
page table 0x0000000087f51000
 ..0: pte 0x0000000021fd3401 pa 0x0000000087f4d000
 .. ..0: pte 0x0000000021fd3001 pa 0x0000000087f4c000
 .. .. ..0: pte 0x0000000021fd381b pa 0x0000000087f4e000
 .. .. ..1: pte 0x0000000021fd2c17 pa 0x0000000087f4b000
 .. .. ..2: pte 0x0000000021fd2807 pa 0x0000000087f4a000
 .. .. ..3: pte 0x0000000021fd2417 pa 0x0000000087f49000
 ..255: pte 0x0000000021fd4001 pa 0x0000000087f50000
 .. ..511: pte 0x0000000021fd3c01 pa 0x0000000087f4f000
 .. .. ..509: pte 0x0000000021fd5413 pa 0x0000000087f55000
 .. .. ..510: pte 0x0000000021fd5807 pa 0x0000000087f56000
 .. .. ..511: pte 0x0000000020001c0b pa 0x0000000080007000
```