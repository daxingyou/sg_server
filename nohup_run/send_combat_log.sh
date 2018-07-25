#!/bin/sh
. ~/sg_server/sh/config.sh 

function send_combat_log()
{
	local arr=()
	
	if [ ! -d $combat_log_dir ];then
		mkdir -p $combat_log_dir
	fi

	#创建对端目录
	sshpass -p $combat_log_dst_passwd ssh -o GSSAPIAuthentication=no $combat_log_dst_user@$combat_log_dst_ip "mkdir -p $combat_log_dst_dir"

	#发送删除对端过期战斗回放日志脚本
	#sshpass -p $combat_log_dst_passwd scp -C -o GSSAPIAuthentication=no "./delete_combat_logs.sh" $combat_log_dst_user@$combat_log_dst_ip:$combat_log_dst_dir

	#发送
	for log in $combat_log_dir/*
	do
		if [ -f $log ];then
			if [[ "$log" =~ ".fight" ]];then
				sshpass -p $combat_log_dst_passwd scp -C -o GSSAPIAuthentication=no $log $combat_log_dst_user@$combat_log_dst_ip:$combat_log_dst_dir
				if [ "$combat_log_backup_dir" = "" ];then
					rm -f $log
				else
					arr=(${arr[*]} $log)
				fi
			fi
		fi
	done

	#本地备份
	local logs_num=${#arr[@]}
	if [ $logs_num -gt 0 ];then
		if [ ! -d $combat_log_backup_dir ];then
			mkdir -p $combat_log_backup_dir
		fi

		for backup_log in ${arr[@]}
		do
			if [ -f $backup_log ];then
				mv $backup_log $combat_log_backup_dir
			fi
		done
	fi

	unset arr
}

rlt=$(sshpass -V)
if [ "$rlt" = "" ];then
	exit
fi

if [ "$1" = "once" ];then
    send_combat_log
else
    while [ true ]
    do
        send_combat_log
        sleep $send_time
    done
fi
