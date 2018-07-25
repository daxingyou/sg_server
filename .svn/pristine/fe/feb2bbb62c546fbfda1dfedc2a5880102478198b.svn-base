#!/bin/sh

. ~/sg_server/sh/config.sh

#是否可以使用[all]参数操作
cancel_all=0

################################
#字符串赋值：val = 10
#参数1：文件
#参数2：参数名
#参数3：参数值
#参数4：参数类型 string int
################################
function deassign()
{
    file=$1
    param_name=$2
    param_val=$3
    param_type=$4
    
    if [ "$param_type" = "string" ];then
        sed -i "s/^\\($param_name = \\).*/\\1\"$param_val\"/" $file
    else
        sed -i "s/^\\($param_name = \\).*/\\1$param_val/" $file
    fi
}

################################
#检测svn路径是否有效
################################
function svn_url_valid()
{
    svn_url=$1
    svn info $svn_url &>./svn.log
    lang=$(echo $LANG)
    if [ "$lang" = "en_US.UTF-8" ];then
        rlt=$(cat svn.log | grep "Not a valid URL")
    elif [ "$lang" = "zh_CN.UTF-8" ];then
        rlt=$(cat svn.log | grep "不是有效的URL")
    else
        echo -e "\033[1;37;31m错误：系统语言不符合条件$lang! \033[0m"
        return 1
    fi
    
    if [ "$rlt" != "" ];then
        #无效，不存在
        rm -f svn.log
        return 2
    else
        #有效，已存在
        rm -f svn.log
        return 0
    fi
}

######################################
# 不能热更新的策划表
######################################
function is_can_hot_update()
{
    if [ "$1" = "City.tbls6" ]; then
        return 1
    fi
    if [ "$1" = "Trade.tbls6" ]; then
        return 1
    fi
    if [ "$1" = "TradeCulture.tbls6" ]; then
        return 1
    fi
    if [ "$1" = "TradeFloat.tbls6" ]; then
        return 1
    fi
    if [ "$1" = "TradeGoodsType.tbls6" ]; then
        return 1
    fi
    if [ "$1" = "TradeMarket.tbls6" ]; then
        return 1
    fi
    if [ "$1" = "DropTable.tbls64" ]; then
        return 1
    fi
    
    return 0
}

######################################
# 检查是否有服务器在开启
######################################
function check_process()
{
    return `ps -ef | grep $1 | grep -w $2 | grep -v "grep" | wc -l `
}
function is_server_running()
{
    if [ "$1" = "all" ];then
        local run_num=0
        for s in $run_global_server_list
        do
            local srv_name=$s"_server"
            check_process $user_name $srv_name
            if [ $? -eq 1 ];then
                let run_num++
            fi
        done
        for s in $run_group_server_list
        do
            local srv_name=$s"_server"
            check_process $user_name $srv_name
            if [ $? -eq 1 ];then
                let run_num++
            fi
        done
        if [[ $run_num -ge 1 ]];then
            echo -e "\033[1;37;31m错误：请关闭服务器再开始更新! \033[0m"
            return 1
        fi
    elif [ "$1" = "group" ];then
        local run_num=0
        for s in $run_group_server_list
        do
            local srv_name=$s"_server"
            check_process $user_name $srv_name
            if [ $? -eq 1 ];then
                let run_num++
            fi
        done
        if [[ $run_num -ge 1 ]];then
            echo -e "\033[1;37;31m错误：请关闭服务器再开始更新! \033[0m"
            return 1
        fi
    elif [ "$1" = "account" -o "$1" = "area" -o "$1" = "glog" ];then
        local srv_name=$1"_server"
        check_process $user_name $srv_name
        if [ $? -eq 1 ];then
             echo -e "\033[1;37;31m错误：请关闭$srv_name再开始更新! \033[0m"
            return 1
        fi

    else
        return 1
    fi
    return 0
}

#########################################################
#更新global_server.xml
#########################################################
function make_transfer_temp()                                                                                         
{   
    local transfer_num=$1                                                                                             
    local tmp_file=.transfer_template.xml
    
    sed -n  '/\<TRANSFER_NO/,/TRANSFER_NO\>/p' $2 > $tmp_file
    for (( i=$transfer_num; i>0; i-- ))                                                                               
    do  
        sed -i ':a;N;$!ba;s/#TRANSFER_NO#/transfer'$i'/1'  $2                                                         
        sed -i ':a;N;$!ba;s/#TRANSFER_NO#/transfer'$i'/1'  $2                                                         
        if (( i != 1 ))
        then
            sed -i '/#TRANSFER_CONF#/r '$tmp_file''  $2                                                         
        fi
    done                                                                                                              
    rm -f $tmp_file
}                                                                                                                     

function make_account_temp()                                                                                          
{   
    local account_num=$1                                                                                              
    local tmp_file=.account_template.xml
    
    sed -n  '/\<ACCOUNT_NO/,/ACCOUNT_NO\>/p' $2 > $tmp_file
    for (( i=$account_num; i>0; i-- ))                                                                                
    do  
        sed -i ':a;N;$!ba;s/#ACCOUNT_NO#/account'$i'/1'   $2                                                          
        sed -i ':a;N;$!ba;s/#ACCOUNT_NO#/account'$i'/1'   $2                                                          
        if (( i != 1 )) 
        then
            sed -i '/#ACCOUNT_CONF#/r  '$tmp_file''  $2                                                          
        fi
    done                                                                                                              
    rm -f $tmp_file
}                                                                                                                     

