#!/bin/bash
echo "Run count-success.sh first!"
S2E_PROJECT_DIR="/home/tong/s2e/projects/"
TOOL_DIR="/home/tong/s2e-tools"

#find . -name debug.txt  -exec grep Solved -l -I {} \; | tee result.txt

cat result.txt | cut -d'/' -f2 |uniq > donelist.txt

data=`cat donelist.txt`
pushd $S2E_PROJECT_DIR

for p in ${data[@]};do 
  mv ${p} ../archive/
  mv ${p}.zip ../archive/
done

popd
