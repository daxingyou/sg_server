#!/bin/sh

#sh r_gen_iptables.sh account 10000
#sh r_gen_iptables.sh global 20000
#sh r_gen_iptables.sh group 30000

function port_bind()
{
    local port1=$1
    local port2=$2
    
    if [ "$port2" != "" ];then
        for((port=$port1;port<=$port2;port++));  
        do   
            local rlt=`netstat -tunlp | grep $port`
            if [ "$rlt" != "" ];then
                echo -e "\033[1;31m端口[$port]被占用：$rlt\033[0m"
                exit
            fi
        done  
    else
    
        local rlt=`netstat -tunlp | grep $port1`
        if [ "$rlt" != "" ];then
            echo -e "\033[1;31m端口[$port1]被占用：$rlt\033[0m"
            exit
        fi
    fi
}

if [ "$1" = "account" ];then
    port_1=$2
    ((port_2=$port_1+1))
    ((port_3=$port_1+2))
    
    port_bind port_1
    port_bind port_2 port_3
    
    echo -e "\033[34m-A INPUT -p tcp -m tcp --dport $port_1 -j ACCEPT\033[0m"
    echo -e "\033[34m-A INPUT -p tcp -m tcp --dport $port_2:$port_3 -j ACCEPT\033[0m"
    
elif [ "$1" = "global" ];then
    port_1=$2
    ((port_2=$port_1+8))
    
    port_bind port_1 port_2
    
    echo -e "\033[34m-A INPUT -s 127.0.0.1/32 -p tcp -m tcp --dport $port_1:$port_2 -j ACCEPT\033[0m"
    
elif [ "$1" = "group" ];then
    port_1=$2
    ((port_2=$port_1+2))
    ((port_3=$port_1+4))
    ((port_4=$port_1+1))
    ((port_5=$port_1+3))
    ((port_6=$port_1+5))
    ((port_7=$port_1+15))
    ((port_8=$port_1+16))
    
    port_bind port_1
    port_bind port_2
    port_bind port_3
    port_bind port_4
    port_bind port_5
    port_bind port_6 port_7
    port_bind port_8
    
    echo -e "\033[34m-A INPUT -p tcp -m tcp --dport $port_1 -j ACCEPT\033[0m" 
    echo -e "\033[34m-A INPUT -p tcp -m tcp --dport $port_2 -j ACCEPT\033[0m"
    echo -e "\033[34m-A INPUT -p tcp -m tcp --dport $port_3 -j ACCEPT\033[0m"
    echo -e "\033[34m-A INPUT -s 127.0.0.1/32 -p tcp -m tcp --dport $port_4 -j ACCEPT\033[0m"
    echo -e "\033[34m-A INPUT -s 127.0.0.1/32 -p tcp -m tcp --dport $port_5 -j ACCEPT\033[0m"
    echo -e "\033[34m-A INPUT -s 127.0.0.1/32 -p tcp -m tcp --dport $port_6:$port_7 -j ACCEPT\033[0m"
    echo -e "\033[34m-A INPUT -s 101.81.236.201/32 -p tcp -m tcp --dport $port_8 -j ACCEPT\033[0m"
    
fi

echo "把生成的结果复制粘贴到：/etc/sysconfig/iptables"
