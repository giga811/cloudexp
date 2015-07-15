#!/bin/bash
#shell script

# 引数
if [ $# -ne 3 ]; then
  echo "指定された引数は$#個です。" 1>&2
  echo "usage: script <vm starting id> <vm end id> <command>" 1>&2
  exit 1
fi

#vm name
NODE_VM="cloud-exp-node"

#vm number
START_ID=$1
END_ID=$2
VM_COMMAND=$3

echo "コマンド:$VM_COMMANDを実行。"
#main for loop
for i in `seq $START_ID $END_ID`
do
	k=$( printf '%02d:%02d' "$[ SECONDS / 60 ]" "$[ SECONDS % 60 ]" )
	echo "$iコ目のクローンにコマンド実行中です。$k"

	#ssh connection and run command $2, option for hostkeycheck
	ssh ubuntu@10.0.100.$i -o "StrictHostKeyChecking no" $VM_COMMAND
	# checks end state
	if [ $? -ne 0 ]; then
 	 echo "Error on running command at node$i";
	else
	  echo "Command run Success! on $i";
	fi

#end of for loop
done

#end of script
k=$( printf '%02d:%02d' "$[ SECONDS / 60 ]" "$[ SECONDS % 60 ]" )
echo "done. total time:$k"
