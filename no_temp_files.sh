#!/bin/bash
n=`ls tmpfile* | wc -l`
if [ $n -eq 0 ]
then
  echo "No temporary files"
  exit 0
else
  echo "Detected "$n" temporary files"
  exit $n
fi
