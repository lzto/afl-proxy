#!/bin/sh

for line in *;do
  r=`curl "http://ubuntu-server:3000/qsolved?p=${line}" 2>/dev/null`
  if [ "$r" != "0" ]; then
    echo ${line}
  fi
done

