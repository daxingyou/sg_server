#!/bin/bash

#sh db_statistica_log.sh
#sh db_statistica_log.sh '2018-04-01' '2018-04-26'

function ex_sql()
{
    local sql=$1
    mysql -h${db_ip} -u${db_user} -p${db_pwd} -S${db_socket} ${db_name} -N -s -e "$sql" 1>>$ss_log 2>/dev/null
}

function statistica()
{
    local ss_log="$(cd `dirname $0`; pwd)/ss.log" 
    if [ ! -f $ss_log ]; then
        touch $ss_log
    fi

    echo "[`date '+%Y-%m-%d %H:%M:%S'`] get data from [$find_start_time] to [$find_end_time]" >>$ss_log
    
    #统计-新增玩家（设备 账号 角色）
    ex_sql "call sp_new_user('$find_start_time', '$find_end_time');"
    
    #统计-活跃玩家(账号)
    ex_sql "call sp_active_user('$find_start_time', '$find_end_time');"
    
    #统计-留存玩家(账号)
    ex_sql "call sp_remain_user('$find_start_time', '$find_end_time');"
    
    #统计-流失和回流玩家(账号)
    ex_sql "call sp_leave_return_user('$find_start_time', '$find_end_time');"
}

#######################################################################################################################
# SQL相关
db_ip="127.0.0.1"
db_user="root"
db_pwd="Sanguo1!"
db_name="sg_statistics"
db_socket="/var/lib/mysql/mysql.sock"
    
default=0
#前一天
find_start_time=`date -d -1day +%Y-%m-%d`
find_end_time=`date -d -1day +%Y-%m-%d`
#今天
#find_start_time=`date +%Y-%m-%d`
#find_end_time=`date +%Y-%m-%d`

if [ "$1" = "" -a "$2" = "" ];then
    default=1
fi
if [ "$1" != "" ];then
    if [[ "$1" =~ "-" ]];then
        find_start_time=$1
    else
        echo -e "\033[1;31m时间参数不规范，应用：2018-06-25\033[0m"
        exit
    fi
fi
if [ "$2" != "" ];then
    if [[ "$2" =~ "-" ]];then
        find_end_time=$1
    else
        echo -e "\033[1;31m时间参数不规范，应用：2018-06-25\033[0m"
        exit
    fi
fi

if [ $default -eq 1 ];then
    statistica
else
    while [ true ]
    do
        echo -e "\033[1;36m确定查询的日期为:\033[0m \033[1;37m[$find_start_time]至[$find_end_time]\033[0m(y/n)? \c"
        read answer
        if [ "$answer" = y ];then

            statistica
            
            break
        elif [ "$answer" = n ];then
            break
       fi
    done
fi