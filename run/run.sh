#!/bin/bash

export AFL_SKIP_CPUFREQ=1
# ignore timeout and dryrun perform_dry_run 
rm dry_run_stamp*
echo > afl.log

/home/tong/AFL/afl-fuzz \
  -b 2 \
  -t 500000000+ -m 256 \
  -i in -o out \
  -d \
  -f seed \
  ../proxy/build/ap @@
