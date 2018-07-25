<?php

require_once("../sg_gm/db_connect.php");
require_once("../sg_gm/lib/log_debug.php");
require_once("../util/ssh2_obj.php");

$msg_id = $_POST['msg_id'];
$msg_data = json_decode($_POST['msg_data'], true);

function get_ssh_need_info($db_con, $server_id){
	$sql = "SELECT gm_server_list.server_id,server_name,server_ssh.ip,ssh_port,username,passwd,location FROM gm_server_list LEFT OUTER JOIN server_ssh ON gm_server_list.server_id=server_ssh.server_id WHERE gm_server_list.server_id={$server_id}";
	$result = $db_con->query($sql);
	if($result == false){
		throw new Exception("QUERY FAILED:" . $sql . $db_con->error);
	}
	$data = mysqli_fetch_array($result, MYSQLI_ASSOC);
	if($data == false){
		throw new Exception("NO SUCH DATA, SERVER_ID:" . $server_id);
	}
	return $data;
}

function update_last_server_action($db_con, $server_id, $action_id){
	$sql = "UPDATE server_action SET last_server_action={$action_id} WHERE server_id={$server_id};";
	$db_con->query($sql);
}

switch($msg_id){
	case 1:
	{
		$db_con = get_connect('sg_gm');
		if($db_con->connect_errno){
			throw new Exception("NO DB CONNECTION " . $db_con->connect_error);
		}
		$sql = "SELECT gm_server_list.server_id,server_name,server_ssh.ip,username,location,last_server_action FROM gm_server_list LEFT OUTER JOIN server_ssh ON gm_server_list.server_id=server_ssh.server_id LEFT OUTER JOIN server_action ON gm_server_list.server_id=server_action.server_id WHERE gm_server_list.server_id >= {$msg_data['start_id']} AND gm_server_list.server_id <= {$msg_data['end_id']};";
		$result = $db_con->query($sql);
		$result_array = array();
		while($array_data = mysqli_fetch_array($result, MYSQLI_ASSOC)){
			array_push($result_array, $array_data);
		}
		echo json_encode($result_array);
	}
	break;
	case 2:
	{
		log_debug(print_r($msg_data, true));
		$db_con = get_connect('sg_gm');
		$res = array();
		foreach($msg_data['server'] as $server_id=>$choosen){
			if($choosen == 1){
				//添加日志
				$now = time();
				$sql = "INSERT INTO server_operation_log SET server_id={$server_id},operation_type={$msg_data['oper']},operation_user='{$msg_data['user']}',operation_date=FROM_UNIXTIME({$now});";
				$result = $db_con->query($sql);
				if($result == false){
					throw new Exception("QUERY FAILED:" . $sql . $db_con->error);
				}
				//进行操作
				$data = get_ssh_need_info($db_con, $server_id);
				log_debug(print_r($data, true));
				$ip = $data['ip'];
				$port = $data['ssh_port'];
				$user = $data['username'];
				$passwd = $data['passwd'];
				$location = $data['location'];
				$ssh2_obj = new SSH2Obj($ip, $port, $user, $passwd);
				switch($msg_data['oper']){
					case 1:
						update_last_server_action($db_con, $server_id, 3);
						$cmd = 'sh ' . $location . '/run.sh -y';
						$result_string = $ssh2_obj->sync_operation($cmd);
						log_debug($result_string);
						$res[$server_id] = $result_string;
						update_last_server_action($db_con, $server_id, 4);
						break;
					case 2:
						update_last_server_action($db_con, $server_id, 1);
						$cmd = 'sh ' . $location . '/stop.sh -y';
						$result_string = $ssh2_obj->sync_operation($cmd);
						log_debug($result_string);
						$res[$server_id] = $result_string;
						update_last_server_action($db_con, $server_id, 2);
						break;
					case 3:

						break;
				}
			}
		}
		echo json_encode($res);
	}
	break;
	case 3:
	{
		log_debug($msg_data);
		$db_con = get_connect('sg_gm');
		$data = get_ssh_need_info($db_con, $msg_data);
		log_debug(print_r($data, true));
		$ip = $data['ip'];
		$port = $data['ssh_port'];
		$user = $data['username'];
		$passwd = $data['passwd'];
		$location = $data['location'];
		log_debug(print_r($data, true));
		$ssh2_obj = new SSH2Obj($ip, $port, $user, $passwd);
		$cmd = 'sh ' . $location . '/check.sh';
		echo json_encode($ssh2_obj->sync_operation($cmd) . $msg_data);
	}
	break;
	case 4:
	{
		echo json_encode(1);
	}
	break;
	case 5:
	{
		// {
		// 	server_group : data.field.oper_choose_server_group,
		//  server_group_data : server_group_data,
		// 	server_id : data.field.oper_choose_server,
		// 	query_type : data.field.query_choice,
		// 	start_date : period_select['start'],
		// 	end_date : period_select['end'],
		// }
		// $start_time = mktime($msg_data['start_date']['hours'], $msg_data['start_date']['minutes'], $msg_data['start_date']['seconds'], $msg_data['start_date']['month'], $msg_data['start_date']['date'], $msg_data['start_date']['year']);
		// $end_time = mktime($msg_data['end_date']['hours'], $msg_data['end_date']['minutes'], $msg_data['end_date']['seconds'], $msg_data['end_date']['month'], $msg_data['end_date']['date'], $msg_data['end_date']['year']);

		$pieces = explode(' ~ ', $msg_data['date']);
		if(count($pieces) != 2){
			throw new Exception('WRONG DATE FORMAT, size:' . count($pieces));
		}
		$start_date = $pieces[0];
		$end_date = $pieces[1];
		$sql = "";
		if($msg_data['query_type'] == 'single'){
			$sql = "SELECT server_operation_log.server_id,operation_type,operation_user,operation_date,server_name FROM server_operation_log LEFT OUTER JOIN gm_server_list ON server_operation_log.server_id=gm_server_list.server_id WHERE server_operation_log.server_id={$msg_data['server_id']} AND UNIX_TIMESTAMP(operation_date)>=UNIX_TIMESTAMP('{$start_date}') AND UNIX_TIMESTAMP(operation_date)<=UNIX_TIMESTAMP('{$end_date}')";
		}
		elseif($msg_data['query_type'] == 'group'){
			$group_name = $msg_data['server_group'];
			$group_data = $msg_data['server_group_data'];
			$start_id = $group_data[$group_name]['group_start_id'];
			$end_id = $group_data[$group_name]['group_end_id'];
			$sql = "SELECT server_operation_log.server_id,operation_type,operation_user,operation_date,server_name FROM server_operation_log LEFT OUTER JOIN gm_server_list ON server_operation_log.server_id=gm_server_list.server_id WHERE server_operation_log.server_id>={$start_id} AND server_operation_log.server_id<={$end_id} AND UNIX_TIMESTAMP(operation_date)>=UNIX_TIMESTAMP('{$start_date}') AND UNIX_TIMESTAMP(operation_date)<=UNIX_TIMESTAMP('{$end_date}')";
		}
		else{
			return;
		}
		$db_con = get_connect('sg_gm');
		$result = $db_con->query($sql);
		if($result == false){
			throw new Exception("QUERY FAILED:" . $sql . $db_con->error);
		}
		$result_array = array();
		while($array_data = mysqli_fetch_array($result, MYSQLI_ASSOC)){
			array_push($result_array, $array_data);
		}
		$res['data'] = $result_array;
		$res['group'] = $msg_data['server_group'];
		echo json_encode($res);
	}
	break;
	case 6:
	{
		$db_con = get_connect('sg_gm');
		if($db_con->connect_errno){
			throw new Exception("NO DB CONNECTION " . $db_con->connect_error);
		}
		$sql = "SELECT server_id,server_name FROM gm_server_list;";
		$result = $db_con->query($sql);
		if($result == false){
			throw new Exception("QUERY FAILED:" . $sql . $db_con->error);
		}
		$result_array = array();
		while($array_data = mysqli_fetch_array($result, MYSQLI_ASSOC)){
			array_push($result_array, $array_data);
		}
		echo json_encode($result_array);
	}
	break;
	case 7:
	{
		$date = $msg_data['date'];
		$db_con = get_connect('sg_log', 1);
		if($db_con->connect_errno){
			throw new Exception("NO DB CONNECTION " . $db_con->connect_error);
		}
		$sql = "SELECT * FROM daily_statistics_log WHERE date='{$date}'";
		$result = $db_con->query($sql);
		if($result == false){
			throw new Exception("QUERY FAILED:" . $sql . $db_con->error);
		}
		$result_array = array();
		while($array_data = mysqli_fetch_array($result, MYSQLI_ASSOC)){
			array_push($result_array, $array_data);
		}
		echo json_encode($result_array);
	}
	default:
		return;
}

?>