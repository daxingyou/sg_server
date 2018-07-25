<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./lib/probuff/message/pb_message.php');	
	require_once('./pb_proto_gm.php');	
	require_once('./tcp_client.php');

	$params = array();
    $post_str=file_get_contents("php://input");
    $paramsArr = explode('&',$post_str);
    foreach($paramsArr as $k=>$v)
    {
        $a = explode('=',$v);
      	$params[$a[0]] = $a[1];
    }

	$data_info=isset($params["data"]) ? $params["data"]:"";

	#$data_info="{\"sid\":[{\"id\":\"1\"}, {\"id\":\"2\"}], \"uid\":\"xxx\", \"mail_data\":{\"type\":\"xxx\", \"title\":\"xxxx\", \"content\":\"xxxx\", \"send_name\":\"xxx\", \"send_time\":\"150000\",\"end_time\":\"xxxx\", \"on_time\":\"xxx\", \"items\":\"xxx:xxx\$xxx:xxx\", \"limit_level\":\"xxxx\", \"limit_cond\":\"xxxxx\"}}";

	$result = array();
    $result["status"]="failed";
    $result["message"]->code="-1";
    $result["message"]->msg="unknow error";
	do {
		if ($data_info == "" ) {
    		$result["message"]->msg="data_info is empty";
			break;
		}

		$data_info=urldecode( $data_info );
		/*
		foreach( $mail_data->sid as $sids ) {
			foreach ( $sids as $sid){
				$client = new tcp_client;	
				$ret = $client->connect("192.168.1.178", "10016");
				if ( $ret['code'] != 0 ){
					continue;
				} 
				$client->send( 3302, $request);
				$client->close();	
			}
		}
		*/

		$client = new tcp_client;	
		$ret = $client->connect("192.168.1.178", "10016");
		if ( $ret['code'] != 0 ){
			continue;
		} 
		$tt = $client->send( 3302, $data_info);
		if( $tt == 0){
    		$result["status"]="success";
    		$result["message"]->code="0";
		    $result["message"]->msg="send_msg_ok";
		}

		$client->close();	
	}while(0);	

	$result = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){ return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1])); }, json_encode($result));
	echo $result;
?>
