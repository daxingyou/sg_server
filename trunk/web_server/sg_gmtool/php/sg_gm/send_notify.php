<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./tcp_client.php');
	require_once('./proto/cmd.php');
	require_once("db_connect.php");
	require_once('./common.php');

	$data_info = $_POST['data'];
	$data = json_decode($data_info);
	//$sids_str="[\"1\",\"2\",\"3\",\"4\",\"10\",\"11\",\"12\",\"13\",\"14\",\"21\"]";
	//$data_info="{ \"type\": \"1\", \"tick\": \"1\", \"start_time\": \"1517881543\", \"end_time\": \"1517891543\", \"notice\":\"我是一条小公告\" }";
	$result = array();
    $result["status"]="failed";
	do{
		$conn = get_connect("sg_gm");
		$ret_data = array();
		//$sids=json_decode($sids_str);
		foreach( $data->sid as $sid ){
    		$result["status"]="success";
			$client = new tcp_client;	
			$ip=0;
			$port=0;
			$ret = get_ip_port_by_sid($conn, $sid, $ip, $port);
			if ($ret == false )	{
				$ret_data[$sid]=-1;
				continue;
			}

   			$ret=$client->connect($ip, $port);                          
			if ( $ret != 0 ){                                                                                 
				$ret_data[$sid]=-1;
				continue;	
   			}                                                                                                         

			$ret=$client->send($tm_send_notice_request, $data_info);
			$ret_data[$sid]=$ret;
      if ($ret == 0){
        $sql = "insert into `send_notice_log` set notice_type = '$data->type', tick = '$data->tick',start_time = '$data->start_time',end_time = '$data->end_time',notice = '$data->notice',oper_time = '$data->oper_time',server_id = '$sid',oper_name = '$data->oper_name'";
        
        $conn->query($sql);
			}
			$client->close();	
		}
		$result["result"]=$ret_data;
	}while(0);
	$result = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){ return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1])); }, json_encode($result));
	echo $result;
?>
