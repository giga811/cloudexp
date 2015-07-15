#!/bin/bash
#shell script

# 引数
if [ $# -ne 1 ]; then
  echo "指定された引数は$#個です。" 1>&2
  echo "usage: script <vm clone id>" 1>&2
  exit 1
fi

# clone id
i=$1

ssh ubuntu@10.0.100.$i -o "StrictHostKeyChecking no"
