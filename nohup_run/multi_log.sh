#!/bin/sh

#sh multi_log.sh info login,gate:1,db

. ~/sg_server/nohup_run/common.sh 

function multi_log()
{
	local log_cmd=""

	for server_log in $*  
	do  
	    OLD_IFS="$IFS" 
		IFS=":" 
		local log_param_list=($server_log)
		IFS="$OLD_IFS"

		local srv_name=${log_param_list[0]} 
		local srv_index=${log_param_list[1]} 
		local log_dir=$srv_name"_server"
		local log_file=$srv_name
		if [ "$srv_name" = "gate" -a -n "$srv_index" ];then
		    if [ $srv_index -gt 0 -a $srv_index -le $gate_number ];then
		        log_dir=$srv_name"_server"_$srv_index
		    fi
		    
		elif [ "$srv_name" = "game" -a -n "$srv_index" ];then
		    if [ $srv_index -gt 0 -a $srv_index -le $game_number ];then
		        log_dir=$srv_name"_server"_$srv_index
		    fi

		elif [ "$srv_name" = "cross" ];then
		    log_file="game"
		    if [ -n "$srv_index" ];then
		        if [ $srv_index -gt 0 -a $srv_index -le $cross_number ];then
		            log_dir=$srv_name"_server"_$srv_index
		        fi
		    fi

		elif [ "$srv_name" = "transfer" ];then
		    log_file="transfer"
		    if [ -n "$srv_index" ];then
		        if [ $srv_index -gt 0 -a $srv_index -le $transfer_number ];then
		            log_dir=$srv_name"_server"_$srv_index
		        fi
		    fi 
            
		elif [ "$srv_name" = "account" ];then
		    log_file="account"
		    if [ -n "$srv_index" ];then
		        if [ $srv_index -gt 0 -a $srv_index -le $account_number ];then
		            log_dir=$srv_name"_server"_$srv_index
		        fi
		    fi 
            
		elif [ "$srv_name" = "glog" ];then
		    log_file="log"
		    
		fi

		local server_log="$work_path/$log_dir/logs/$log_file`date '+%Y%m%d'`.log"
		log_cmd=$log_cmd$log_lv$server_log" "

	done 

	if [ "$log_cmd" != "" ];then
		multitail $log_cmd
	fi
}

log_lv=$1
if [ "$log_lv" = "" ]; then
    exit
fi
if [ "$log_lv" = "all" ]; then
	log_lv=" "
else
	log_lv="-e \[$log_lv\] "
fi

server_list=$2
if [ "$server_list" = "" ]; then
    exit
fi
OLD_IFS="$IFS" 
IFS="," 
server_param_list=($server_list)
IFS="$OLD_IFS"
param=""
for s in ${server_param_list[@]} 
do
	OLD_IFS="$IFS" 
	IFS=":" 
	slist=($s)
	IFS="$OLD_IFS"
	srv_name=${slist[0]} 
	srv_index=${slist[1]} 
	if [ "$srv_index" = "" ];then
		param=$param" $srv_name:0"
	else
		param=$param" $srv_name:$srv_index"
	fi
done
multi_log $param