#!/bin/sh
#!/usr/bin/python

. ~/sg_server/sh/config.sh 

function show_run_path()
{
    echo -e "\033[32mPath: $work_path \033[0m"
}

##############################################################################################################
function start_run()
{
    if [ "$1" = "" ];then
        run_group_servers

    elif [ "$1" = "global" ];then
        run_global_servers
        
    elif [ "$1" = "gg" ];then
        run_global_servers
        run_group_servers
    
    elif [ "$1" = "account" ];then
        run_account_server
    
    elif [ "$1" = "alls" ];then
        run_account_server
        run_global_servers
        run_group_servers
    
    elif [ "$1" = "sm" ];then
        run_server_monitor
    
    elif [ "$1" = "cl" ];then
        run_send_combat_logs
        
    #elif [ "$1" = "rm" ];then
    #    run_robot_monitor 

    #elif [ "$1" = "pm" ];then
    #    run_performance_monitor
    
    else
        OLD_IFS="$IFS" 
        IFS="," 
        input_server_list=($1)
        IFS="$OLD_IFS" 
        for s in ${input_server_list[@]} 
        do 
            if echo "${run_group_server_list[@]}" | grep -w "$s" &>/dev/null; then
                if [ "$2" != "" ];then
                    if [ $2 -ge 1 ];then
                        run_server $s"_server"_$2
                    fi
                else
                    run_server $s"_server"
                fi
                
            elif echo "${all_global_server_list[@]}" | grep -w "$s" &>/dev/null; then
                if [ "$2" != "" ];then
                    if [ $2 -ge 1 ];then
                        run_server $s"_server"_$2
                    fi
                else
                    run_server $s"_server"
                fi

            else
                echo -e "\033[1;31mparam[$1] error!!!\033[0m"
                exit
            fi
        done
    fi
}
function start_stop()
{
    stop_server_monitor

    if [ "$1" = "" ];then
        stop_robot_monitor
        stop_performance_monitor
        stop_group_servers

    elif [ "$1" = "global" ];then
        stop_global_servers
        
    elif [ "$1" = "gg" ];then
        stop_robot_monitor
        stop_performance_monitor
        stop_send_combat_logs
        stop_global_servers
        stop_group_servers
    
    elif [ "$1" = "account" ];then
        stop_account_server
        
    elif [ "$1" = "alls" ];then
        stop_robot_monitor
        stop_performance_monitor
        stop_send_combat_logs
        stop_account_server
        stop_global_servers
        stop_group_servers
    
    elif [ "$1" = "sm" ];then
        stop_server_monitor
        
    elif [ "$1" = "cl" ];then
        stop_send_combat_logs

    #elif [ "$1" = "rm" ];then
    #    stop_robot_monitor

    #elif [ "$1" = "pm" ];then
    #    stop_performance_monitor
  
    else
        OLD_IFS="$IFS" 
        IFS="," 
        input_server_list=($1)
        IFS="$OLD_IFS" 
        for s in ${input_server_list[@]} 
        do 
            if echo "${stop_group_server_list[@]}" | grep -w "$s" &>/dev/null; then                
                if [ "$2" != "" ];then
                    if [ $2 -ge 1 ];then
                        stop_server $s"_server"_$2
                    fi
                else
                    stop_server $s"_server"
                fi
                
            elif echo "${all_global_server_list[@]}" | grep -w "$s" &>/dev/null; then
                if [ "$2" != "" ];then
                    if [ $2 -ge 1 ];then
                        stop_server $s"_server"_$2
                    fi
                else
                    stop_server $s"_server"
                fi

            else
                echo -e "\033[1;31mparam[$1] error!!!\033[0m"
                exit
            fi
        done
    fi
}
function start_check()
{
	check_server_monitor
	check_send_combat_logs
	#check_robot_monitor
	#check_performance_monitor
    
    check_account_server
    check_global_servers
    check_group_servers
}

##############################################################################################################
function is_server_running()
{
    local run_num=0
    for s in $1
    do
        local has=0
        for i_s in $stop_ignore_server_list
        do
            if [ "$s" = "$i_s" ];then
                has=1
                break
            fi
        done
        if [ $has -eq 1 ];then
            continue
        fi
        
        srv_name=$s"_server"
        check_server './'$srv_name
        if [ $? -eq 1 ];then
            let run_num++
        fi
    done
    
    if [ $run_num -ge 1 ];then
        return 1 #有服务器在运�?
    else
        return 0 #所有服务器都已关闭
    fi
}
function run_multiple_server()
{
    local s=$1
    local run_number=$2
    for ((i=1; i<=$run_number; i++))  
    do  
        srv_name=$s"_server"_$i
        if [ ! -d $work_path/$srv_name/logs ]; then
            mkdir -p  $work_path/$srv_name/logs
        fi
        cp $work_path/$s"_server"/$s"_server" $work_path/$srv_name/$srv_name
        cp $work_path/$s"_server"/config.xml $work_path/$srv_name/config.xml
        sed -i "s#<id>1</id>#<id>$i</id>#g" $work_path/$srv_name/config.xml
        run_server $srv_name
    done
}
function run_server()
{
    echo "=============================="
    check_server './'$1
    if [ $? -eq 1 ];then
        echo -e "\033[31m$1 is running......\033[0m"
        return
    fi

    echo "running $1..."
    cd $work_path/$1
    local server=$1
    local srv=${server%%_*}
    if [ $memcheck -eq 0 ];then
        nohup ./$1 1>/dev/null 2>./logs/$srv`date '+%Y%m%d'`.asan &
    elif [ $memcheck -eq 1 ];then
        if [ ! -d $valgrind_log_dir ];then
            mkdir $valgrind_log_dir
        fi
        nohup valgrind --time-stamp=yes $mck_param --log-file=$valgrind_log_dir/$srv`date '+%Y%m%d%H%M%S'`.valgrind ./$1 >/dev/null 2>&1 &
    fi
    cd $main_path/nohup_run
    sleep 1
   
    check_server './'$1
    if [ $? -eq 1 ];then
        echo "success!"
    else
        echo -e "\033[31mfailed!\033[0m"
    fi
    
    #game或者cross启动�?运行发送战斗回放脚�?
    #if [[ "$1" =~ "game_server" ]];then
    #    run_send_combat_logs
    #elif [[ "$1" =~ "cross_server" ]];then
    #    run_send_combat_logs
    #fi
}

