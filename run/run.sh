#!/bin/bash

echo core | sudo tee /proc/sys/kernel/core_pattern
pushd /sys/devices/system/cpu
echo performance | sudo tee cpu*/cpufreq/scaling_governor
popd

export AFL_SKIP_CPUFREQ=1
# ignore timeout and dryrun perform_dry_run 
rm dry_run_stamp*
echo > afl.log

sudo taskset -c 3 /home/yiluwu/firecracker/AFL/afl-fuzz \
  -b 2 \
  -t 500000000+ -m 256 \
  -i in -o out \
  -d \
  -f seed \
  ../proxy/build/ap @@
