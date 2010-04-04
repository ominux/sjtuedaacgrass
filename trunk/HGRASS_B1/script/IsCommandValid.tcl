#!/bin/sh
path=$PATH:

while [ $path ]; do
  p=${path%%:*}
  path=${path#*:}
  filelist=`ls $p 2>/dev/null`
  for q in $filelist
  do
    if [ $q == $1 ]; then
      exit 0
    fi
  done
done
exit 1
