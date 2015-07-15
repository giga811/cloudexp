#!/bin/bash
#shell script

# 引数
if [ $# -ne 2 ]; then
  echo "指定された引数は$#個です。" 1>&2
  echo "usage: script <command> <vm id>" 1>&2
  exit 1
fi

#vm name
NODE_VM="cloud-exp-node"

#inputs
VM_ID=$2
VM_COMMAND=$1

#echo "コマンド:$VM_COMMANDを実行。"
	#ssh connection and run command, option for hostkeycheck
	ssh ubuntu@10.0.100.$VM_ID -o "StrictHostKeyChecking no" $VM_COMMAND
	# checks end state
	if [ $? -ne 0 ]; then
 	 echo "***Error on running command at clone$VM_ID. Retrying in 3 sec."
	 sleep 3
	 ssh ubuntu@10.0.100.$VM_ID -o "StrictHostKeyChecking no" $VM_COMMAND;
	else
	  k=$( printf '%02d:%02d' "$[ SECONDS / 60 ]" "$[ SECONDS % 60 ]" )
	  #echo "Command run Success! on $VM_ID. time: $k";
	fi
#end of script

