#!/bin/bash

name=$1
VID=$2
PID=$3
SUBVID=$4
SUBPID=$5
# need to add extra 00 at the end 
CLASS=${6}00

if [ "$name" == "" ]; then
  echo "need a name"
  exit
fi

if [ -f "$name" ]; then
  echo "$name already exists!"
  exit
fi

cp -rv template $name
cp /home/tong/shf/a.sh /home/tong/shf/$name.sh
sed -i s/KONAME/$name/g /home/tong/shf/$name.sh

pushd $name
ln -sf /home/tong/shf/$name.sh ./a.sh
sed -i s/KONAME/$name/g s2e-config.lua
sed -i s/SFPVID/${VID}/g s2e-config.lua
sed -i s/SFPPID/${PID}/g s2e-config.lua
sed -i s/SFPSUBVID/${SUBVID}/g s2e-config.lua
sed -i s/SFPSUBPID/${SUBPID}/g s2e-config.lua
sed -i s/SFPCLASS/${CLASS}/g s2e-config.lua
popd
echo "Remember to edit s2e-config.lua and update VID,PID"



