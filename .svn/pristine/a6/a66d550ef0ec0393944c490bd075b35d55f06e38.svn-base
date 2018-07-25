<?php

function get_ip_port_by_sid($conn, $sid, &$ip, &$port)
{
	$sql = "select ip, port from gm_server_list where server_id = '$sid'";
	$sql_result=$conn->query($sql);	
	if ( $sql_result == false ){	
		return false;
	}
	
	if( $row=$sql_result->fetch_array() ){	
		$ip=$row["ip"];
		$port=$row["port"];
		return true;
	}

	return false;
}

function get_log_db_by_sid($conn, $sid, &$log_host, &$log_db, &$log_user, &$log_pwd)
{
	$sql = "select log_host, log_db, log_user, log_pwd from gm_server_list where server_id = '$sid'";
	$sql_result=$conn->query($sql);	
	if ( $sql_result == false ){	
		return false;
	}
	
	if( $row=$sql_result->fetch_array() ){	
		$log_host=$row["log_host"];
		$log_db=$row["log_db"];
    $log_user=$row["log_user"];
    $log_pwd=$row["log_pwd"];
		return true;
	}

	return false;
}
?>