function run_group_servers()
{
    is_server_running $run_group_server_list
    if [ $? -eq 1 ];then
        echo -e "\033[1;37;31m错误：请关闭正在运行的服务器，再启动！\033[0m"
        return
    fi

    for s in $run_group_server_list
    do      
        if [ "$s" = "gate" -a $gate_number -gt 1 ];then
            run_multiple_server $s $gate_number
        elif [ "$s" = "game" -a $game_number -gt 1 ];then
            run_multiple_server $s $game_number
        else
            srv_name=$s"_server"
            run_server $srv_name
        fi
    done
}
function run_global_servers()
{
    is_server_running $run_global_server_list
    if [ $? -eq 1 ];then
        echo -e "\033[1;37;31m错误：请关闭正在运行的服务器，再启动！\033[0m"
        return
    fi

    for s in $run_global_server_list
    do      
        if [ "$s" = "cross" -a $cross_number -gt 1 ];then
            run_multiple_server $s $cross_number
        elif [ "$s" = "transfer" -a $transfer_number -gt 1 ];then
            run_multiple_server $s $transfer_number
        else
            srv_name=$s"_server"
            run_server $srv_name
        fi
    done
}
function run_account_server()
{
    local account_list="account glog"
    is_server_running $account_list
    if [ $? -eq 1 ];then
        echo -e "\033[1;37;31m错误：请关闭正在运行的服务器，再启动！\033[0m"
        return
    fi

    for s in $account_list
    do      
        if [ "$s" = "account" -a $account_number -gt 1 ];then
            run_multiple_server $s $account_number
        else
            srv_name=$s"_server"
            run_server $srv_name
        fi
    done
}
##############################################################################################################
function stop_multiple_server()
{
    local s=$1
    local run_number=$2
    for ((i=1; i<=$run_number; i++))  
    do  
        srv_name=$s"_server"_$i
        stop_server $srv_name
    done
}
function stop_server()
{
	if [[ "$1" =~ "db" ]];then
        check_server './game_server_'
        if [ $? -eq 1 ];then
            echo -e "\033[1;37;31m请先关闭game_server!\033[0m"
            exit
        fi
    fi

    echo "=============================="
    echo "stop $1..."
    
    ps aux | grep -w $ps_name | grep -w "./"$1 | grep -v 'grep' | awk '{print $2}' | xargs kill -s 15

    check_server './'$1
    res=$?
    while [[ $res != 0 ]]
    do
        check_server './'$1
        res=$?
    done
    echo "success!"

    #删除大小�?的asan日志
    find $work_path/$1/logs -size 0c -name '*.asan' | xargs rm -f
    
    #关闭发送战斗回放脚�?
    if [[ "$1" =~ "game_serve" ]];then
        local cross_res=`ps aux | grep -w $ps_name | grep "cross_server" | grep -v "grep" | wc -l`
        if [ $cross_res -eq 0 ];then
            stop_send_combat_logs
        fi
    elif [[ "$1" =~ "cross_server" ]];then
        local game_res=`ps aux | grep -w $ps_name | grep "game_server" | grep -v "grep" | wc -l`
        if [ $game_res -eq 0 ];then
            stop_send_combat_logs
        fi
    fi
}

function stop_group_servers()
{
    for s in $stop_group_server_list
    do
        local has=0
        for i_s in $stop_ignore_server_list
        do
            if [ "$s" = "$i_s" ];then
                has=1
                break
            fi
        done
        if [ $has -eq 1 ];then
            continue
        fi
        
        if [ "$s" = "gate" -a $gate_number -gt 1 ];then
            stop_multiple_server $s $gate_number
        elif [ "$s" = "game" -a $game_number -gt 1 ];then
            stop_multiple_server $s $game_number
        else
            srv_name=$s"_server"
            stop_server $srv_name
        fi
    done
}
function stop_global_servers()
{
    for s in $stop_global_server_list
    do
        local has=0
        for i_s in $stop_ignore_server_list
        do
            if [ "$s" = "$i_s" ];then
                has=1
                break
            fi
        done
        if [ $has -eq 1 ];then
            continue
        fi
        
        if [ "$s" = "cross" -a $cross_number -gt 1 ];then
            stop_multiple_server $s $cross_number
        elif [ "$s" = "transfer" -a $transfer_number -gt 1 ];then
            stop_multiple_server $s $transfer_number
        else
            srv_name=$s"_server"
            stop_server $srv_name
        fi
    done
}
function stop_account_server()
{
    local account_list="account glog"
    for s in $account_list
    do
        #local has=0
        #for i_s in $stop_ignore_server_list
        #do
        #    if [ "$s" = "$i_s" ];then
        #        has=1
        #        break
        #    fi
        #done
        #if [ $has -eq 1 ];then
        #    continue
        #fi
            
        if [ "$s" = "account" -a $account_number -gt 1 ];then
            stop_multiple_server $s $account_number
        else
            srv_name=$s"_server"
            stop_server $srv_name
        fi
    done
}

##############################################################################################################
function check_server()
{
#echo 'ps aux | grep -w '${ps_name}' | grep -w '${1}' | grep -v "grep" | wc -l' 
	local rlt=`ps aux | grep -w $ps_name | grep -w $1 | grep -v "grep" | wc -l `
    if [ $rlt -eq 0 ];then
        return 0
    elif [ $rlt -ge 1 ];then
        return 1
    fi
}

