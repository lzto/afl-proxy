#!/bin/bash

SHMID=${1}

cd /home/tong/qemu-afl-image
( ./run-sfp.sh ${SHMID} ) 2>/dev/null 


