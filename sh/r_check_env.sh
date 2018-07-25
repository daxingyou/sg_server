#!/bin/sh

# sh r_check_env.sh

function check()
{
    echo "==================================================="
    local rlt=`service iptables status`
    if [[ "$rlt" =~ "iptables: Firewall is not running" ]];then
        echo -e "\033[1;31mFirewall未开启！\033[0m"
    else
        echo -e "\033[1;36mFirewall已开启！\033[0m"
        echo -e "\033[1;36m检查/etc/sysconfig/iptables，相应服务器端口是否已设置\033[0m"
    fi
    
    echo "==================================================="
    local rlt=`service mysqld status`
    if [[ "$rlt" =~ "mysqld is stopped" ]];then
        echo -e "\033[1;31mmysql未开启！\033[0m"
    else
        echo -e "\033[1;36mmysql已开启！\033[0m"
    fi
    
    echo "==================================================="
    local rlt=`ps axu | grep -v 'grep' | grep 'redis-server'`
    if [ "$rlt" = "" ];then
        echo -e "\033[1;31mredis未开启！\033[0m"
    else
        echo -e "\033[1;36mredis已开启！\033[0m"
    fi
    
    echo "==================================================="
    local rlt=`ps axu | grep -v 'grep' | grep 'nginx'`
    if [ "$rlt" = "" ];then  
        echo -e "\033[1;31mnginx未开启！\033[0m"
    else
        echo -e "\033[1;36mnginx已开启！\033[0m"
    fi
    
    echo "==================================================="
    local rlt=`service irqbalance status`
    if [ "$rlt" = "" ];then
        echo -e "\033[1;31mCPU优化服务irqbalance未开启！\033[0m"
    else
        echo -e "\033[1;36mCPU优化服务irqbalance已开启！\033[0m"
    fi
    
    echo "==================================================="
    local rlt=`service crond status`
    if [ "$rlt" = "" ];then
        echo -e "\033[1;31mcrond未开启！\033[0m"
    else
        echo -e "\033[1;36mcrond已开启！\033[0m"
        crontab -l
    fi   
}

check