function check_account_server()
{
    local account_list="account glog"
    for s in $account_list
    do
        echo "=============================="
        if [ "$s" = "account" -a $account_number -gt 1 ];then
            for ((i=1; i<=$account_number; i++))  
            do
                srv_name=$s"_server"_$i
                check_server './'$srv_name
                if [ $? -eq 1 ];then
                    echo $s"_server"_$i" is running......"
                else
                    echo -e "\033[31m"$s"_server"_$i" has been stopped!\033[0m"
                fi
                check_core $srv_name
            done
            
        else
            srv_name=$s"_server"
            check_server './'$srv_name
            if [ $? -eq 1 ];then
                echo $s"_server is running......"
            else
                echo -e "\033[31m"$s"_server has been stopped!\033[0m"
            fi
            check_core $srv_name
        fi
    done

    backup_server
}
function check_global_servers()
{
    for s in $run_global_server_list
    do
        echo "=============================="
        if [ "$s" = "cross" -a $cross_number -gt 1 ];then
            for ((i=1; i<=$cross_number; i++))  
            do
                srv_name=$s"_server"_$i
                check_server './'$srv_name
                if [ $? -eq 1 ];then
                    echo $s"_server"_$i" is running......"
                else
                    echo -e "\033[31m"$s"_server"_$i" has been stopped!\033[0m"
                fi
                check_core $srv_name
            done
            
        elif [ "$s" = "transfer" -a $transfer_number -gt 1 ];then
            for ((i=1; i<=$transfer_number; i++))  
            do
                srv_name=$s"_server"_$i
                check_server './'$srv_name
                if [ $? -eq 1 ];then
                    echo $s"_server"_$i" is running......"
                else
                    echo -e "\033[31m"$s"_server"_$i" has been stopped!\033[0m"
                fi
                check_core $srv_name
            done
            
        else
            srv_name=$s"_server"
            check_server './'$srv_name
            if [ $? -eq 1 ];then
                echo $s"_server is running......"
            else
                echo -e "\033[31m"$s"_server has been stopped!\033[0m"
            fi
            check_core $srv_name
        fi
    done

    backup_server
}
function check_group_servers()
{
    for s in $run_group_server_list
    do
        echo "=============================="
        if [ "$s" = "gate" -a $gate_number -gt 1 ];then
            for ((i=1; i<=$gate_number; i++))  
            do
                srv_name=$s"_server"_$i
                check_server './'$srv_name
                if [ $? -eq 1 ];then
                    echo $s"_server"_$i" is running......"
                else
                    echo -e "\033[31m"$s"_server"_$i" has been stopped!\033[0m"
                fi
                check_core $srv_name
            done
            
        elif [ "$s" = "game" -a $game_number -gt 1 ];then
            for ((i=1; i<=$game_number; i++))  
            do
                srv_name=$s"_server"_$i
                check_server './'$srv_name
                if [ $? -eq 1 ];then
                    echo $s"_server"_$i" is running......"
                else
                    echo -e "\033[31m"$s"_server"_$i" has been stopped!\033[0m"
                fi
                check_core $srv_name
            done
        
        else
            srv_name=$s"_server"
            check_server './'$srv_name
            if [ $? -eq 1 ];then
                echo $s"_server is running......"
            else
                echo -e "\033[31m"$s"_server has been stopped!\033[0m"
            fi
            check_core $srv_name
        fi
    done

    backup_server
}

