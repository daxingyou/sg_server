#!/bin/sh

#版本名:trunk or 20171207
version_name="trunk"

#服务器名是否显示版本 0不显示 1显示
server_name_show_version=1

#多开服务器数量
account_number=1
gate_number=1
game_number=2
cross_number=1
transfer_number=1

#关服的时候忽略的服务器列表
stop_ignore_server_list=""

#编译版本
make_type="release"

#开服时间
start_time="2016-08-01"

#######################################################################
##### golbal_server.xml #####
#######################################################################
#服务器列表类型
server_list_type="INNER_EDITOR;OUT_BRANCH_IOS_VERIFY;OUT_BRANCH_ANDROID_VERIFY"

#渠道ID 支持混渠道，用;隔开
#0：test 1：apple 2：九游
global_channel="0;1;2"

login_list_url="http:\/\/139.196.41.108:8080\/sg_update"
sdk_login_url="http:\/\/139.196.41.108:8080\/sg_sdk\/sdk_service.php"

##################################################
#account_server
#监听IP和端口起始值
account_server_listen_ip="127.0.0.1"
account_server_port_start=36200

#连接mysql:sg_account
account_db_ip="127.0.0.1"
account_db_user="root"
account_db_pwd="Sanguo1!"
account_db_name=$user_name"_"$version_name"_sg_account" 

##################################################
#除了account_server其他全局服务器
#监听端口起始值
global_server_port_start=41000

##################################################
#transfer_server
#监听IP
transfer_server_listen_ip="127.0.0.1"

##################################################
#cross_server
#监听IP
cross_server_listen_ip="127.0.0.1"

##################################################
#area_server
#监听IP
area_server_listen_ip="127.0.0.1"

#连接mysql:sg_area(global_server.xml)
area_db_ip="127.0.0.1"
area_db_user="root"
area_db_pwd="Sanguo1!"
area_db_name=$user_name"_"$version_name"_sg_area"

#连接redis
#注意：db索引不能和game_redis_db一样！
area_redis_ip="127.0.0.1"
area_redis_port=6379
area_redis_pwd="Sanguo1!"
area_redis_db=12

##################################################
#glog_server
#监听IP
glog_server_listen_ip="127.0.0.1"

#连接mysql:sg_glog
glog_db_ip="127.0.0.1"
glog_db_user="root"
glog_db_pwd="Sanguo1!"
glog_db_name=$user_name"_"$version_name"_sg_glog"

#######################################################################
##### server.xml #####
#######################################################################
#渠道ID(支持多个，用';'隔开)
#0：test 1：apple 2：九游
channel="0"

#服务器组描述，服务器名称的一部分
server_desc="审核服"

#服务器组ID，取值范围是[1, 1500]
server_id=25

#服务器组port起始值，取值范围是[1024, 65535]
server_port_start=10200

#服务器组内网IP
server_ip="127.0.0.1"

#服务器组外网IP
server_out_ip="139.196.41.108"

#连接mysql:sg_account(sql\config.py)
cnf_account_db_ip="127.0.0.1"
cnf_account_db_user="root"
cnf_account_db_pwd="Sanguo1!"
cnf_account_db_name=$account_db_name

#连接mysql:sg_area(sql\config.py)
cnf_area_db_ip="127.0.0.1"
cnf_area_db_user="root"
cnf_area_db_pwd="Sanguo1!"
cnf_area_db_name=$area_db_name

#服务器组连接mysql:sg_game
game_db_ip="127.0.0.1"
game_db_user="root"
game_db_pwd="Sanguo1!"
game_db_name=$user_name"_"$version_name"_sg_game"

#服务器组连接redis
#注意：db索引不能和area_redis_db一样！
game_redis_ip="127.0.0.1"
game_redis_port=6379
game_redis_pwd="Sanguo1!"
game_redis_db=5

#服务器组连接mysql:sg_log
log_db_ip="127.0.0.1"
log_db_user="root"
log_db_pwd="Sanguo1!"
log_db_name=$user_name"_"$version_name"_sg_log"

#服务器组连接mysql:sg_gm
#sg_gmtool\sg_gm\db_connect.php
gm_db_ip="127.0.0.1"
gm_db_user="root"
gm_db_pwd="Sanguo1!"
gm_db_name="sg_gm"

#GM等级权限 0:无权限 1:普通gm 2:超级gm
gm_level=1

#cdkey兑换的web地址
cdkey_url="http:\/\/139.196.41.108:8080\/sg_cdkey\/cdkey.php"

##################################################
#login_server
#db_server
#连接account_server(支持多个，用';'隔开)，参考account_server_port_start
connect_to_account_ip="127.0.0.1"
login_connect_to_account_port="36201"
db_connect_to_account_port="36202"

##################################################
#gate_server
#连接cross_server(支持多个，用';'隔开)，参考global_server_port_start
gate_connect_to_cross_ip="127.0.0.1"
gate_connect_to_cross_port="41004"

#默认500s 和客户端的会话超时 防止客户端sokcet连接上但不发消息占用socket数量
gate_timeout_session=500

#默认180s 心跳超时 设为0即心跳关闭
gate_timeout_pingpong=180

#默认1800s 心跳超时后等待重连的超时
gate_timeout_reconnect=1800

##################################################
#center_server
#连接transfer_server(支持多个，用';'隔开)，参考global_server_port_start
center_connect_to_transfer_ip="127.0.0.1"
center_connect_to_transfer_port="41001"

##################################################
#chat_server
#连接transfer_server(支持多个，用';'隔开)，参考global_server_port_start
chat_connect_to_transfer_ip="127.0.0.1"
chat_connect_to_transfer_port="41000"

#######################################################################
##### 其他数据库 #####
#######################################################################
#sg_statistics
ss_db_ip="127.0.0.1"
ss_db_user="root"
ss_db_pwd="Sanguo1!"
ss_db_name="sg_statistics"

#sg_bbs
bbs_db_ip="127.0.0.1"
bbs_db_user="root"
bbs_db_pwd="Sanguo1!"
bbs_db_name="shenhe_20180509_sg_bbs"

#sg_up
up_db_ip="127.0.0.1"
up_db_user="root"
up_db_pwd="Sanguo1!"
up_db_name="sg_up"

#######################################################################
##### 其他 #####
#######################################################################
#valgrind内存检查参数 0不使用 1使用
memcheck=0
mck_param="--tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes"
#mck_param="--tool=massif"

#######################################################################
##### 服务器监控 server_monitor.sh #####
#######################################################################
#跳过指定服务器的监控，""为都监控
skip_server=""
#监控间隔 秒
monitor_time=10

#######################################################################
##### 更新update.sh #####
#######################################################################
#是否可以使用[all]参数操作 0可以 1不可以
cancel_all=0

#######################################################################
##### 战斗回放日志 send_combat_log.sh #####
#######################################################################
#发送战斗回放日志的时间间隔 秒
send_time=10
#对端信息
combat_log_dst_user="root"
combat_log_dst_passwd="rootADMIN123"
combat_log_dst_ip="127.0.0.1"
combat_log_dst_dir="/igame/backup/sg_game_combat_log/"