#!/bin/sh

# sh modify_version.sh trunk
# sh modify_version.sh 20180525

. ~/sg_server/sh/config.sh

new=$1
old=$version_name

if [ "$1" != "" ];then
    sed -i "s/^\\(version_name=\\).*/\\1\"$1\"/" $main_path/sh/my_config.sh
    
    . ./config.sh
    if [ ! -d $work_path ];then
        echo -e "\033[1;31m切换版本[$new]失败：$work_path 不存在!\033[0m"
        
        sed -i "s/^\\(version_name=\\).*/\\1\"$old\"/" $main_path/sh/my_config.sh
        
    else
        echo -e "\033[1;36m切换版本[$new]成功：$work_path\033[0m"
    fi
fi
