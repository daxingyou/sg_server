#!/bin/bash

# 只用于本地mysql
#sh r_db_backup.sh
#sh r_db_backup.sh source /root/sh/db_log/db_bak/20180525/120001/nei_trunk_sg_game nei_trunk_sg_game"

dump_time=0 #秒 0代表只备份一次，可配合crontab使用
dump_db_list="sg_account sg_area sg_glog sg_game sg_log sg_gm sg_bbs sg_statistics sg_up"

db_user="root"
db_pwd="Sanguo1!"

####################################################
db_log_path="$(cd `dirname $0`; pwd)/dba_log" 
if [ ! -d "${db_log_path}" ]; then
    mkdir -p ${db_log_path}
fi
    
dump_path="$db_log_path/db_bak" 
if [ ! -d "${dump_path}" ]; then
    mkdir -p ${dump_path}
fi

monitor_log="$db_log_path/monitor.log"
if [ ! -f "${monitor_log}" ]; then
    touch ${monitor_log}
fi

####################################################
#导出
function dump_db()
{
	local now_date=$(date +%Y%m%d)
	local now_time=$(date +%H%M%S)

	local db_name=$1
	local db_dump_path=$dump_path/$now_date/$now_time
    if [ ! -d "${db_dump_path}" ]; then
        mkdir -p ${db_dump_path}
    fi
	local db_dump_file=$db_dump_path/$db_name.sql.gz

	#/usr/bin/mysqldump -u$db_user -p$db_pwd --single-transaction --master-data=2 --default-character-set=utf8 $db_name | gzip > $db_dump_file
    /usr/bin/mysqldump -u$db_user -p$db_pwd --default-character-set=utf8 $db_name | gzip > $db_dump_file
	echo "[`date '+%Y-%m-%d %H:%M:%S'`] 完成备份[$db_name]: $db_dump_file" | tee -a $monitor_log
}
function do_dump_db()
{
    echo "[`date '+%Y-%m-%d %H:%M:%S'`] 开始备份......" | tee -a $monitor_log
    for db1 in $dump_db_list
    do
        local db_list=$(mysql -u$db_user -p$db_pwd  -e "show databases;")
        for db2 in $db_list
        do
            local is_dump=$(echo $db2 | grep $db1 | wc -l)
            if [  $is_dump == 1 ]
            then
                dump_db $db2
            fi
        done
    done
    echo "[`date '+%Y-%m-%d %H:%M:%S'`] 结束备份！" | tee -a $monitor_log
}

#导入
function source_db()
{
    local dump_db=$1
    local dump_db_tmp=$dump_db"_tmp"
    local source_db=$2
    
    if [ ! -f "$dump_db.sql.gz" ];then
        echo "$1 不存在！"
        exit;
    fi
    
    cp $dump_db.sql.gz $dump_db_tmp.sql.gz
    gzip -d $dump_db_tmp.sql.gz
    mysql -uroot -pSanguo1! -e "SET NAMES utf8; CREATE DATABASE IF NOT EXISTS $source_db DEFAULT CHARSET utf8 COLLATE utf8_general_ci"
    mysql -uroot -pSanguo1! $source_db < $dump_db_tmp.sql --default-character-set=utf8
    rm -rf $dump_db_tmp
}
function do_source_db()
{
    if [ ! -f "$1.sql.gz" ];then
        echo "$1 不存在！"
        exit;
    fi
    
    while [ true ]
    do
        echo -e "\033[1;36m确定服务器是否已经关闭\033[0m(Yes/n)? \c"
        read answer
        if [ "$answer" = "Yes" ];then
            
            #do_dump_db
            
            source_db $1 $2
            
            break
        elif [ "$answer" = "n" ];then
            break
        fi
    done
            
}

####################################################################
if [ "$1" == "source" ]; then
    do_source_db $2 $3
    
else
    if [ $dump_time -gt 0 ];then
        while [ true ]
        do
            do_dump_db
            sleep $dump_time
        done
        
    else
        do_dump_db
    fi
fi