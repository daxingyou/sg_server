<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./tcp_client.php');
	require_once('./proto/cmd.php');
	require_once("db_connect.php");
	require_once('./common.php');

	$params = array();

	$data_info=isset($_POST["data"])?$_POST["data"]:"";
	$data = json_decode($data_info);
	//$sids_str=isset($_POST["sid"])?$_POST["sid"]:0;
	//$sids = json_decode($sids_str);

	$result = array();
    $result["status"]="failed";
	do {
		$conn = get_connect("sg_gm");
		if ($data_info == "" ) {
    		$result["message"]->msg="data_info is empty";
			break;
		}
		$data_info=urldecode( $data_info );

		$ret_data=array();
		foreach( $data->sid as $sid ){
    		$result["status"]="success";
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

   			$ret=$client->send($tm_send_mail_request, $data_info);
			if ($ret == 0){
				$ret_data[$sid]=0;
		        $mail_data = $data->mail_data;
		        $sql = "insert into `send_mail_log` set id = '$data->id', mail_type = '$mail_data->type',title = '$mail_data->title',content = '$mail_data->content',send_name = '$mail_data->send_name',send_time = '$mail_data->send_time',end_time = '$mail_data->end_time',on_time = '$mail_data->on_time',items = '$mail_data->items',limit_level = '$mail_data->limit_level',limit_cond = '$mail_data->limit_cond',server_id = '$sid',oper_time = '$data->oper_time',oper_name = '$data->oper_name'";
		        $conn->query($sql);
			}else{
				$ret_data[$sid]=-1;
			}
			$client->close();  
		}
		$result["result"]=$ret_data;
	}while(0);	
	$result = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){ return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1])); }, json_encode($result));
	echo $result;
?>
