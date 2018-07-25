<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./tcp_client.php');
	require_once('./proto/cmd.php');
	require_once("db_connect.php");
	require_once('./common.php');

	$data_info=isset($_POST["data"])?$_POST["data"]:"";
	$data = json_decode($data_info);
 
	do{
		$conn = get_connect("sg_gm");
		$ret_data=array();
		foreach( $data->sid as $sid ){
			$client = new tcp_client;	
			$ip=0;
			$port=0;
			$ret=get_ip_port_by_sid($conn, $sid, $ip, $port);
			if ($ret == false )	{
				$ret_data[$sid]=-1;
				continue;
			}

   			$ret=$client->connect($ip, $port);                          
			if ( $ret != 0 ){                                                                                 
				$ret_data[$sid]=-1;
				continue;	
   			}                                                                                                         
   
      $ret=$client->send($tm_manage_role_request, $data_info);
      $reply_data = json_decode($ret);
      $role_list = split(";",$reply_data->role_did);
      foreach( $role_list as $role ){
			  $role_data = split("-",$role);
        $role_did = $role_data[0];
        $expired_time = $role_data[1];
        $sql = "insert into manage_role_log set id = '$role_did', manage_type='$reply_data->manage_type', offset_time='$reply_data->offset_time', oper_time='$reply_data->oper_time', server_id='$reply_data->server_id', reason='$reply_data->reason', expired_time='$expired_time', oper_name='$reply_data->oper_name'";
        //echo $sql;
        $conn->query($sql);
      }
      
			$client->close();	
      echo $ret;
		}

	}while(0);
	$result = "ok";
?>
