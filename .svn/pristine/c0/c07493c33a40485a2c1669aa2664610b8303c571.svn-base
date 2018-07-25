#!/bin/sh

. ~/sg_server/nohup_run/common.sh 

monitor_time=300 #秒
robot_log_file=$rm_log_dir/robot.log
robot_server_dir=$work_path/robot_server/

function robot_check_servers()
{
    #后台启动机器人
    cd $robot_server_dir
    nohup ./robot_server wsy123mon &>$robot_log_file &
    sleep 20

    #执行完关闭
    #ps aux | grep -w $ps_name | grep "robot_server wsy123mon" | grep -v 'grep' | awk '{print $2}' | xargs kill -9 &>/dev/null
    ps aux | grep -w $ps_name | grep "robot_server wsy123mon" | grep -v 'grep' | awk '{print $2}' | xargs kill -s 15 &>/dev/null
    sleep 10

    #分析日志
    local rlt=`cat $robot_log_file | grep -c "all the 1 robots enter success"`
    if [ $rlt -eq 0 ]; then 
        echo "[`date '+%Y-%m-%d %H:%M:%S'`] 机器人登陆失败，重启服务器！"
    else
        echo "[`date '+%Y-%m-%d %H:%M:%S'`] 机器人登陆成功！"
    fi
}

#######################################################################################################
while [ true ]
do
    robot_check_servers
    
    sleep $monitor_time
done