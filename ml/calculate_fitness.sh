#!/bin/bash
cd ${1}
Ns=`grep  "ERR\|-E\|WARN" sundance.c.gcov |grep -Po "^\\s+\\d+"`
N=`printf "0\n" ${Ns} | paste -sd+ -| bc`
P=`grep -Po "\\d+\.\\d+" report.txt | paste -sd+ -| bc`
echo ${P}-${N} | bc
