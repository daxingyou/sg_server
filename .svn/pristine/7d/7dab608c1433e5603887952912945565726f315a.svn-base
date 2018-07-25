#!/bin/sh

# sh checkout_svn.sh 20180525

. ~/sg_server/sh/config.sh

if [ "$1" != "" ];then
    sed -i "s/^\\(version_name=\\).*/\\1\"$1\"/" $main_path/sh/my_config.sh
    . ~/sg_server/sh/config.sh
fi

while [ true ]
do
    echo -e "\033[1;35m请确定checkout路径是否正确:\033[0m  \033[1;37m$server_svn_path --> $work_path\033[0m (Yes/n) \c"
    
    read answer
    if [ "$answer" = "Yes" ];then

        echo -e "\033[1;36m开始checkout...\033[0m"
        first=0
        if [ ! -d $work_path ];then
            first=1
        fi
        svn co $server_svn_path $work_path 1>/dev/null
        if [ $first -eq 1 ];then
            cd $work_path
            if [ "$make_type" = "debug" ];then
                sh env_install.sh 1>/dev/null
            else
                sh env_install.sh $make_type 1>/dev/null
            fi
        fi
        echo -e "\033[1;36m完成checkout(版本为$make_type)！\033[0m"

        break
    elif [ "$answer" = "n" ];then
        exit
        break
    fi
done