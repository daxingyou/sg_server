#!/bin/sh

. ~/sg_server/nohup_run/common.sh 

function check()
{
	check_server_monitor
	check_send_combat_logs
	#check_robot_monitor
	#check_performance_monitor
    
    check_account_server
    check_global_servers
    check_group_servers
}

check

echo "=============================="
echo "DONE!"
echo "=============================="