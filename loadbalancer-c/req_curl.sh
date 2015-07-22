#!/bin/bash
#shell script

# 引数
if [ $# -lt 2 ]; then
  echo "指定された引数は$#個です。" 1>&2
  echo "usage: script <rr|lc|bsr> <1-100>" 1>&2
  exit 1
fi

METHOD=$1
X=$2

# seq 1 10 | xargs -L 1 -P 1 time sh req_curl.sh bsr 40

if [[ $METHOD == "rr" ]]; then
    #statements
    curl 172.20.50.137/exp/exp1/req/$X
elif [[ $METHOD == "lc" ]]; then
    #statements
    curl 172.20.50.142/exp/exp1/req/$X
elif [[ $METHOD == "bsr" ]]; then
    #statements
    curl 172.20.50.1:22641/$X
fi



