# Linux Proc File System
## If run without an argument, dumps information about the system to STDOUT.

**./getstats**

model name      : 12th Gen Intel(R) Core(TM) i5-12400

cpu cores       : 6

Linux version 5.15.90.1-microsoft-standard-WSL2 (oe-user@oe-host) (x86_64-msft-linux-gcc (GCC) 9.3.0, GNU ld (GNU Binutils) 2.34.0.20200220) #1 SMP Fri Jan 27 02:56:13 UTC 2023

MemTotal:        3873904 kB

Uptime: 0 days, 18 hours, 35 minutes, 29 seconds

## If run with a process number created by the current user, dumps information about that process to STDOUT.

**./getstats 238**

Process number: 238

Name:   SessionLeader

Command Line: /init

Threads: 1

Total Context Switches: 2
