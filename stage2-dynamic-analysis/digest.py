#!/usr/bin/python3
# 2022 Tong Zhang<ztong0001@gmail.com>

import sys
import re

def usage():
    print("digest.py qemu-w.log kvm-trace.log")
    exit()

if len(sys.argv) < 2:
    usage()

fn_wlog = sys.argv[1]
fn_klog = sys.argv[2]

# store the value to register mapping
addr2reg = {}

# process qemu-w.log
fh_wlog = open(fn_wlog, 'r')
Lines = fh_wlog.readlines()

for line in Lines:
    line = line.strip()
    r = re.search("write", line)
    if r:
        arr=re.split(r" |=",line)
        reg=arr[6];
        val=arr[7];
        if val in addr2reg.keys():
            addr2reg[val].add(reg)
        else:
            addr2reg[val] = {reg}

#for k in addr2reg.keys():
#    print(k, addr2reg[k])

fh_wlog.close()
# process kvm-trace.log
fh_klog = open(fn_klog, 'r')
Lines = fh_klog.readlines()

dmaaddrs=set()

for line in Lines:
    line = line.strip()
    r = re.search("qemu", line)
    if r:
        arr = re.split(r" ",line)
        if (arr[-1]!="0x0"):
            dmaaddrs.add(arr[-1])

print("Register, DMA Buffer Address")
for a in dmaaddrs:
    if a in addr2reg.keys():
        print(addr2reg[a],a)



