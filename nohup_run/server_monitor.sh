#!/bin/sh

. ~/sg_server/nohup_run/common.sh 

monitor_list="db game center chat gate login gm"

########################################################################
function monitor_server()
{
	local srv_name=$1
	check_server './'$srv_name &>/dev/null
    if [ $? -ne 1 ];then
        stop_list=$stop_list$srv_name" "
        check_core $srv_name &>/dev/null
    fi
}

function monitor_all_servers()
{
	local stop_list=""

    for s in $monitor_list
    do
    	local is_skip=0
    	for skip in $skip_server
    	do
    		if [ "$s" = "$skip" ];then
    			is_skip=1
    			continue
    		fi
    	done
    	if [ $is_skip -eq 1 ];then
    		continue
    	fi

    	if [ "$s" = "gate" -a $gate_number -gt 1 ];then
            for ((i=1; i<=$gate_number; i++))  
            do
                srv_name=$s"_server"_$i
                monitor_server $srv_name $stop_list
            done
            
        elif [ "$s" = "game" -a $game_number -gt 1 ];then
            for ((i=1; i<=$game_number; i++))  
            do
                srv_name=$s"_server"_$i
                monitor_server $srv_name $stop_list
            done

        elif [ "$s" = "cross" -a $cross_number -gt 1 ];then
            for ((i=1; i<=$cross_number; i++))  
            do
                srv_name=$s"_server"_$i
                monitor_server $srv_name $stop_list
            done
            
        elif [ "$s" = "transfer" -a $transfer_number -gt 1 ];then
            for ((i=1; i<=$transfer_number; i++))  
            do
                srv_name=$s"_server"_$i
                monitor_server $srv_name $stop_list
            done
        
        elif [ "$s" = "account" -a $account_number -gt 1 ];then
            for ((i=1; i<=$account_number; i++))  
            do
                srv_name=$s"_server"_$i
                monitor_server $srv_name $stop_list
            done
            
        else
            srv_name=$s"_server"
            monitor_server $srv_name $stop_list
        fi
    done

    if [ "$stop_list" != "" ];then
    	echo "[`date '+%Y-%m-%d %H:%M:%S'`] [$stop_list] has been stop!"
    fi
    backup_server &>/dev/null
   
    for stop in $stop_list
    do
        run_server $stop &>/dev/null
        check_server $stop
        if [ $? -eq 1 ];then
            echo "$stop is already running!"
        else
            echo "$stop failed to run!"
        fi   
    done
    unset stop_list
}

########################################################################
if [ "$1" = "once" ];then
    monitor_all_servers
else
    while [ true ]
    do
        monitor_all_servers
        sleep 5
    done
fi
