#!/bin/bash

name=$1

if [ "$name" == "" ]; then
  name=`pwd|rev|cut -d'/' -f1 |rev `
fi

find . -name dmesg  -exec grep -i $name {} \;|cut -d']' -f2-|sort |uniq |grep -v enabling
