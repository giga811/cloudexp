#!/bin/bash
#shell script

# 引数
if [ $# -ne 4 ]; then
  echo "指定された引数は$#個です。" 1>&2
  echo "usage: script <vm starting id> <vm end id> <filename> <destination>" 1>&2
  exit 1
fi

#clone name
CLONE_VM="debian7-sirius-clone"

#vm number
START_ID=$1
END_ID=$2
FILE_PATH=$3
DESTINATION=$4

echo "コマンド:$VM_COMMANDを実行。"
#main for loop
for i in `seq $START_ID $END_ID`
do
    k=$( printf '%02d:%02d' "$[ SECONDS / 60 ]" "$[ SECONDS % 60 ]" )
    echo "$iコ目のクローンにファイル送信中です。$k"

    #ssh connection and run command $2, option for hostkeycheck
    scp $FILE_PATH ubuntu@10.0.100.$i:$DESTINATION
    # checks end state
    if [ $? -ne 0 ]; then
     echo "Error on sending file at clone$i";
    else
      echo "Success!";
    fi

#end of for loop
done

#end of script
k=$( printf '%02d:%02d' "$[ SECONDS / 60 ]" "$[ SECONDS % 60 ]" )
echo "done. total time:$k"
