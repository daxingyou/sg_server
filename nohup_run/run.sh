#!/bin/sh

. ~/sg_server/nohup_run/common.sh

if [ "$1" = "-y" ];then
	run_group_servers
	exit
fi


if [ "$1" = "sm" -o "$1" = "cl" -o "$1" = "rm" -o "$1" = "pm" ];then
    start_run $1
    exit
fi

start_check

echo "=============================="
echo -e "\033[1;35mrun.sh               --表示启动log,db,chat,center,game,gate,login\033[0m"
echo -e "\033[1;35mrun.sh global        --表示启动area,transfer,cross\033[0m"
echo -e "\033[1;35mrun.sh account       --表示启动account,glog\033[0m"
echo -e "\033[1;35mrun.sh game,gate     --表示启动game,gate(中间用逗号','隔开)\033[0m"

echo "=============================="
while [ true ]
do
    echo -e "\033[1;36m确定要运行服务器分支:\033[0m \033[1;37m$work_path\033[0m(y/n)? \c"
    read answer
    if [ "$answer" = y ];then

        start_run $1 $2

		start_check

        break
    elif [ "$answer" = n ];then
        break
    fi
done

echo "=============================="
echo "DONE!"
echo "=============================="