function make_cross_temp()                                                                                            
{   
    local cross_num=$1                                                                                                
    local tmp_file=.cross_template.xml

    sed -n  '/\<CROSS_NO/,/CROSS_NO\>/p' $2 > $tmp_file
    for (( i=$cross_num; i>0; i-- ))                                                                                  
    do  
        sed -i ':a;N;$!ba;s/#CROSS_NO#/cross'$i'/1'   $2                                                              
        sed -i ':a;N;$!ba;s/#CROSS_NO#/cross'$i'/1'   $2                                                              
        if (( i != 1 ))
        then
            sed -i '/#CROSS_CONF#/r  '$tmp_file''  $2                                                              
        fi  
    done                                                                                                              

    rm  -f  $tmp_file
}       

function make_gate_temp()
{
    local gate_num=$1
    local tmp_file=.gate_template.xml

    sed -n  '/\<GATE_NO/,/GATE_NO\>/p' $2 > $tmp_file
    for (( i=$gate_num; i>0; i-- ))
    do
        sed -i ':a;N;$!ba;s/#GATE_NO#/gate'$i'/1'  $2
        sed -i ':a;N;$!ba;s/#GATE_NO#/gate'$i'/1'  $2
        if (( i != 1 ))
        then
            sed -i '/#GATE_CONF#/r  '$tmp_file'' $2
        fi
    done
    rm  -f  $tmp_file
}

function update_global_server_xml()
{
    echo -e "\033[1;36m更新【global_server.xml】\033[0m"
    
    # 原文件
    local XML_SRC="$work_path/global_server_new.xml"
    # 生成的文件
    local XML_DST="$work_path/global_server.xml"
    # 临时过渡文件
    local TMP_XML_DST="$work_path/global_server.xml_templete"
    cp $XML_SRC  $TMP_XML_DST -f

    #生成模板文件
    make_transfer_temp  $transfer_number $TMP_XML_DST
    make_cross_temp     $cross_number   $TMP_XML_DST
    make_account_temp   $account_number $TMP_XML_DST

    #shell库目录
    local SHELL_LIB_DIR="$main_path/sh"
    #引用set_xml_value.sh
    PATH=$PATH:$SHELL_LIB_DIR   

    # 渠道
    sh set_xml_value.sh   server.channel $global_channel $TMP_XML_DST
    
    sh set_xml_value.sh   server.server_list_type $server_list_type $TMP_XML_DST
    
    #修改account_server ip节点      
    #修改account1
    for (( i = 1; i<=$account_number; i++ ))
    {
        sh set_xml_value.sh   "server.account_server.account$i.listen.client.ip"    $account_server_listen_ip $TMP_XML_DST
        sh set_xml_value.sh   "server.account_server.account$i.listen.login.ip"     $account_server_listen_ip $TMP_XML_DST
        sh set_xml_value.sh   "server.account_server.account$i.listen.db.ip"        $account_server_listen_ip $TMP_XML_DST
    }
    #修改host
    sh set_xml_value.sh   server.account_server.mysql.host $account_db_ip $TMP_XML_DST
    
    #修改transfer_server节点
    for (( i = 1; i<=$transfer_number; i++ ))
    {
        sh set_xml_value.sh   "server.transfer_server.transfer$i.listen.chat.ip"        $transfer_server_listen_ip $TMP_XML_DST
        sh set_xml_value.sh   "server.transfer_server.transfer$i.listen.center.ip"      $transfer_server_listen_ip $TMP_XML_DST
    }

    #修改area_server节点
    sh set_xml_value.sh   server.area_server.listen.transfer.ip $area_server_listen_ip $TMP_XML_DST
    sh set_xml_value.sh   server.area_server.listen.cross.ip    $area_server_listen_ip $TMP_XML_DST
    sh set_xml_value.sh   server.area_server.mysql.host         $area_db_ip  $TMP_XML_DST
    sh set_xml_value.sh   server.area_server.redis.host         $area_redis_ip  $TMP_XML_DST
    sh set_xml_value.sh   server.area_server.redis.db           $area_redis_db $TMP_XML_DST

    #修改cross_server节点
    for (( i = 1; i<=$cross_number; i++ ))
    {
        sh set_xml_value.sh   "server.cross_server.cross$i.listen.gate.ip"   $cross_server_listen_ip $TMP_XML_DST
    }

    #修改glog节点   
    sh set_xml_value.sh   server.glog_server.listen.account.ip $glog_server_listen_ip $TMP_XML_DST
    sh set_xml_value.sh   server.glog_server.mysql.host        $glog_db_ip     $TMP_XML_DST

    #增长替换port
    local change_num=$(grep "#CHANGE_PORT#" $TMP_XML_DST | wc -l)       
    local start_port=$global_server_port_start
    
    for (( i=0; i<$change_num; i++ ))
    do
        sed -i ':a;N;$!ba;s/#CHANGE_PORT#/'$start_port'/1'  $TMP_XML_DST
        ((start_port=start_port+1))
    done

    #增长替换account_port       
    local account_change_num=$(grep "#ACCOUNT_CHANGE_PORT#" $TMP_XML_DST | wc -l)       
    local account_start_port=$account_server_port_start
    for (( i=0; i<$account_change_num; i++ ))
    do
        sed -i ':a;N;$!ba;s/#ACCOUNT_CHANGE_PORT#/'$account_start_port'/1'  $TMP_XML_DST
        ((account_start_port=account_start_port+1))
    done 

    mv $TMP_XML_DST $XML_DST
}