###############################################################################################
function check_core()
{
    local server=$1

    if [ ! -d $work_path/$server/logs ]; then
        return
    fi
    
    local asan=$(find $work_path/$server/logs -size +0c -name '*.asan')
    if [ "$asan" != "" ];then
        local stop=0
        #如果遇到assert不会关闭服务器，那就强制关闭
        if [ `cat $asan | grep -c "Assertion"` -ge 1 ]; then
            stop=1
        elif [ `cat $asan | grep -c "terminate"` -ge 1 ]; then
            stop=1
        #elif [ `cat $asan | grep -c "libprotobuf"` -ge 1 ]; then
        #    stop=1
        fi
        if [ $stop -eq 1 ];then
            check_server './'$1
            if [ $? -eq 1 ];then
                ps aux | grep -w $ps_name | grep -w "./"$1 | grep -v 'grep' | awk '{print $2}' | xargs kill -9 &>/dev/null
                check_server './'$1
                if [ $? -ne 1 ];then
                    echo -e "\033[31m$1 is forced to be stopped!\033[0m"
                fi
            fi
        fi
        asan_list[$server]=$asan
        #rlt_asan=$(ls -lh $asan)
        #echo -e "\033[1;37;31m$rlt_asan\033[0m"
    fi

    local core=$(find $work_path/$server -name 'core.*')
    if [ "$core" != "" ];then
        core_list[$server]=$core
        rlt_core=$(ls -lh $core)
        echo -e "\033[1;37;31m$rlt_core\033[0m"
    fi
}
function backup_server()
{
    #asan
    local asan_list_len=${#asan_list[@]}
    if [ $asan_list_len -gt 0 ];then
        if [ ! -d $asan_log_dir ];then
            mkdir $asan_log_dir
        fi
        echo "=============================="
        echo -e "\033[1;37;31m服务器出错，停止一切更新！！！\033[0m"
        #for asan_log in ${asan_list[@]}
        for server in ${!asan_list[@]}
        do
            local asan_log_1=${asan_list[$server]}
            local log_modify_time=$(stat $asan_log_1 | grep 'Modify')
            log_modify_time=${log_modify_time##*: }
            echo $log_modify_time >> $asan_log_1
            
            local asan_log_2=$server"_"`date '+%Y%m%d%H%M%S'`.asan
            
            mv $asan_log_1 $asan_log_dir/$asan_log_2
            rm -f $asan_log_1
            echo -e "\033[1;36m$asan_log_dir/$asan_log_2\033[0m"
        done
        unset asan_list
    fi

    #core
    local core_list_len=${#core_list[@]}
    if [ $core_list_len -gt 0 ]; then  
        if [ ! -d $core_log_dir ];then
            mkdir $core_log_dir
        fi
        echo "=============================="
        echo -e "\033[1;37;31m服务器出错，停止一切更新！！！\033[0m"
        for server in ${!core_list[@]}
        do
            local log=${core_list[$server]}
            local core_name=${log##*/}
            local core_path=$core_log_dir/$server"_"`date '+%Y%m%d%H%M%S'`"_"$core_name
            mv $log $core_path
            echo -e "\033[1;36m$core_path\033[0m"
        done
        unset core_list
    fi
}

function copy_ssh_cmd()
{
    if [ "$1" = "" ];then
        return
    fi
    sshpass -p $copy_dst_user_passwd ssh -o GSSAPIAuthentication=no $copy_dst_user_name@$copy_dst_ip $1
}

function copy_scp()
{
    if [ "$copy_src_ip" = "$copy_dst_ip" ]; then
        if [ "$1" = "-f" ];then
            if [ "$4" != "" ];then
                cp $2/*.$4 $3
            else
                cp $2 $3
            fi 
        elif [ "$1" = "-r" ];then
            if [ "$4" != "" ];then
                cp -r $2/*.$4 $3
            else
                cp -r $2 $3
            fi 
        fi
    else
        if [ "$1" = "-f" ];then
            if [ "$4" != "" ];then
                sshpass -p $copy_dst_user_passwd scp -C -o GSSAPIAuthentication=no $2/*.$4 $copy_dst_user_name@$copy_dst_ip:$3
            else
                sshpass -p $copy_dst_user_passwd scp -C -o GSSAPIAuthentication=no $2 $copy_dst_user_name@$copy_dst_ip:$3
            fi    
        elif [ "$1" = "-r" ];then
            if [ "$4" != "" ];then
                sshpass -p $copy_dst_user_passwd scp -r -C -o GSSAPIAuthentication=no $2/*.$4 $copy_dst_user_name@$copy_dst_ip:$3
            else
                sshpass -p $copy_dst_user_passwd scp -r -C -o GSSAPIAuthentication=no $2 $copy_dst_user_name@$copy_dst_ip:$3
            fi  
        fi
    fi
    #echo -e "\033[1;35m完成拷贝:\033[0m \033[1;37m$user_name@$copy_src_ip:$2 --> $copy_dst_user_name@$copy_dst_ip:$3\033[0m"
}

function md5()
{
    cd $1
    for filename in *; 
    do 
        echo "$filename: `md5sum $filename`" >>$1.md5; 
    done
}

##################################################################################################################################
function copy_gen_src_md5()
{
    md5_file_path=$tmp/server.md5
    
    rm -f $md5_file_path

    ##############################################################################################################
    find $copy_src_main_path/nohup_run/common.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/nohup_run/run.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/nohup_run/stop.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/nohup_run/check.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/nohup_run/log.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/nohup_run/multi_log.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/nohup_run/robot_monitor.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/nohup_run/server_monitor.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/nohup_run/auto_up.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    
    find $copy_src_main_path/sh/config.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/sh/get_xml_value.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/sh/set_xml_value.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/sh/r_db_statistica_log.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/sh/r_db_backup.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/sh/r_gen_iptables.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_main_path/sh/r_check_env.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    
    find $copy_src_work_path/sql/config_new.py -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/sql/database.py -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/sql/sg_account.sql -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/sql/sg_area.sql -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/sql/sg_glog.sql -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/sql/sg_game.sql -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/sql/sg_log.sql -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/sql/sg_gm.sql -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/sql/sg_bbs.sql -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/sql/sg_up.sql -type f -print0 | xargs -0 md5sum >> $md5_file_path
    #find $copy_src_work_path/sql/sg_statistics.sql -type f -print0 | xargs -0 md5sum >> $md5_file_path
    
    find $copy_src_work_path/config/readtbl -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/config/map \( -name "*.xml" -o -name "*.map" \) -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/config/script \( -name "*.lua" \) -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/config/tbl \( -name "*.tbls64" -o -name "*.bytes" \) -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/config/ai \( -name "*.xml" \) -type f -print0 | xargs -0 md5sum >> $md5_file_path

    find $copy_src_work_path/update.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/clear.sh -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/channel.xml -type f -print0 | xargs -0 md5sum >> $md5_file_path

    ##############################################################################################################
    #全局服务�?
    find $copy_src_work_path/global_server_new.xml -type f -print0 | xargs -0 md5sum >> $md5_file_path
    for s in $all_global_server_list
    do
        server=$s"_server"
        
        if [ "$s" != "cross" ] && [ "$s" != "glog" ];then
            find $copy_src_work_path/$server/$server -type f -print0 | xargs -0 md5sum >> $md5_file_path
        fi
        
        if [ "$s" = "account" ] || [ "$s" = "transfer" ] || [ "$s" = "cross" ] || [ "$s" = "glog" ];then
            find $copy_src_work_path/$server/config.xml -type f -print0 | xargs -0 md5sum >> $md5_file_path
        fi
    done

    ##############################################################################################################
    #单服务器�?
    find $copy_src_work_path/server_new.xml -type f -print0 | xargs -0 md5sum >> $md5_file_path
    for s in $run_group_server_list
    do
        server=$s"_server"
        find $copy_src_work_path/$server/$server -type f -print0 | xargs -0 md5sum >> $md5_file_path
        
        if [ "$s" = "gate" ] || [ "$s" = "game" ] || [ "$s" = "log" ] ;then
            find $copy_src_work_path/$server/config.xml -type f -print0 | xargs -0 md5sum >> $md5_file_path
        fi
    done

    server="robot_server"
    find $copy_src_work_path/$server/$server -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/$server/config.ini -type f -print0 | xargs -0 md5sum >> $md5_file_path
    find $copy_src_work_path/$server/script_config \( -name "*.lua" \) -type f -print0 | xargs -0 md5sum >> $md5_file_path
}

function copy_gen_dst_md5()
{
    copy_ssh_cmd "if [ ! -d $copy_dst_main_path/sh ];then mkdir -p $copy_dst_main_path/sh; fi"
    copy_ssh_cmd "if [ ! -d $copy_dst_main_path/nohup_run ];then mkdir -p $copy_dst_main_path/nohup_run; fi"
    copy_ssh_cmd "if [ ! -d $copy_dst_work_path ];then mkdir -p $copy_dst_work_path; fi"
    
    copy_ssh_cmd "find $copy_dst_main_path/sh/ \( -name '*.sh' \) -type f -print0 | xargs -0 md5sum >> $copy_dst_main_path/server.md5" 
    copy_ssh_cmd "find $copy_dst_main_path/nohup_run/ \( -name '*.sh' \) -type f -print0 | xargs -0 md5sum >> $copy_dst_main_path/server.md5"
    copy_ssh_cmd "find $copy_dst_work_path -type f -print0 | xargs -0 md5sum >> $copy_dst_main_path/server.md5"
    
    sshpass -p $copy_dst_user_passwd scp -C -o GSSAPIAuthentication=no $copy_dst_user_name@$copy_dst_ip:$copy_dst_main_path/server.md5 $tmp/server$copy_dst_ip.md5 1>/dev/null
    copy_ssh_cmd "rm -f $copy_dst_main_path/server.md5"
}

function copy_check_md5()
{
    echo -e "\033[1;35m比较MD5\033[0m"
    
    local start_time=`date '+%s'`
    
    copy_gen_src_md5
    copy_gen_dst_md5

    while read -r row1
    do
        file_1=${row1##*${copy_src_main_path}}
        md5_1=${row1%%  *}
        state="none"

        while read -r row2
        do
            file_2=${row2##*${copy_dst_main_path}}
            md5_2=${row2%%  *}

            if [ "$file_1" = "$file_2" ];then
                if [ "$md5_1" = "$md5_2" ];then
                    state="same"
                else
                    state="U"
                fi
                break
            fi
        done < $tmp/server$copy_dst_ip.md5

        if [ "$state" = "none" ];then
            state="A"
        fi

        if [ "$state" = "U" -o "$state" = "A" ];then
            diff_md5_files+=([$file_1]=$state)
        fi
    done < $tmp/server.md5

    while read -r row3
    do
        file_3=${row3##*${copy_dst_main_path}}

        copy_filter $file_3
        if [ $? -ne 0 ];then
            continue
        fi
        
        state="D"

        while read -r row4
        do
            file_4=${row4##*${copy_src_main_path}}

            if [ "$file_3" = "$file_4" ];then
                state="none"
                break
            fi
        done < $tmp/server.md5

        if [ "$state" = "D" ];then
            diff_md5_files+=([$file_3]=$state)
        fi
    done < $tmp/server$copy_dst_ip.md5
    
    local end_time=`date '+%s'`
    ((diff_time=$end_time-$start_time))
    ((hour=$diff_time/3600))
    ((min=$diff_time/60%60))
    ((sec=$diff_time%60))
    echo -e "\033[36m用时: $hour时$min分$sec秒\033[0m"
}

function copy_filter()
{
    if [ "$1" = "/server.md5" ];then
        return 1
    fi
    if [ "$1" = "/sh/my_config.sh" ];then
        return 1
    fi

    if [[ "$1" =~ "logs" ]];then
        return 1
    fi
    if [[ "$1" =~ ".asan" ]];then
        return 1
    fi
    if [[ "$1" =~ "global_server.xml" ]];then
        return 1
    fi
    if [[ "$1" =~ "server.xml" ]];then
        return 1
    fi
    if [[ "$1" =~ "/sql/config.py" ]];then
        return 1
    fi
    if [[ "$1" =~ "/sql/config.pyc" ]];then
        return 1
    fi    
    if [[ "$1" =~ "/nohup_run/performance_monitor.py" ]];then
        return 1
    fi
    if [[ "$1" =~ "/robot_server/config.ini" ]];then
        return 1
    fi
    if [[ "$1" =~ "_server_" ]];then
        return 1
    fi
    if [[ "$1" =~ "db_save" ]];then
        return 1
    fi
    if [[ "$1" =~ "version_no_file" ]];then
        return 1
    fi
    if [[ "$1" =~ "update.log" ]];then
        return 1
    fi
    if [[ "$1" =~ "old.sql" ]];then
        return 1
    fi
    if [[ "$1" =~ "sql/sg_statistics.sql" ]];then
        return 1
    fi
    if [[ "$1" =~ "cross_server" ]];then
        return 1
    fi
    if [[ "$1" =~ "glog_server" ]];then
        return 1
    fi
    if [[ "$1" =~ "ss_log" ]];then
        return 1
    fi
    return 0
}

function copy_set_tips()
{
    local tips_file=$1
    
    if [ "$tips_file" = "server_new.xml" -o "$tips_file" = "global_server_new.xml" ];then
        do_xml=1

    elif [ "$tips_file" = "sg_game.sql" -o "$tips_file" = "sg_log.sql" ];then
        do_game_log_sql=1
        
    elif [ "$tips_file" = "sg_account.sql" ];then
        do_account_sql=1

    elif [ "$tips_file" = "sg_area.sql" ];then
        do_area_sql=1
        
    elif [ "$tips_file" = "sg_glog.sql" ];then
        do_glog_sql=1
        
    elif [ "$tips_file" = "game_server" ];then
        do_cross=1
     
    elif [ "$tips_file" = "log_server" ];then
        do_glog=1
        
    fi
}
function copy_show_tips()
{
    for tips in "${tips_list[@]}"
    do
       echo -e "\033[1;36m$tips\033[0m"
    done
}

function copy_mkdir_server_logs()
{
    for s in $all_global_server_list
    do
        server=$s"_server"
        dst_dir=$copy_dst_work_path/$server

        if [ "$copy_src_ip" = "$copy_dst_ip" ]; then
            if [ ! -d $$dst_dir/logs ];then mkdir -p $dst_dir/logs; fi
        else
            copy_ssh_cmd "if [ ! -d $dst_dir/logs ];then mkdir -p $dst_dir/logs; fi"
        fi

    done

    for s in $run_group_server_list
    do
        server=$s"_server"
        dst_dir=$copy_dst_work_path/$server

        if [ "$copy_src_ip" = "$copy_dst_ip" ]; then
            if [ ! -d $$dst_dir/logs ];then mkdir -p $dst_dir/logs; fi
        else
            copy_ssh_cmd "if [ ! -d $dst_dir/logs ];then mkdir -p $dst_dir/logs; fi"
        fi

    done
}

function copy_compare()
{
    local no_diff=0
    
    declare -A diff_md5_files=()
    copy_check_md5 $diff_md5_files

    declare -a tips_list=()

    if [[ ${#diff_md5_files[*]} > 0 ]];then
        echo -e "\033[1;35m拷贝开始\033[0m"
        for file in ${!diff_md5_files[@]}
        do
            state=${diff_md5_files[$file]}
            echo "[$state]$file"

            file_1=${file%/*}
            if [ ! -d $tmp/$file_1 ];then 
                mkdir -p $tmp/$file_1; 
            fi

            file_2=${file##*/}
            copy_set_tips $file_2

            if [ "$state" = "A" -o "$state" = "U" ];then
                cp $copy_src_main_path$file $tmp$file_1
            elif [ "$state" = "D" ];then
                copy_ssh_cmd "rm -f $copy_dst_main_path$file"
            fi
        done

        local start_time=`date '+%s'`
        if [ -d $tmp/$branches_path ];then
            if [ "$version_name" = "trunk" ];then
                copy_scp -r $tmp/$branches_path $copy_dst_main_path #1>/dev/null
            else
                copy_scp -r $tmp/$branches_path $copy_dst_main_path/branches #1>/dev/null
            fi
        fi
        local end_time=`date '+%s'`
        ((diff_time=$end_time-$start_time))
        ((hour=$diff_time/3600))
        ((min=$diff_time/60%60))
        ((sec=$diff_time%60))
        echo -e "\033[36m用时: $hour时$min分$sec秒\033[0m"
    
        if [ -d $tmp/sh ];then
            copy_scp -r $tmp/sh $copy_dst_main_path #1>/dev/null
        fi
        
        if [ -d $tmp/nohup_run ];then
            copy_scp -r $tmp/nohup_run $copy_dst_main_path #1>/dev/null
        fi

        copy_mkdir_server_logs

        copy_show_tips

    else
        echo -e "\033[1;35m无差异\033[0m"
        no_diff=1
    fi

    cp $copy_src_main_path/sh/$copy_dst_ip@$copy_dst_user_name.sh $tmp"_my_config.sh"
    sed -i "s/^\\(version_name=\\).*/\\1\"$version_name\"/" $tmp"_my_config.sh"
    copy_scp -f $tmp"_my_config.sh"  $copy_dst_main_path/sh/my_config.sh
    rm -f $tmp"_my_config.sh"
    
    copy_ssh_cmd "if [ -f $copy_dst_main_path/sh/r_db_statistica_log.sh ];then chmod +x $copy_dst_main_path/sh/r_db_statistica_log.sh; fi"
    copy_ssh_cmd "if [ -f $copy_dst_main_path/sh/r_db_backup.sh ];then chmod +x $copy_dst_main_path/sh/r_db_backup.sh; fi"
    
    return $no_diff
}

###########################################################################################################
function copy_all_servers()
{    
    do_cross=0
    do_game_log_sql=0
    do_account_sql=0
    do_area_sql=0
    do_glog_sql=0
    do_xml=0
    do_glog=0
    
    local start_time=`date '+%s'`

    #关闭对端运行着的服务器
    #copy_ssh_cmd "sh $copy_dst_main_path/nohup_run/stop.sh"

    #检查对端服务器是否在运行，运行的话需要关闭才能进行下一�?
    
    if [ -d $tmp ];then
        rm -rf $tmp
    fi
    mkdir -p $tmp
    
    copy_compare
    
    #if [ $? -ne 1 ];then
    #    #再检查一遍MD5
    #    declare -A diff_md5_files=()
    #    copy_check_md5 $diff_md5_files
    #fi
    
    local end_time=`date '+%s'`
    ((diff_time=$end_time-$start_time))
    ((hour=$diff_time/3600))
    ((min=$diff_time/60%60))
    ((sec=$diff_time%60))
    
    if [[ ${#diff_md5_files[*]} = 0 ]];then
        echo -e "\033[1;35m拷贝完成\033[0m"
    else
        for file_key in ${!diff_md5_files[@]}
        do
            state=${diff_md5_files[$file_key]}
            echo "[$state]$file_key"
        done
        echo -e "\033[1;35m拷贝失败\033[0m"
    fi
    rm -rf $tmp
    echo -e "\033[1;36m用时: $hour时$min分$sec秒\033[0m"
    
    copy_ssh_cmd "sh $copy_dst_work_path/update.sh xml"
    copy_ssh_cmd "sh $copy_dst_work_path/update.sh cross"
    copy_ssh_cmd "sh $copy_dst_work_path/update.sh glog"
    if [ $do_game_log_sql -eq 1 ];then
        copy_ssh_cmd "sh $copy_dst_work_path/update.sh sql game"
    fi

    if [ $do_account_sql -eq 1 ] ;then
        echo -e "\033[1;35m请确认是否要执行：update.sh .sh sql account\033[0m"
        #copy_ssh_cmd "sh $copy_dst_work_path/update.sh sql account"
    fi
    if [ $do_glog_sql -eq 1 ];then
        echo -e "\033[1;35m请确认是否要执行：update.sh .sh sql glog\033[0m"
        #copy_ssh_cmd "sh $copy_dst_work_path/update.sh sql glog"
    fi
    if [ $do_area_sql -eq 1 ];then
        echo -e "\033[1;35m请确认是否要执行：update.sh .sh sql area\033[0m"
        #copy_ssh_cmd "sh $copy_dst_work_path/update.sh sql area"
    fi
}

function copy_web()
{
    local web_name=$1
    
    sshpass -p $copy_dst_user_passwd ssh -o GSSAPIAuthentication=no $copy_dst_user_name@$copy_dst_ip "if [ ! -d $copy_dst_main_path ];then mkdir -p $copy_dst_main_path; fi"
    
    sshpass -p $copy_dst_user_passwd ssh -o GSSAPIAuthentication=no $copy_dst_user_name@$copy_dst_ip "if [ -d $copy_dst_main_path/$web_name ];then rm -rf $copy_dst_main_path/$web_name; fi"
    
    sshpass -p $copy_dst_user_passwd scp -r -C -o GSSAPIAuthentication=no $copy_src_main_path/$web_name $copy_dst_user_name@$copy_dst_ip:$copy_dst_main_path
    
    if [ "$web_name" = "sg_update" ] || [ "$web_name" = "sg_sdk" ] || [ "$web_name" = "sg_bbs" ];then
        sshpass -p $copy_dst_user_passwd ssh -o GSSAPIAuthentication=no $copy_dst_user_name@$copy_dst_ip "chmod 777 $copy_dst_main_path/$web_name/log"
        
        if [ "$web_name" = "sg_update" ];then
            sshpass -p $copy_dst_user_passwd ssh -o GSSAPIAuthentication=no $copy_dst_user_name@$copy_dst_ip "chmod 777 $copy_dst_main_path/$web_name/server_info"
        fi
    fi
    
    check_web $web_name
}

function check_web()
{
    local web_name=$1
    local cmd=""
    
    if [ "$web_name" = "sg_gmtool" ];then
        cmd="if [ -f $copy_dst_main_path/$web_name/db_config.php ];then cat $copy_dst_main_path/$web_name/db_config.php | grep '\$sg_gm_conf'; fi"
        cmd="if [ -f $copy_dst_main_path/$web_name/db_config.php ];then cat $copy_dst_main_path/$web_name/db_config.php | grep '\$sg_statistics_conf'; fi"
        cmd="if [ -f $copy_dst_main_path/$web_name/db_config.php ];then cat $copy_dst_main_path/$web_name/db_config.php | grep '\$sg_log'; fi"
        cmd="if [ -f $copy_dst_main_path/$web_name/db_config.php ];then cat $copy_dst_main_path/$web_name/db_config.php | grep '\$sg_up_conf'; fi"
        cmd="if [ -f $copy_dst_main_path/$web_name/db_config.php ];then cat $copy_dst_main_path/$web_name/db_config.php | grep 'var sg_update'; fi"
    
    elif [ "$web_name" = "sg_bbs" ];then
        cmd="cat $copy_dst_main_path/$web_name/db_config.php | grep '\$sg_bbs_conf'"
        echo -e "检查：数据库[sg_up.bbs_info.bbs_url] http://$dst_ip:8080/branches/$version/$web_name/sg_bbs.php"  
    
    elif [ "$web_name" = "sg_cdkey" ];then
        cmd="cat $copy_dst_main_path/$web_name/db_config.php | grep '\$sg_cdk_conf'"
        cmd="cat $copy_dst_work_path/server.xml | grep cdkey_url"
  
    elif [ "$web_name" = "sg_sdk" ];then
        cmd="cat $copy_dst_work_path/global_server.xml | grep sdk_login_url"
        #echo -e "顺带查看对应sg_update的配置是否正�?  
        #cmd="cat $copy_dst_work_path/global_server.xml | grep login_list_url"
        
    elif [ "$web_name" = "sg_update" ];then
        cmd="if [ -f $copy_dst_main_path/$web_name/db_config.php ];then cat $copy_dst_main_path/$web_name/db_config.php | grep '\$sg_up_conf'; fi"
    fi
    
    if [ "$cmd" != "" ];then
        sshpass -p $copy_dst_user_passwd ssh -o GSSAPIAuthentication=no $copy_dst_user_name@$copy_dst_ip $cmd
    fi
}


###########################################################################################################
function copy_check_input_param()
{
    copy_src_ip=$server_ip
    copy_src_main_path=$main_path
    copy_src_work_path=$work_path
        
    copy_dst_ip=$1
    copy_dst_user_name=$2
    copy_dst_user_passwd=$copy_dst_user_name
    copy_dst_main_path="/home/$copy_dst_user_name/sg_server"
    copy_dst_work_path="$copy_dst_main_path/$branches_path"

    copy_file=$3
    
    copy_web=$4
    
    tmp="$(cd `dirname $0`; pwd)/tmp$copy_dst_user_name@$copy_dst_ip"
        
    if [ "$copy_file" = "web" ];then
        if [ "$copy_dst_user_name" != "root" ];then
            echo -e "\033[1;37;31m错误：对端需要root权限的账�? \033[0m"
            exit
        fi

        echo -n "请输�?copy_dst_user_name@$copy_dst_ip的密码："
        read pwd
        copy_dst_user_passwd=$pwd
        if [ "$version_name" = "trunk" ];then
            copy_src_main_path="$copy_src_work_path/web_server"
            copy_dst_main_path="/var/www/html"
        else
            copy_src_main_path="$copy_src_work_path/web_server"
            copy_dst_main_path="/var/www/html/branches/$version_name"
        fi
    fi

    if [ "$copy_dst_ip" = "" ];then
        echo -e "\033[1;37;31m错误：目标机器IP为空! \033[0m"
        exit
    fi
    if [ "$copy_dst_user_name" = "" ];then
        echo -e "\033[1;37;31m错误：目标机器用户名为空! \033[0m"
        exit
    fi
    if [ "$copy_file" = "" ];then
        echo -e "\033[1;37;31m错误：拷贝文件为�? \033[0m"
        exit
    fi
}

function start_copy()
{
    while [ true ]
    do
        echo -e "\033[1;35m请确认拷贝版本[$version_name]的主目录：\033[0m"
        echo -e "\033[1;36m    源头机器目录:\033[0m \033[1;37m$user_name@$copy_src_ip:$copy_src_main_path\033[0m"
        echo -e "\033[1;36m    目标机器目录:\033[0m \033[1;37m$copy_dst_user_name@$copy_dst_ip:$copy_dst_main_path\033[0m"
        echo -e "(Yes/n) \c"
        read answer
        if [ "$answer" = "Yes" ];then
            
            local cp_log="$(cd `dirname $0`; pwd)/cp.log"
            echo "[`date '+%Y-%m-%d %H:%M:%S'`] '$copy_src_work_path' to '$copy_dst_work_path' [$copy_file]" >> $cp_log
            
            # sh copy.sh 192.168.1.5 nei all
            if [ "$copy_file" = "all" ];then
                while [ true ]
                do
                    echo -e "\033[1;36m确定对端服务器是否已经关闭\033[0m(Yes/n)? \c"
                    read answer
                    if [ "$answer" = "Yes" ];then
                        copy_all_servers
                        break
                    elif [ "$answer" = "n" ];then
                        break
                    fi
                done
            
            # sh copy.sh 47.100.130.174 root web sg_gmtool
            elif [ "$copy_file" = "web" ];then
                copy_web $copy_web

            # sh copy.sh 192.168.1.5 nei ss
            elif [[ "$copy_file" = "ss" ]];then
                copy_scp -f $copy_src_work_path/sql/sg_statistics.sql  $copy_dst_work_path/sql/sg_statistics.sql
                copy_ssh_cmd "sh $copy_dst_work_path/update.sh sql ss"
                
            # sh copy.sh 192.168.1.5 nei my_config.sh    
            elif [ "$copy_file" = "my_config.sh" ];then
                cp $copy_src_main_path/sh/$copy_dst_ip@$copy_dst_user_name.sh $tmp"_my_config.sh"
                sed -i "s/^\\(version_name=\\).*/\\1\"$version_name\"/" $tmp"_my_config.sh"
                copy_scp -f $tmp"_my_config.sh"  $copy_dst_main_path/sh/my_config.sh
                rm -f $tmp"_my_config.sh"
               
            # sh copy.sh 192.168.1.5 nei game_server
            elif [[ "$copy_file" =~ "_server" ]];then
                while [ true ]
                do
                    echo -e "\033[1;36m确定对端服务器是否已经关闭\033[0m(Yes/n)? \c"
                    read answer
                    if [ "$answer" = "Yes" ];then
                        copy_scp -f $copy_src_work_path/$copy_file/$copy_file  $copy_dst_work_path/$copy_file/$copy_file
                        if [ "$copy_file" = "game_server" ];then
                            copy_ssh_cmd "sh $copy_dst_work_path/update.sh cross"
                        elif [ "$copy_file" = "log_server" ];then
                            copy_ssh_cmd "sh $copy_dst_work_path/update.sh glog"
                        fi
                        break
                    elif [ "$answer" = "n" ];then
                        break
                    fi
                done
                
            # sh copy.sh 192.168.1.5 nei BossBornTable.tbls64
            elif [[ "$copy_file" =~ ".tbls64" ]];then
                    copy_scp -f $copy_src_work_path/config/tbl/$copy_file  $copy_dst_work_path/config/tbl/$copy_file
                    
            # sh copy.sh 192.168.1.5 nei userevent/user_event.lua
            elif [[ "$copy_file" =~ ".lua" ]];then
                copy_scp -f $copy_src_work_path/config/script/game/$copy_file  $copy_dst_work_path/config/script/game/$copy_file

            # sh copy.sh 192.168.1.5 nei nohup_run/run.sh
            elif [[ "$copy_file" =~ ".sh" ]];then
                copy_scp -f $copy_src_main_path/$copy_file  $copy_dst_main_path/$copy_file

            else
                echo -e "\033[1;37;31m错误：参数不匹配! \033[0m"
                exit
            fi

            break
        elif [ "$answer" = "n" ];then
            break
        fi
    done
}

#######################################################################################################
function run_server_monitor()
{
	local sh_name="server_monitor.sh"
	local log="$(cd `dirname $0`; pwd)/sm.log" 
  
    check_server $sh_name
    if [ $? -eq 1 ];then
        return
    fi

    echo "=============================="
    while [ true ]
    do
        echo -e "\033[1;36m是否要开启服务器监控\033[0m(Yes/n)? \c"
        read answer
        if [ "$answer" = "Yes" ];then
            if [ ! -f $log ];then
                touch $log
            fi
            echo "running server monitor..."
            nohup sh $sh_name 1>>$log 2>/dev/null &
            check_server $sh_name
            if [ $? -eq 1 ];then
                echo "success!"
            else
                echo -e "\033[31mfailed!\033[0m"
            fi
            break
        elif [ "$answer" = "n" ];then
            break
        fi
    done
}
function stop_server_monitor()
{
	local sh_name="server_monitor.sh"

    check_server $sh_name
    if [ $? -ne 1 ];then
        return
    fi

    echo "=============================="
    echo "stop server monitor..."
    ps aux | grep -w $ps_name | grep $sh_name | grep -v 'grep' | awk '{print $2}' | xargs kill -9 &>/dev/null
    check_server $sh_name
    res=$?
    while [ $res -ne 0 ]
    do
        check_server $sh_name
        res=$?
    done
    echo "success!"
}
function check_server_monitor()
{
	local sh_name="server_monitor.sh"

    echo "=============================="
    check_server $sh_name
    if [ $? -eq 1 ];then
        echo -e "\033[1;34m服务器监控正在运行中...\033[0m"
        return 0
    else
        echo -e "\033[1;31m服务器监控已关闭!\033[0m"
        return 1
    fi
}

#######################################################################################################
function run_send_combat_logs
{
	local sh_name="send_combat_log.sh"
    local log="$combat_log_dir/cl.log" 
    
    check_server $sh_name
    if [ $? -eq 1 ];then
        return
    fi
    
    echo "=============================="
    while [ true ]
    do
        echo -e "\033[1;36m是否要开启发送战斗回放日志\033[0m(Yes/n)? \c"
        read answer
        if [ "$answer" = "Yes" ];then
            if [ ! -f $log ];then
                touch $log
            fi
            echo "running send combat logs..."
            nohup sh $sh_name 1>>$log 2>/dev/null &
            check_server $sh_name
            if [ $? -eq 1 ];then
                echo "success!"
            else
                echo -e "\033[31mfailed!\033[0m"
            fi
            break
        elif [ "$answer" = "n" ];then
            break
        fi
    done
}
function stop_send_combat_logs
{
	local sh_name="send_combat_log.sh"

    check_server $sh_name
    if [ $? -ne 1 ];then
        return
    fi

    echo "=============================="
    echo "stop send combat logs..."
    ps aux | grep -w $ps_name | grep $sh_name | grep -v 'grep' | awk '{print $2}' | xargs kill -9 &>/dev/null
    check_server $sh_name
    res=$?
    while [ $res -ne 0 ]
    do
        check_server $sh_name
        res=$?
    done
    echo "success!"
}
function check_send_combat_logs()
{
	local sh_name="send_combat_log.sh"

    echo "=============================="
    check_server $sh_name
    if [ $? -eq 1 ];then
        echo -e "\033[1;34m发送战斗回放日志正在运行中...\033[0m"
        return 0
    else
        echo -e "\033[1;31m发送战斗回放日志已关闭!\033[0m"
        return 1
    fi
}

#######################################################################################################
function run_robot_monitor()
{
	local sh_name="robot_monitor.sh"

    check_server $sh_name
    if [ $? -eq 1 ];then
        return
    fi

    echo "=============================="
    while [ true ]
    do
        echo -e "\033[1;36m是否要开启服务器的机器人监控\033[0m(Yes/n)? \c"
        read answer
        if [ "$answer" = "Yes" ];then
            if [ ! -d $rm_log_dir ];then
                mkdir $rm_log_dir
            fi
            echo "running robot monitor..."
            nohup sh $sh_name 1>>$rm_log_dir/monitor.log 2>/dev/null &
            check_server $sh_name
            if [ $? -eq 1 ];then
                echo "success!"
            else
                echo -e "\033[31mfailed!\033[0m"
            fi
            break
        elif [ "$answer" = "n" ];then
            break
        fi
    done
}
function stop_robot_monitor()
{
	local sh_name="robot_monitor.sh"
	local robot="wsy123mon"

    check_server $sh_name
    if [ $? -ne 1 ];then
        return
    fi

    echo "=============================="
    echo "stop robot monitor..."
    ps aux | grep -w $ps_name | grep $sh_name | grep -v 'grep' | awk '{print $2}' | xargs kill -9 &>/dev/null
    ps aux | grep -w $ps_name | grep "./robot_server $robot" | grep -v 'grep' | awk '{print $2}' | xargs kill -9 &>/dev/null
    check_server $sh_name
    res=$?
    while [ $res -ne 0 ]
    do
        check_server $sh_name
        res=$?
    done
    echo "success!"
}
function check_robot_monitor()
{
	local sh_name="robot_monitor.sh"

    echo "=============================="
    check_server $sh_name
    if [ $? -eq 1 ];then
        echo -e "\033[1;34m机器人监控正在运行中...\033[0m"
        return 0
    else
        echo -e "\033[1;31m机器人监控已关闭!\033[0m"
        return 1
    fi
}

#######################################################################################################
function mv_msg_count()
{
    local msg_log_path=$main_path/nohup_run/msg_log/`date '+%Y%m%d'`
    if [ ! -d $msg_log_path ];then
        mkdir -p $msg_log_path
    fi

    local server_path=$msg_log_path/$1
    if [ ! -d $server_path ];then
        mkdir -p $server_path
    fi
    mv $work_path/$1/logs/*.csv $server_path &>/dev/null
}

#######################################################################################################
function run_performance_monitor()
{
	local sh_name="performance_monitor.py"

    check_server $sh_name
    if [ $? -eq 1 ];then
        return
    fi

    echo "=============================="
    while [ true ]
    do
        echo -e "\033[1;36m是否要开启服务器的性能监控\033[0m(Yes/n)? \c"
        read answer
        if [ "$answer" = "Yes" ];then
            if [ ! -d $pm_log_dir ];then
                mkdir $pm_log_dir
            fi
            echo "running performance monitor..."
            nohup python $sh_name 1>>$pm_log_dir/monitor.log 2>/dev/null &
            check_server $sh_name
            if [ $? -eq 1 ];then
                echo "success!"
            else
                echo -e "\033[31mfailed!\033[0m"
            fi
            break
        elif [ "$answer" = "n" ];then
            break
        fi
    done
}
function stop_performance_monitor()
{
	local sh_name="performance_monitor.py"

    check_server $sh_name
    if [ $? -ne 1 ];then
        return
    fi

    echo "=============================="
    echo "stop performance monitor..."
    ps aux | grep -w $ps_name | grep "python $sh_name" | grep -v 'grep' | awk '{print $2}' | xargs kill -s 15 &>/dev/null
    check_server $sh_name
    res=$?
    while [ $res -ne 0 ]
    do
        check_server $sh_name
        res=$?
    done
    echo "success!"
}
function check_performance_monitor()
{
	local sh_name="performance_monitor.py"

    echo "=============================="
    check_server $sh_name
    if [ $? -eq 1 ];then
        echo -e "\033[1;34m性能监控正在运行�?..\033[0m"
        return 0
    else
        echo -e "\033[1;31m性能监控已关�?\033[0m"
        return 1
    fi
}
