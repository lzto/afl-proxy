#!/bin/bash

export AFL_SKIP_CPUFREQ=1
# ignore timeout and dryrun perform_dry_run 
rm dry_run_stamp*
rm afl.log

~/AFL/afl-fuzz \
  -b 0 \
  -t 5000+ -m 256 \
  -i in -o out \
  -f seed \
  -d  -- \
  ../proxy/build/ap /home/tong/afl-proxy/run/seed
