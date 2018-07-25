#!/bin/sh

. ~/sg_server/nohup_run/common.sh

if [ "$1" = "-y" ];then
	stop_group_servers
	exit
fi

if [ "$1" = "sm" -o "$1" = "cl" -o "$1" = "rm" -o "$1" = "pm" ];then
    start_stop $1
    exit
fi

start_check

echo "=============================="
echo -e "\033[1;35mstop.sh               --表示关闭login,game,gate,chat,center,db,log\033[0m"
echo -e "\033[1;35mstop.sh global        --表示关闭transfer,cross,area\033[0m"
echo -e "\033[1;35mstop.sh account       --表示关闭account,glog\033[0m"
echo -e "\033[1;35mstop.sh game,gate     --表示关闭game,gate(中间用逗号','隔开)\033[0m"

echo "=============================="
while [ true ]
do
    echo -e "\033[1;36m确定要关闭服务器分支:\033[0m \033[1;37m$work_path\033[0m(y/n)? \c"
    read answer
    if [ "$answer" = y -o "$answer" = Y ];then

        start_stop $1 $2

        start_check

        break
    elif [ "$answer" = n -o "$answer" = N ];then
        break
    fi
done

echo "=============================="
echo "DONE!"
echo "=============================="
