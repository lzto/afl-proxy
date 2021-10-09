#!/bin/bash

d=$1
name=$2

pushd $d

if [ "$name" == "" ]; then
  name=`pwd|rev|cut -d'/' -f1 |rev `
fi

find . -name "dmesg*"  -exec grep -i $name {} \;|cut -d']' -f2-|sort |uniq |grep -v enabling

popd

