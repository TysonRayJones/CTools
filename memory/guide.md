### Measure memory of a Linux process in C

The header file

```C
#include "memorymeasure.h"
```
provides a single function `getMemory` which returns the current and peak, 
real and virtual memories (in bytes, accurate to ~kilobytes) used by your C code's Linux process.
An equivalent module for Python can be found [here](https://github.com/TysonRayJones/PythonTools/tree/master/memory).

Call via
```C
unsigned long currRealMem, peakRealMem, currVirtMem, peakVirtMem;

int success = getMemory(&currRealMem, &peakRealMem, &currVirtMem, &peakVirtMem);
```
where `success = 0` if memory fetching was successful (if the needed linux process files exist).

--------------------

Real memory ([resident set size](https://en.wikipedia.org/wiki/Resident_set_size)) 
is the amount of physical RAM your process is using, and virtual memory is the size of 
the memory address space your process is using. Linux chooses what in your virtual memory gets to
reside in RAM. Note that in addition to your program data, these memories include the space taken up by your code itself, and any libraries your code 
is using (which may be shared by other running processes, skewing your usage). A good explanation can
be found [here](https://superuser.com/questions/618687/why-do-programs-on-linux-kernel-use-so-much-more-vmem-than-resident-memory).

Peak memory is the maximum amount of memory your process has used over its lifetime so far.

---------------------

This function works by reading `/proc/self/status` which linux populates with info about your running process.
It uses fields

| Output      | status Field  | Description  |
| ------------|:-------------:|:-------------|
| currRealMem | VmRSS  | Resident set size |
| peakRealMem | VmHWM  | Peak resident set size ("high water mark") |
| currVirtMem | VmSize | Virtual memory size |
| peakVirtMem | VmPeak | Peak virtual memory size |

taken from under /proc/[pid]/status on the [proc man page](https://linux.die.net/man/5/proc).

See [procps](https://gitlab.com/procps-ng/procps) for a much more versatile tool in analysing the linux process table.
