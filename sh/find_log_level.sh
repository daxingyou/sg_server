#!/bin/sh

#sh find_log_level.sh 20180620 info

. ~/sg_server/sh/config.sh

########################################################################
function find_key()
{
    while read -r row
    do
        local find_log_level=`echo $row | awk -F ' ' '{print $2}'`
        if [ "$find_log_level" = "$log_level" ];then
            local key_1=`echo $row | awk -F ':' '{print $4}'`
            local key_2=`echo $row | awk -F ':' '{print $5}'`
            local key_3=`echo $key_2 | awk -F ']' '{print $1}'`
            key=$key_1":"$key_3 
            if [ "$key" != "" ];then
                not_key=0
                echo $row >> $new_log_file_path
                return
            fi
        fi
    done < $tmp_log_file_path

    not_key=1
}

function filter_key()
{
    local logs_path=$1
    if [ ! -d $logs_path ];then
        return
    fi
    
    local log_file=$2
    local log_file_path=$logs_path/$log_file
    if [ ! -f $log_file_path ];then
        return
    fi
    
    local tmp_log_file_path=$logs_path/"tmp_"$log_file
    rm -f $tmp_log_file_path
    cp $log_file_path $tmp_log_file_path

    local new_log_file=$3
    local new_log_file_path=$save_path/$log_level$new_log_file
    rm -f $new_log_file_path
    touch $new_log_file_path

    local key=""
    local not_key=0

    while [ $not_key -eq 0 ]
    do
        find_key
        if [ "$key" != "" ];then
            sed -i "/${key}/d" $tmp_log_file_path
        fi
    done 
    
    rm -f $tmp_log_file_path
    
    echo "完成：$log_file提取$log_level => $new_log_file_path"
}

########################################################################
log_date=$1
if [ "$log_date" = "" ];then
    exit
fi
log_level="[$2]"
if [ "$log_level" = "" ];then
    exit
fi

save_path="$(cd `dirname $0`; pwd)/log_level"
if [ ! -d $save_path ];then
    mkdir -p $save_path
fi

server_list=$run_group_server_list" "$all_global_server_list
for s in $server_list
do
    if [ "$s" = "gate" -a $gate_number -gt 1 ];then
        for ((i=1; i<=$gate_number; i++))  
        do
            srv_name=$s"_server"_$i
            filter_key $work_path/$srv_name/logs $s$log_date.log $srv_name$log_date.log
        done
        
    elif [ "$s" = "game" -a $game_number -gt 1 ];then
        for ((i=1; i<=$game_number; i++))  
        do
            srv_name=$s"_server"_$i
            filter_key $work_path/$srv_name/logs $s$log_date.log $srv_name$log_date.log
        done

    elif [ "$s" = "cross" -a $cross_number -gt 1 ];then
        for ((i=1; i<=$cross_number; i++))  
        do
            srv_name=$s"_server"_$i
            filter_key $work_path/$srv_name/logs "game"$log_date.log $srv_name$log_date.log
        done
        
    elif [ "$s" = "transfer" -a $transfer_number -gt 1 ];then
        for ((i=1; i<=$transfer_number; i++))  
        do
            srv_name=$s"_server"_$i
            filter_key $work_path/$srv_name/logs $s$log_date.log $srv_name$log_date.log
        done
    
    elif [ "$s" = "account" -a $account_number -gt 1 ];then
        for ((i=1; i<=$account_number; i++))  
        do
            srv_name=$s"_server"_$i
            filter_key $work_path/$srv_name/logs $s$log_date.log $srv_name$log_date.log
        done
        
    else
        srv_name=$s"_server"
        if [ "$s" = "glog" ];then
            filter_key $work_path/$srv_name/logs "log"$log_date.log $srv_name$log_date.log
        else
            filter_key $work_path/$srv_name/logs $s$log_date.log $srv_name$log_date.log
        fi
    fi
done
