#!/bin/bash
find . -name debug.txt  -exec grep Solved -l -I {} \; | tee result.txt
cat result.txt | cut -d'/' -f2 |uniq | wc -l



