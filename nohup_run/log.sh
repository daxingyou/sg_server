#!/bin/sh

#sh log.sh game
#sh log.sh gate 1

. ~/sg_server/nohup_run/common.sh 

srv_name=$1
log_lv=$2
srv_index=$3

log_dir=$srv_name"_server"
log_file=$srv_name
is_server=1

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

elif [ "$srv_name" = "account" -a -n "$srv_index" ];then
    if [ $srv_index -gt 0 -a $srv_index -le $account_number ];then
        log_dir=$srv_name"_server"_$srv_index
    fi
    
elif [ "$srv_name" = "glog" ];then
    log_file="log"

elif [ "$srv_name" = "sm" ];then
	is_server=0
    tail -f $main_path/logs/sm_log/monitor.log
    
elif [ "$srv_name" = "cl" ];then
	is_server=0
    tail -f $main_path/logs/combat_logs/monitor.log
 
elif [ "$srv_name" = "dba" ];then
	is_server=0
    tail -f $main_path/logs/dba_log/monitor.log
    
fi

if [ $is_server -eq 1 ];then
	cd $work_path/$log_dir/logs
	server_log="$log_file`date '+%Y%m%d'`.log"
	echo $work_path/$log_dir/logs/$server_log
	if [ "$log_lv" = "d" ]; then
	    tail -f $server_log | grep --line-buffer "\[debug\]"
	elif [ "$log_lv" = "i" ]; then
	    tail -f $server_log | grep --line-buffer "\[info\]"
	elif [ "$log_lv" = "t" ]; then
	    tail -f $server_log | grep --line-buffer "\[trace\]"
	elif [ "$log_lv" = "w" ]; then
	    tail -f $server_log | grep --line-buffer "\[warn\]"
	elif [ "$log_lv" = "e" ]; then
	    tail -f $server_log | grep --line-buffer "\[error\]"
	elif [ "$log_lv" = "a" ]; then
	    tail -f $server_log
	fi
fi