#########################################################
#更新server.xml
#########################################################
function update_server_xml()
{
    echo -e "\033[1;36m更新【server.xml】\033[0m"
    
    OLD_IFS=$IFS #保存原始值 空格
    IFS="" #改变IFS的值

    # 原文件
    XML_SRC="$work_path/server_new.xml"
    # 生成的文件
    XML_DST="$work_path/server.xml"
    # 临时模板文件
    TMP_XML_DST="$work_path/server.xml_temp"
    cp $XML_SRC  $TMP_XML_DST -f

    #生成模板文件
    make_gate_temp   $gate_number   $TMP_XML_DST

    #shell库目录
    local SHELL_LIB_DIR="$main_path/sh"
    #引用set_xml_value.sh
    PATH=$PATH:$SHELL_LIB_DIR
   
    # 渠道
    sh set_xml_value.sh  server.channel                             $channel                        $TMP_XML_DST

    # GM命令权限
    sh set_xml_value.sh  server.gm_level                            $gm_level                       $TMP_XML_DST 
    
    # login             
    sh set_xml_value.sh  server.server_id                           $server_id                      $TMP_XML_DST 
    sh set_xml_value.sh  server.server_name                         $server_name                    $TMP_XML_DST 

    sh set_xml_value.sh  server.login_server.listen.client.ip       $server_ip                      $TMP_XML_DST 
    sh set_xml_value.sh  server.login_server.listen.client.out_ip   $server_out_ip                  $TMP_XML_DST 
    sh set_xml_value.sh  server.login_server.listen.gate.ip         $server_ip                      $TMP_XML_DST 

    sh set_xml_value.sh  server.login_server.connect.account.ip     $connect_to_account_ip           $TMP_XML_DST 
    sh set_xml_value.sh  server.login_server.connect.port           $login_connect_to_account_port  $TMP_XML_DST 

    sh set_xml_value.sh  server.login_server.mysql.host             $game_db_ip                     $TMP_XML_DST 
    sh set_xml_value.sh  server.login_server.mysql.db               $sql_sg_game                    $TMP_XML_DST 
    sh set_xml_value.sh  server.login_server.redis.host             $game_redis_ip                  $TMP_XML_DST 
    sh set_xml_value.sh  server.login_server.redis.db               $game_redis_db                  $TMP_XML_DST 

    # gate
    sh set_xml_value.sh  server.gate_server.timeout.session         $gate_timeout_session           $TMP_XML_DST 
    sh set_xml_value.sh  server.gate_server.timeout.pingpong        $gate_timeout_pingpong          $TMP_XML_DST 
    sh set_xml_value.sh  server.gate_server.timeout.reconnect       $gate_timeout_reconnect         $TMP_XML_DST 
    sh set_xml_value.sh  server.gate_server.timeout.alive           $gate_timeout_alive             $TMP_XML_DST 
 
    sh set_xml_value.sh  server.gate_server.connect.cross.ip        $gate_connect_to_cross_ip       $TMP_XML_DST 
    sh set_xml_value.sh  server.gate_server.connect.cross.port      $gate_connect_to_cross_port     $TMP_XML_DST 
    for (( i = 1; i<=$gate_number; i++ ))
    {
        sh set_xml_value.sh  "server.gate_server.gate$i.listen.client.ip"           $server_ip      $TMP_XML_DST 
        sh set_xml_value.sh  "server.gate_server.gate$i.listen.client.out_ip"       $server_out_ip  $TMP_XML_DST 
        sh set_xml_value.sh  "server.gate_server.gate$i.listen.game.ip"             $server_ip      $TMP_XML_DST 
    }

    # db
    sh set_xml_value.sh  server.db_server.listen.game.ip            $server_ip                      $TMP_XML_DST 
    sh set_xml_value.sh  server.db_server.listen.center.ip          $server_ip                      $TMP_XML_DST 

    sh set_xml_value.sh  server.db_server.connect.account.ip        $connect_to_account_ip          $TMP_XML_DST 
    sh set_xml_value.sh  server.db_server.connect.account.port      $db_connect_to_account_port     $TMP_XML_DST 

    sh set_xml_value.sh  server.db_server.mysql.host                $game_db_ip                     $TMP_XML_DST 
    sh set_xml_value.sh  server.db_server.mysql.db                  $sql_sg_game                    $TMP_XML_DST 
    sh set_xml_value.sh  server.db_server.redis.host                $game_redis_ip                  $TMP_XML_DST 
    sh set_xml_value.sh  server.db_server.redis.db                  $game_redis_db                  $TMP_XML_DST 

    # center
    sh set_xml_value.sh  server.center_server.listen.game.ip    $server_ip                          $TMP_XML_DST 
    sh set_xml_value.sh  server.center_server.listen.gate.ip    $server_ip                          $TMP_XML_DST 
    sh set_xml_value.sh  server.center_server.listen.chat.ip    $server_ip                          $TMP_XML_DST 
    sh set_xml_value.sh  server.center_server.listen.gm.ip      $server_ip                          $TMP_XML_DST 

    sh set_xml_value.sh  server.center_server.connect.transfer.ip   $center_connect_to_transfer_ip      $TMP_XML_DST 
    sh set_xml_value.sh  server.center_server.connect.transfer.port $center_connect_to_transfer_port    $TMP_XML_DST 

    sh set_xml_value.sh  server.center_server.mysql.host            $game_db_ip                     $TMP_XML_DST 
    sh set_xml_value.sh  server.center_server.mysql.db              $sql_sg_game                    $TMP_XML_DST 
    sh set_xml_value.sh  server.center_server.redis.host            $game_redis_ip                  $TMP_XML_DST 
    sh set_xml_value.sh  server.center_server.redis.db              $game_redis_db                  $TMP_XML_DST 

    # chat
    sh set_xml_value.sh  server.chat_server.listen.game.ip          $server_ip                      $TMP_XML_DST 
    sh set_xml_value.sh  server.chat_server.listen.gate.ip          $server_ip                      $TMP_XML_DST 

    sh set_xml_value.sh  server.chat_server.connect.transfer.ip     $chat_connect_to_transfer_ip    $TMP_XML_DST 
    sh set_xml_value.sh  server.chat_server.connect.transfer.port   $chat_connect_to_transfer_port  $TMP_XML_DST 

    # log
    sh set_xml_value.sh  server.log_server.listen.game.ip       $server_ip                      $TMP_XML_DST 
    sh set_xml_value.sh  server.log_server.listen.center.ip     $server_ip                      $TMP_XML_DST 
    sh set_xml_value.sh  server.log_server.mysql.host           $log_db_ip                      $TMP_XML_DST 
    sh set_xml_value.sh  server.log_server.mysql.db             $sql_sg_log                     $TMP_XML_DST 

    # gm
    sh set_xml_value.sh  server.gm_server.listen.gmtool.ip      $server_ip                      $TMP_XML_DST 
    sh set_xml_value.sh  server.gm_server.mysql.host            $game_db_ip                     $TMP_XML_DST 
    sh set_xml_value.sh  server.gm_server.mysql.db              $sql_sg_game                    $TMP_XML_DST 

    # 自增替换port
    local change_num=$(grep "#CHANGE_PORT#" $TMP_XML_DST | wc -l)
    local start_port=$server_port_start

    for (( i=0; i<$change_num; i++ ))
    do
        sed -i ':a;N;$!ba;s/#CHANGE_PORT#/'$start_port'/1'  $TMP_XML_DST
        ((start_port=start_port+1))
    done    

    # 设置特殊端口out_port
    # login
    local out_port=$( sh get_xml_value.sh  server.login_server.listen.port  $TMP_XML_DST )
    sh set_xml_value.sh  server.login_server.listen.out_port $out_port  $TMP_XML_DST
    # gate
    for (( i = 1; i<=$gate_number; i++ ))
    do
        local out_port=$(sh get_xml_value.sh  "server.gate_server.gate$i.listen.client.port" $TMP_XML_DST )
        sh set_xml_value.sh  "server.gate_server.gate$i.listen.client.out_port" $out_port   $TMP_XML_DST 
    done

    mv $TMP_XML_DST $XML_DST
}

