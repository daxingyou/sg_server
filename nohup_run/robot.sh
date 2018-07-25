#!/bin/sh

# sh robot.sh gmtest 26 2 OUT_BRANCH_ANDROID_CTEST "36000;36003" 

. ~/sg_server/nohup_run/common.sh

robot_account=$1
server_id=$2
gm_level=$3
server_type=$4
account_port=$5

cd $work_path/robot_server

if [ "$robot_account" = "" ];then
    echo "账号不能为空"
    exit;
fi

if [ $server_id -gt 0 ];then
    sed -i "s/^\\(server_group_id = \\).*/\\1$server_id/" config.ini
fi

if [ $gm_level -ge 0 -a  $gm_level -le 2 ];then
    sed -i "s/^\\(gm_level = \\).*/\\1$gm_level/" config.ini
fi

if [ "$server_type" != "" ];then
    sed -i "s/^\\(server_list_type = \\).*/\\1$server_type/" config.ini
fi

if [ "$account_port" != "" ];then
    sed -i "s/^\\(account_server_port = \\).*/\\1$account_port/" config.ini
fi

./robot_server $robot_account

#重置
gm_level=1
sed -i "s/^\\(gm_level = \\).*/\\1$gm_level/" config.ini