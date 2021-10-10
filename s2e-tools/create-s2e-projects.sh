#!/bin/bash
# create s2e projects

S2E_DIR=/home/tong/s2e/

function run() {
SFPVID=$1
SFPPID=$2
SFP_SUBVID=$3
SFP_SUBPID=$4
PCI_CLASS=$5
KO=$6

if [ "$PCI_CLASS" == "0x000000" ]; then
  PCI_CLASS="0xff00"
fi

#echo "I should create project for ${KO}"
pushd ${S2E_DIR}/projects > /dev/null
  if [ ! -d "${KO}" ];then
    echo "creating ${KO}"
    ./prepare.sh ${KO} ${SFPVID} ${SFPPID} ${SFP_SUBVID} ${SFP_SUBPID} ${PCI_CLASS}
  fi
popd > /dev/null


}

data=`cat pci-ko-data.csv`
for line in ${data[@]};do
  VID=`echo $line | cut -d',' -f1`
  PID=`echo $line | cut -d',' -f2`
  SVID=`echo $line | cut -d',' -f3`
  SPID=`echo $line | cut -d',' -f4`
  BC=`echo $line | cut -d',' -f5`
  SC=`echo $line | cut -d',' -f6`
  PI=`echo $line | cut -d',' -f7`
  KO=`echo $line | cut -d',' -f8`
  #echo 0x$VID,0x$PID,$KO
  r=`curl "http://ubuntu-server:3000/qsolved?p=${KO}" 2>/dev/null`
  if [ "$r" != "0" ]; then
    continue
  fi

  if [ "$SVID" == "" ]; then
    SVID="0"
  fi
  if [ "$SPID" == "" ]; then
    SPID="0"
  fi
  if [ "$BC" == "" ]; then
    BC="0"
  fi
  if [ "$SC" == "" ]; then
    SC="0"
  fi
  if [ "$PI" == "" ]; then
    PI="0"
  fi
  PCI_CLASS=`/usr/bin/printf "0x%02x%02x%02x" "0x${BC}" "0x${SC}" "0x${PI}"`
  run 0x$VID 0x$PID 0x$SVID 0x$SPID $PCI_CLASS $KO
done

