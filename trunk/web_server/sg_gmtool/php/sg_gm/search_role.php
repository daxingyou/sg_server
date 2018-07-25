<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./tcp_client.php');
	require_once('./proto/cmd.php');
	require_once('db_connect.php');
	require_once('./common.php');

	$data_info=isset($_POST["data"])?$_POST["data"]:"";
	$data = json_decode($data_info);
 
	do{
		$conn = get_connect("sg_gm");
		if($conn->connect_errno)
    {
        $conn = NULL;
        echo("Á¬½Ó´íÎó:".$conn->connect_errno);
    }
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
      if(property_exists($data,"id"))
      {
        $ret=$client->send($tm_search_role_byid_request, $data_info);
      }else if(property_exists($data,"role_name"))
      {
        $ret=$client->send($tm_search_role_byname_request, $data_info);
      }
			$client->close();	
      echo $ret;
		}

	}while(0);
	//$result = "ok";
?>