#########################################################
#更新sql
#########################################################
function update_sql()
{
	if [ "$1" = "all" ]; then
		if [ $cancel_all -eq 1 ];then
			echo -e "\033[1;37;31m错误：禁止使用[all]操作! \033[0m"
			exit
		fi
	fi

    echo -e "\033[1;36m更新【sql】\033[0m"

    if [ ! -d $work_path/sql ];then
        echo -e "\033[1;37;31m错误：$work_path/sql目录不存在！\033[0m"
        return
    fi
    
    # 修改【sql/config.py】，最终执行【database.py】
    cd $work_path/sql
    rm -f config.py
    cp -f config_new.py config.py
    
    deassign    config.py   con_accountdb [\"$account_db_ip\",\"root\",\"Sanguo1\!\",\"sg_account\",\"sg_account.sql\",\"$server_id\"\] 0
    deassign    config.py   gamedb       [\"$create_game_db_ip\",\"root\",\"Sanguo1!\",\"$sql_sg_game\",\"sg_game.sql\"]   0
    deassign    config.py   logdb        [\"$create_log_db_ip\",\"root\",\"Sanguo1\!\",\"$sql_sg_log\",\"sg_log.sql\"\]   0
    deassign    config.py   accountdb    [\"$create_account_db_ip\",\"root\",\"Sanguo1\!\",\"sg_account\",\"sg_account.sql\",\"$server_id\"\] 0
    deassign    config.py   areadb       [\"$create_area_db_ip\",\"root\",\"Sanguo1\!\",\"sg_area\",\"sg_area.sql\"] 0
    deassign    config.py   glogdb       [\"$create_glog_db_ip\",\"root\",\"Sanguo1\!\",\"sg_glog\",\"sg_glog.sql\"] 0
    deassign    config.py   bbsdb        [\"$create_bbs_db_ip\",\"root\",\"Sanguo1\!\",\"sg_bbs\",\"sg_bbs.sql\"] 0
    deassign    config.py   updb         [\"$create_up_db_ip\",\"root\",\"Sanguo1\!\",\"sg_up\",\"sg_up.sql\"] 0
    deassign    config.py   ssdb         [\"$create_ss_db_ip\",\"root\",\"Sanguo1\!\",\"sg_statistics\",\"sg_statistics.sql\"] 0
    deassign    config.py   global_redis [\"$create_area_redis_ip\",\"6379\",\"Sanguo1\!\",\"$create_area_redis_db\"] 0
    deassign    config.py   game_redis   [\"$create_game_redis_ip\",\"6379\",\"Sanguo1\!\",\"$create_game_redis_db\"] 0
    
    if [ ! -d $work_path/sql/logs ];then
        mkdir $work_path/sql/logs
    fi
    
    if [ "$1" = "all" ]; then
        echo -e "\033[1;36m<sg_account>\033[0m"
        echo -e "\033[1;36m   mysql_ip: $create_account_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: sg_account\033[0m"
        echo -e "\033[1;36m<sg_area>\033[0m"
        echo -e "\033[1;36m   mysql_ip: $create_area_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: sg_area\033[0m"
        echo -e "\033[1;36m   redis_ip: $create_area_redis_ip\033[0m"
        echo -e "\033[1;36m   redis_db: $create_area_redis_db\033[0m"
        echo -e "\033[1;36m<sg_glog>\033[0m"
        echo -e "\033[1;36m   mysql_ip: $create_glog_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: sg_glog\033[0m"
        echo -e "\033[1;36m<sg_game>\033[0m"
        echo -e "\033[1;36m   account:  $create_account_db_ip: server_id($server_id)\033[0m"
        echo -e "\033[1;36m   mysql_ip: $create_game_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: $sql_sg_game\033[0m"
        echo -e "\033[1;36m   redis_ip: $create_game_redis_ip\033[0m"
        echo -e "\033[1;36m   redis_db: $create_game_redis_db\033[0m"
        echo -e "\033[1;36m<sg_log>\033[0m"
        echo -e "\033[1;36m   mysql_ip: $create_log_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: $sql_sg_log\033[0m"
    
    elif [ "$1" = "account" ]; then
        echo -e "\033[1;36m<sg_account>\033[0m"
        echo -e "\033[1;36m   mysql_ip: $create_account_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: sg_account\033[0m"
        
    elif [ "$1" = "area" ]; then
        echo -e "\033[1;36m<sg_area>\033[0m"
        echo -e "\033[1;36m   mysql_ip: $create_area_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: sg_area\033[0m"
        echo -e "\033[1;36m   redis_ip: $create_area_redis_ip\033[0m"
        echo -e "\033[1;36m   redis_db: $create_area_redis_db\033[0m"
        
    elif [ "$1" = "glog" ]; then
        echo -e "\033[1;36m<sg_glog>\033[0m"
        echo -e "\033[1;36m   mysql_ip: $create_glog_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: sg_glog\033[0m"

    elif [ "$1" = "game" ]; then
        echo -e "\033[1;36m<sg_game>\033[0m"
        echo -e "\033[1;36m   account:  $account_db_ip: server_id($server_id)\033[0m"
        echo -e "\033[1;36m   mysql_ip: $create_game_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: $sql_sg_game\033[0m"
        echo -e "\033[1;36m   redis_ip: $create_game_redis_ip\033[0m"
        echo -e "\033[1;36m   redis_db: $create_game_redis_db\033[0m"
        echo -e "\033[1;36m<sg_log>\033[0m"
        echo -e "\033[1;36m   mysql_ip: $create_log_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: $sql_sg_log\033[0m"
        
    elif [ "$1" = "log" ]; then
        echo -e "\033[1;36m   mysql_ip: $create_log_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: $sql_sg_log\033[0m"
    
    elif [ "$1" = "bbs" ]; then
        echo -e "\033[1;36m   mysql_ip: $create_bbs_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: sg_bbs\033[0m"

    elif [ "$1" = "up" ]; then
        echo -e "\033[1;36m   mysql_ip: $create_up_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: sg_up\033[0m"
    
    elif [ "$1" = "ss" ]; then
        echo -e "\033[1;36m   mysql_ip: $create_ss_db_ip\033[0m"
        echo -e "\033[1;36m   mysql_db: sg_statistics\033[0m"

    else
        echo -e "\033[1;37;31m错误：无效参数! \033[0m"
        exit
    fi
        
    if [ "$2" = "d" ]; then
        while [ true ]
        do
            echo -e "\033[1;35m是否要删除数据库？\033[0m(Yes/n) \c"
            read answer
            if [ "$answer" = Yes ];then
                if [ "$1" = "all" ]; then
                    python database.py d account area glog game log
                elif [ "$1" = "account" ]; then
                    python database.py d account
                elif [ "$1" = "area" ]; then
                    python database.py d area
                elif [ "$1" = "glog" ]; then
                    python database.py d glog
                elif [ "$1" = "game" ]; then
                    python database.py d game log
                elif [ "$1" = "log" ]; then
                    python database.py d log
                elif [ "$1" = "bbs" ]; then
                    python database.py d bbs
                elif [ "$1" = "up" ]; then
                    python database.py d up
                elif [ "$1" = "ss" ]; then
                    python database.py d ss            
                fi
                break
            elif [ "$answer" = n ];then
                break
            fi
        done
    else
        if [ "$1" = "all" ]; then
            echo "account--------------------------"
            python database.py i account
            python database.py u account
            
            echo "area--------------------------"
            python database.py i area
            python database.py u area
            
            echo "glog--------------------------"
            python database.py i glog
            python database.py u glog
            
            echo "game--------------------------"
            python database.py i game
            python database.py u game
            
            echo "log--------------------------"
            python database.py i log
            python database.py u log
            
        elif [ "$1" = "account" ]; then
            python database.py i account
            python database.py u account
            
        elif [ "$1" = "area" ]; then
            python database.py i area
            python database.py u area
            
        elif [ "$1" = "glog" ]; then
            python database.py i glog
            python database.py u glog

        elif [ "$1" = "game" ]; then
            python database.py i game log
            python database.py u game log
            
        elif [ "$1" = "log" ]; then
            python database.py i log
            python database.py u log

        elif [ "$1" = "bbs" ]; then
            python database.py i bbs
            python database.py u bbs

        elif [ "$1" = "up" ]; then
            python database.py i up
            python database.py u up
         
        elif [ "$1" = "ss" ]; then
            python database.py i ss
            python database.py u ss
                    
        fi

        while [ true ]
        do
            echo -e "\033[1;35m是否要清空数据库？\033[0m(Yes/n) \c"
            read answer
            if [ "$answer" = Yes ];then
                if [ "$1" = "all" ]; then
                    python database.py r account area glog game log
                elif [ "$1" = "account" ]; then
                    python database.py r account
                elif [ "$1" = "area" ]; then
                    python database.py r area
                elif [ "$1" = "glog" ]; then
                    python database.py r glog
                elif [ "$1" = "game" ]; then
                    python database.py r game log
                elif [ "$1" = "log" ]; then
                    python database.py r log
                elif [ "$1" = "bbs" ]; then
                    python database.py r bbs
                elif [ "$1" = "up" ]; then
                    python database.py r up 
                elif [ "$1" = "ss" ]; then
                    python database.py r ss           
                fi
                break
            elif [ "$answer" = n ];then
                break
            fi
        done
    fi
    
    cd $work_path
}

#########################################
# 更新对应版本的策划表到服务器本地
#########################################
function update_tbl()
{
:<<!
    while [ true ]
    do
        echo -e "\033[1;35m该脚本不支持windows的svn更新，请确认是否在linux上执行:\033[0m (y/n)? \c"
        read answer
        if [ "$answer" = y -o "$answer" = Y ];then
            break
        elif [ "$answer" = n -o "$answer" = N ];then
            return 1
        fi
    done
!
    if [ "$1" = "all" ]; then
		if [ $cancel_all -eq 1 ];then
			echo -e "\033[1;37;31m错误：禁止使用[all]操作! \033[0m"
			exit
		fi
	fi

    if [ "$version_name" != "trunk" ];then
        svn_url_valid $design_svn_path
        if [ $? -ne 0 ];then
            echo -e "\033[1;37;31m错误：策划分支版本$design_svn_path不存在! \033[0m"
            return 1
        fi
    fi

    echo -e "\033[1;36m更新【/config/tbl】:\033[0m \033[1;37m$design_svn_path/out --> $work_path/config/tbl \033[0m"
    
    cd ./config
    if [ ! -d ./tbl ]; then 
        svn co "$design_svn_path/out" tbl 1>/dev/null
    fi
    if [ "$1" = "all" ]; then
        svn up ./tbl
    elif [[ "$1" =~ ".tbls64" ]];then
        svn up ./tbl/$1
    else
        echo -e "\033[1;37;31m错误：参数不匹配! \033[0m"
        return
    fi
    cd ../

    is_can_hot_update $1
    if [ $? -eq 1 ];then
        echo -e "\033[1;35m需要重启服务器！！！\033[0m"
    fi

    return 0
}

#########################################
# 更新对应版本的策划表到服务器本地
#########################################
function update_map()
{
:<<!
    while [ true ]
    do
        echo -e "\033[1;35m该脚本不支持windows的svn更新，请确认是否在linux上执行:\033[0m (y/n)? \c"
        read answer
        if [ "$answer" = y -o "$answer" = Y ];then
            break
        elif [ "$answer" = n -o "$answer" = N ];then
            return 1
        fi
    done
!
    if [ "$1" = "all" ]; then
		if [ $cancel_all -eq 1 ];then
			echo -e "\033[1;37;31m错误：禁止使用[all]操作! \033[0m"
			exit
		fi
	fi

    echo -e "\033[1;36m更新【/config/map】:\033[0m \033[1;37m$design_svn_path/map --> $work_path/config/map \033[0m"

    cd ./config
    if [ ! -d ./map ]; then 
        svn co "$design_svn_path/map" map 1>/dev/null     
    fi
    if [ "$1" = "all" ]; then
        svn up ./map
    elif [[ "$1" =~ ".xml" ]]; then
        svn up ./map/$1
    elif [[ "$1" =~ ".map" ]]; then
        svn up ./map/$1
    else
        echo -e "\033[1;37;31m错误：参数不匹配! \033[0m"
        return
    fi
    cd ../

    echo -e "\033[1;35m需要重启服务器！！！\033[0m"
}

################################
#更新script
################################
function update_script()
{
:<<!
    while [ true ]
    do
        echo -e "\033[1;35m该脚本不支持windows的svn更新，请确认是否在linux上执行:\033[0m (y/n)? \c"
        read answer
        if [ "$answer" = y -o "$answer" = Y ];then
            break
        elif [ "$answer" = n -o "$answer" = N ];then
            return 1
        fi
    done
!
    if [ "$version_name" != "trunk" ];then
        svn_url_valid $design_svn_path
        if [ $? -ne 0 ];then
            echo -e "\033[1;37;31m错误：策划分支版本$design_svn_path不存在! \033[0m"
            return 1
        fi
    fi

    echo -e "\033[1;36m更新【/config/script】:\033[0m \033[1;37m$design_svn_path/script --> $work_path/config/script \033[0m"
    
    cd ./config
    if [ -d ./script ]; then
        svn up ./script
    else
        svn co "$design_svn_path/script" script 1>/dev/null
    fi
    cd ../

    return 0
}

################################
#更新ai
################################
function update_ai()
{
:<<!
    while [ true ]
    do
        echo -e "\033[1;35m该脚本不支持windows的svn更新，请确认是否在linux上执行:\033[0m (y/n)? \c"
        read answer
        if [ "$answer" = y -o "$answer" = Y ];then
            break
        elif [ "$answer" = n -o "$answer" = N ];then
            return 1
        fi
    done
!
    if [ "$version_name" != "trunk" ];then
        svn_url_valid $design_svn_path
        if [ $? -ne 0 ];then
            echo -e "\033[1;37;31m错误：策划分支版本$design_svn_path! \033[0m"
            return 1
        fi
    fi

    echo -e "\033[1;36m更新【/config/ai】:\033[0m \033[1;37m$design_svn_path/ai --> $work_path/config/ai \033[0m"
    
    cd ./config
    if [ -d ./ai ]; then
        svn up ./ai
    else
        svn co "$design_svn_path/ai" ai 1>/dev/null
    fi
    cd ../

    return 0
}

################################
#更新code
################################
function update_server()
{
:<<!
    while [ true ]
    do
        echo -e "\033[1;35m该脚本不支持windows的svn更新，请确认是否在linux上执行:\033[0m (y/n)? \c"
        read answer
        if [ "$answer" = y -o "$answer" = Y ];then
            break
        elif [ "$answer" = n -o "$answer" = N ];then
            return 1
        fi
    done
!
    echo -e "\033[1;36m更新代码:\033[0m \033[1;37m$server_svn_path --> $work_path\033[0m"
    
    svn up |& tee ./svn.log
    rlt=$(cat svn.log | grep -E proto.\+\.proto)
    if [ "$rlt" != "" ];then
        sh gen_proto.sh
    fi
    rm -f svn.log

    return 0
}

function update_cross_server()
{
    echo -e "\033[1;36m更新【cross_server】:\033[0m \033[1;37m$work_path/game_server/game_server --> $work_path/cross_server/cross_server\033[0m"
    cp $work_path/game_server/game_server $work_path/cross_server/cross_server
}

function update_glog_server()
{
    echo -e "\033[1;36m更新【glog_server】:\033[0m \033[1;37m$work_path/log_server/log_server --> $work_path/glog_server/glog_server\033[0m"
    cp $work_path/log_server/log_server $work_path/glog_server/glog_server
}

###################################################################################################
#生成版本号
function make_version_no()
{
    local XML_DST="$work_path/server.xml"
    local SHELL_LIB_DIR="$main_path/sh"
    PATH=$PATH:$SHELL_LIB_DIR   

    if [ ! -f .version_no_file ]
    then
        touch version_no_file
        echo "1.1.0.$(date +%y%m%d%H%M).0" > .version_no_file
    fi

    local version_no=$(cat .version_no_file)
    echo -e "\033[36m服务器旧版本号[小版本号.热更版本号.svn版本号.时间.记录版本号]：$version_no\033[0m"
    local sml_v_no=$(echo $version_no | cut -d . -f 1)
    local hot_v_no=$(echo $version_no | cut -d . -f 2)
    local svn_v_no=$(svn info| awk 'NR==5{print $2}')
    local dte_v_no=$(date +%y%m%d-%H%M)
    local add_v_no=$(echo $version_no | cut -d . -f 5)
    ((add_v_no=add_v_no+1))

    local last_date=$(echo $version_no | cut -d . -f 4 | cut -d - -f 1)
    local cur_date=$(date +%y%m%d)

    if ((  $last_date != $cur_date ))
    then
        add_v_no=1
    fi

    local imput_version="NULL"
    local cur_version_no=$sml_v_no.$hot_v_no.$svn_v_no.$dte_v_no.$add_v_no
    if (( 1 == 2 )); then
        echo "生成版本号:$cur_version_no"
        while [ true ]
        do
            echo -e "\033[1;35m是否手动输入版本号\033[0m(Yes/n) \c"
            read answer
            if [ "$answer" = Yes ];then
                echo "请输入版本号:"
                read imput_version
                local imput_num=$(echo $imput_version | awk -F '.' '{print NF}')
                if [ $imput_num != 5 ]; then
                    echo -e "\033[1;35m版本号必须为,小版本号.热更版本号.svn版本号.时间.记录版本号 \033[0m \n"
                    imput_version="NULL"
                    continue
                fi
                break
            elif [ "$answer" = n ];then
                break
            fi
        done
    fi

    if [ $imput_version == 'NULL' ]
    then
        imput_version=$cur_version_no
    fi

    sh set_xml_value.sh   server.version    $imput_version  $XML_DST
    echo $imput_version > .version_no_file
    
    echo -e "\033[36m服务器新版本号[小版本号.热更版本号.svn版本号.时间.记录版本号]：$imput_version\033[0m"
}

###################################################################################################
is_make=false
if [ "$1" = "xml" ];then
    is_server_running 'all'
    if [ $? -ne 0 ];then
        exit
    fi
    echo "============================================================================="
    update_global_server_xml
    update_server_xml

elif [ "$1" = "sql" ];then
    if [ "$2" = "game" ];then
        is_server_running 'group'
        if [ $? -ne 0 ];then
            exit
        fi
    elif [ "$2" = "account" ];then
        is_server_running 'account'
        if [ $? -ne 0 ];then
            exit
        fi
    elif [ "$2" = "area" ];then
        is_server_running 'area'
        if [ $? -ne 0 ];then
            exit
        fi
    elif [ "$2" = "glog" ];then
        is_server_running 'glog'
        if [ $? -ne 0 ];then
            exit
        fi
    fi
    echo "============================================================================="
    update_sql $2 $3

    echo "============================================================================="
    make_version_no
    
elif [ "$1" = "cross" ];then
    echo "============================================================================="
    update_cross_server

elif [ "$1" = "glog" ];then
    echo "============================================================================="
    update_glog_server
       
elif [ "$1" = "tbl" ];then
    echo "============================================================================="
    update_tbl $2
    
    echo "============================================================================="
    make_version_no
    
elif [ "$1" = "map" ];then
    echo "============================================================================="
    update_map $2
    
    echo "============================================================================="
    make_version_no
    
elif [ "$1" = "script" ];then
    echo "============================================================================="
    update_script
    
    echo "============================================================================="
    make_version_no
    
elif [ "$1" = "ai" ];then
    echo "============================================================================="
    update_ai
    
    echo "============================================================================="
    make_version_no
    
elif [ "$1" == "ver" ]; then
    echo "============================================================================="
    make_version_no

elif [ "$1" == "code" ]; then
    echo "============================================================================="
    update_server
    if [ $? -le 0 ];then
        is_make=true
    fi
    
    echo "============================================================================="
    make_version_no
    
elif [ "$1" = "all" ];then
	if [ "$1" = "all" ]; then
		if [ $cancel_all -eq 1 ];then
			echo -e "\033[1;37;31m错误：禁止使用[all]操作! \033[0m"
			exit
		fi
	fi

    is_server_running 'all'
    if [ $? -ne 0 ];then
        exit
    fi
    
    echo "============================================================================="
    update_tbl "all"

    echo "============================================================================="
    update_map "all"

    echo "============================================================================="
    update_script

    echo "============================================================================="
    update_ai

    echo "============================================================================="
    update_server
    if [ $? -le 0 ];then
        is_make=true
    fi

    echo "============================================================================="
    update_global_server_xml
    update_server_xml
    
    echo "============================================================================="
    update_sql 'game'
    
    echo "============================================================================="
    make_version_no
    
else
    echo -e "\033[1;37;31m错误：无效参数! \033[0m"
    exit
fi

if [ $is_make = true ];then
    echo "============================================================================="
    echo -e "\033[1;36m开始编译代码\033[0m"
    make -j8
    echo -e "\033[1;36m完成编译代码！\033[0m"

    echo "============================================================================="
    update_cross_server
    
    echo "============================================================================="
    update_glog_server
fi

echo "============================================================================="
echo -e "\033[1;36m服务器更新完成！\033[0m"