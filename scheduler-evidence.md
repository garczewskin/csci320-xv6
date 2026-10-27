# Stride Scheduler: Evidence

## How it should behave

Each time a process is chosen, `runtime` increases by 1 and `pass` increases by the process's `stride`. The scheduler always picks the RUNNABLE process with the lowest `pass`, so a process with stride `s` runs about `1/s` as often as a stride-1 process.

Expected share of CPU for job `i`: `(1/s_i) / sum(1/s_j)` over all running jobs.

## Setup

- Boot: `make qemu CPUS=1` (single CPU). `MAXPASSTOTAL = 1000000` for Tests 1, 3, and 4, and `MAXPASSTOTAL = 2000` for Test 2.
- Jobs: `hardwork <stride> &` (calls `setStride`).
- Tests 1 and 2 use repeated `pinfo` runs typed by hand.
- Tests 3 and 4 use `monitor 100 30`: 30 snapshots of `getpinfo`, 100 ticks apart. Share = a job's runtime gain in the interval divided by the total gain of all listed jobs.


## Test 1: strides 1, 2, 4, 8

Runtime gain from snapshot A to snapshot C:

| PID | Stride | Runtime gain | Observed share | Expected share |
|---|---|---|---|---|
| 6 | 1 | 722 | 53.32% | 53.33% |
| 8 | 2 | 361 | 26.66% | 26.67% |
| 10 | 4 | 180 | 13.29% | 13.33% |
| 12 | 8 | 91 | 6.72% | 6.67% |

Raw snapshots (columns ordered by stride 1, 2, 4, 8):

| Snapshot | passTotal | Runtime | Pass |
|---|---|---|---|
| A | 648 | 138, 70, 36, 18 | 138, 139, 141, 137 |
| B | 2039 | 483, 242, 122, 62 | 483, 483, 485, 489 |
| C | 3549 | 860, 431, 216, 109 | 860, 861, 861, 865 |

Runtimes differ by 8x while pass values stay within 6 of each other (largest stride is 8).

## Test 2: strides 1, 2, 4, 8 with pass reset (`MAXPASSTOTAL = 2000`)

Strides 1, 2, 4, 8 on PIDs 4, 6, 8, 10. `passTotal` climbed from 454 to 1783 over eight snapshots, then reset.

| Snapshot | passTotal | Runtime (PIDs 4, 6, 8, 10) | Pass (PIDs 4, 6, 8, 10) |
|---|---|---|---|
| Before reset | 1783 | 425, 213, 107, 54 | 425, 425, 425, 425 |
| After reset | 9 | 479, 241, 122, 61 | 1, 2, 4, 0 |
| Next snapshot | 249 | 538, 270, 136, 69 | 60, 60, 60, 64 |

`passTotal` and all `pass` values return to near 0. `runtime` keeps increasing, and the 8:4:2:1 split continues after the reset.

## Test 3: four jobs, all stride 1

Runtime gain per interval (PIDs 4, 6, 8, 10):

| Interval | Total runs | PID 4 | PID 6 | PID 8 | PID 10 |
|---|---|---|---|---|---|
| 1 | 101 | 0 | 33 | 34 | 34 |
| 2 | 103 | 7 | 32 | 32 | 32 |
| 3 to 30 (each) | 100 | 25 | 25 | 25 | 25 |

Intervals 3 to 30: 700 runs per job, 25.0% each, expected 25%. Intervals 1 and 2 show jobs started later catching up with the first one.

## Test 4: strides 3, 5, 7

| PID | Stride | Runtime gain (30 intervals) | Observed share | Expected share | Per-interval range |
|---|---|---|---|---|---|
| 4 | 3 | 1479 | 49.30% | 49.30% | 49 to 50 |
| 6 | 5 | 887 | 29.57% | 29.58% | 29 to 30 |
| 8 | 7 | 634 | 21.13% | 21.13% | 21 to 22 |

3000 runs total (100 per interval). Pass values were within 6 of each other in every snapshot (largest stride is 7).
