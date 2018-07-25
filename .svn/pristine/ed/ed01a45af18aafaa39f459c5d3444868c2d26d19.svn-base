<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./tcp_client.php');
	require_once('./proto/cmd.php');
	require_once("db_connect.php");
	require_once('./common.php');

	//$sids_str=$_POST['sid'];
	//$data_info=$_POST['data'];
  $sids_str="[\"7\"]";
	$data_info="{ \"role_did\": \"1836756;1836757\", \"manage_type\": \"0\", \"offset_time\": \"300\", \"reason\": \"看你不爽\" }";

	//$data_info=isset($_POST["data"])?$_POST["data"]:"";
	$data = json_decode($data_info);
  $sids = json_decode($sids_str);
  
	do{
		$conn = get_connect("sg_gm");
		$ret_data=array();
		foreach( $sids as $sid ){
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

			$ret=$client->send($tm_manage_role_byid_request, $data_info);
			$client->close();	
      echo $ret;
		}

	}while(0);
	$result = "ok";
?>
