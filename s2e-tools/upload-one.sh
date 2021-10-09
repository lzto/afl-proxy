#!/bin/bash

project=${1}
echo "Compressing ${project}"
zip -y -r ${project}.zip ${project}
echo "Uploading"
curl -F "file=@./${project}.zip" http://ubuntu-server:3000/s2e-upload/api_upload
echo "[OK]"
