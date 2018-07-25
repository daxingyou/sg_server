<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./tcp_client.php');
	require_once('./config.php');

	$params = array();
    $post_str=file_get_contents("php://input");
    $paramsArr = explode('&',$post_str);
    foreach($paramsArr as $k=>$v)
    {
        $a = explode('=',$v);
      	$params[$a[0]] = $a[1];
    }

	$data_info=isset($params["data"]) ? $params["data"]:"";
	$sids_str=isset($params["sid"])?$params["sid"]:0;
	$sids = json_decode($sids_str);

	$result = array();
    $result["status"]="failed";
    $result["message"]->code="-1";
    $result["message"]->msg="unknow error";
	$result["message"]->sids=$sids_str;

	do {
		if ($data_info == "" ) {
    		$result["message"]->msg="data_info is empty";
			break;
		}

		$data_info=urldecode( $data_info );

		$client = new tcp_client;                                                                                 
   		$ret = $client->connect("47.100.130.174", 10016); //1服                         
   		#$ret = $client->connect("47.100.130.174", 10066);  //2服                        
   		#$ret = $client->connect("192.168.1.6", 10016);                          
		if ( $ret['code'] != 0 ){                                                                                 
    		$result["message"]->msg="socked has closed";
			break;
   		}                                                                                                         
   		$ret=$client->send(3302, $data_info);                                                                     
    	$result["message"]->code="0";
		if ($ret == 0){
    		$result["message"]->msg="success";
		}else{
    		$result["message"]->msg="socked has closed";
		}
		$client->close();  
	}while(0);	
	$result = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){ return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1])); }, json_encode($result));
	echo $result;
?>
