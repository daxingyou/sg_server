#!/bin/sh

# sh r_mv_server_logs.sh

#--sg_server
#----logs
#------core_log(*)
#------asan_log(*)
#------dba_log(*)
#------trunk
#--------game_server_1
#----------logs(*)
#------branches
#--------20180525
#----------game_server_1
#------------logs(*)

server_list="account glog area transfer cross db log gm game center chat gate login robot sql"

dst_ip="139.196.41.108"
dst_user="root"
dst_pwd="rootADMIN123"
dst_path="/igame/backup"

####################################################################################
declare -A main_path_list=()
declare -A backup_path_list=()

####################################################################################
function set_path()
{
    local user_list=`cat /etc/passwd | awk -F: '$3>=500' | cut -f 1 -d :`
    for user in $user_list
    do
        local main_path="/home/$user/sg_server"
        if [ ! -d $main_path ];then
            continue
        fi

        local backup_main_path="$dst_path/$user/sg_server"
        sshpass -p $dst_pwd ssh -o GSSAPIAuthentication=no $dst_user@$dst_ip "if [ ! -d $backup_main_path ];then mkdir -p $backup_main_path; fi"
        
        main_path_list[$user]=$main_path
        backup_path_list[$user]=$backup_main_path 
    done   
}

function mv_logs()
{
    local main_path=$1
    local backup_path=$2
    
    local log_path=$main_path/logs
    if [ ! -d $log_path ];then
        echo "$log_path 不存在！"
        return
    fi
    
    #################################################################################
    # /home/xxx/sg_server/trunk/xxx_server/logs/
    for path in $main_path/trunk/*
    do
        mv_server_log $path $log_path
    done
    
    # /home/xxx/sg_server/branches/20180525/xxx_server/logs/
    for path1 in $main_path/branches/*
    do
        for path2 in $path1/*
        do
            mv_server_log $path2 $log_path
        done
    done
    
    # /home/xxx/sg_server/.../dba_log
    local dba_log_path=`find $main_path -name dba_log -type d`
    if [ "$dba_log_path" != "" ];then
        mv $dba_log_path $log_path
    fi
    
    # /home/xxx/sg_server/.../log_level
    local log_level_path=`find $main_path -name log_level -type d`
    if [ "$log_level_path" != "" ];then
        mv $log_level_path $log_path
    fi
    #################################################################################
    
    #迁移
    sshpass -p $dst_pwd scp -r -C -o GSSAPIAuthentication=no $log_path $dst_user@$dst_ip:$backup_path
    
    #删除
    rm -rf $log_path/*
}

function mv_server_log()
{
    local path=$1
    local log_path=$2
    
    local server_name=""
    local server_log_path_1=""
    for s in $server_list
    do
        if [[ "$path" =~ $s ]];then                  
            server_log_path_1="$path/logs"
            if [ ! -d $server_log_path_1 ];then
                break
            fi
            if [ "`ls -A $server_log_path_1`" = "" ]; then
                break
            fi
            server_name_path=${server_log_path_1##*${main_path}}
            if [ "$server_name_path" != "" ];then
                for log_file in $server_log_path_1/*
                do
                    if [[ "$log_file" =~ "`date '+%Y%m%d'`" ]];then
                        continue
                    fi 
                    local server_log_path_2="$log_path$server_name_path"
                    if [ ! -d $server_log_path_2 ];then
                        mkdir -p $server_log_path_2
                    fi
                    mv $log_file $server_log_path_2
                done
            fi
            break
        fi
    done
}

#################################################################################### 
mv_log="$(cd `dirname $0`; pwd)/mv.log" 
home_size=`du -skx /home`
igame_size=`du -skx /igame`
echo "[`date '+%Y-%m-%d %H:%M:%S'`] [$home_size] => [$igame_size]" >> $mv_log

set_path

for user in ${!main_path_list[@]}
do
    main_path=${main_path_list[$user]}
    backup_path=${backup_path_list[$user]}

    mv_logs $main_path $backup_path

done

home_size=`du -skx /home`
igame_size=`du -skx /igame`
echo "[`date '+%Y-%m-%d %H:%M:%S'`] [$home_size] => [$igame_size]" >> $mv_log
echo "=================================================="