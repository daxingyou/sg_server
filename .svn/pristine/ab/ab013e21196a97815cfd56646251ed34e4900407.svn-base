#!/bin/sh

. ~/sg_server/nohup_run/common.sh

#目标机器
copy_dst_ip="47.100.130.174"
copy_dst_user_name_list="jy nms"
copy_dst_main_user_name="jy"

au_log="$(cd `dirname $0`; pwd)/au.log"

###########################################################################################################
function auto_stop()
{ 
    start_stop $1 $2  &>/dev/null
}

function auto_run()
{
    start_run $1 $2  &>/dev/null
}

function auto_up()
{
    local copy_dst_user_name=$1
    local copy_dst_user_passwd=$copy_dst_user_name
    
    if [ ! -d $monitor_dir ];then
        mkdir -p $monitor_dir
    fi
    
    echo "[`date '+%Y-%m-%d %H:%M:%S'`] 开始自动更新[$copy_dst_user_name@$copy_dst_ip]......" >> $au_log
    
    copy_check_input_param $copy_dst_ip $copy_dst_user_name all
    
    copy_scp -f $copy_src_main_path/nohup_run/auto_up.sh  $copy_dst_main_path/nohup_run/
    
    echo "关服" >> $au_log
    if [ "$copy_dst_user_name" = "$copy_dst_main_user_name" ];then
        copy_ssh_cmd "sh /home/$copy_dst_user_name/sg_server/nohup_run/auto_up.sh stop account"
        copy_ssh_cmd "sh /home/$copy_dst_user_name/sg_server/nohup_run/auto_up.sh stop global"
    fi
    copy_ssh_cmd "sh /home/$copy_dst_user_name/sg_server/nohup_run/auto_up.sh stop"
    
    echo "拷贝: $user_name@$copy_src_ip:$copy_src_work_path To $copy_dst_user_name@$copy_dst_ip:$copy_dst_work_path" >> $au_log
    copy_all_servers >> $au_log

    echo "开服" >> $au_log
    copy_ssh_cmd "sh /home/$copy_dst_user_name/sg_server/nohup_run/auto_up.sh run"
    if [ "$copy_dst_user_name" = "$copy_dst_main_user_name" ];then
        copy_ssh_cmd "sh /home/$copy_dst_user_name/sg_server/nohup_run/auto_up.sh run global"
        copy_ssh_cmd "sh /home/$copy_dst_user_name/sg_server/nohup_run/auto_up.sh run account"
    fi
    
    echo "[`date '+%Y-%m-%d %H:%M:%S'`] 结束自动更新！" >> $au_log
}

###########################################################################################################
if [ "$1" = "stop" ];then
    auto_stop $2 $3
    
elif [ "$1" = "run" ];then
    auto_run $2 $3
    
else
    
    for user in $copy_dst_user_name_list
    do
        auto_up $user
    done
fi
