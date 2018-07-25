<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./tcp_client.php');
	require_once('./proto/cmd.php');
	require_once("db_connect.php");
	require_once('./common.php');
	require_once('./lib/log_debug.php');

	$data_info = $_POST['data'];
	$data = json_decode($data_info);
	
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
			log_debug("ip:" . $ip);
			log_debug("port:" . $port);
   			$ret=$client->connect($ip, $port);                          
			if ( $ret != 0 ){                                                                                 
				$ret_data[$sid]=-1;
				continue;	
   			}                                                                                                         

			$ret=$client->send($tm_reload_config_request, $data_info);
			$ret_data[$sid]=$ret;

			$client->close();	
		}
		$result["result"]=$ret_data;
	}while(0);
	$result = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){ return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1])); }, json_encode($result));
	echo $result;
?>
