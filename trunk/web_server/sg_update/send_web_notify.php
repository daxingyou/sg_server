<?php
    header('Access-Control-Allow-Origin:*');
	function save_login_data_to_file($mark_type, $json_data)                                                              
	{
		$json_data = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){ return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1])); },$json_data);
   	 	$file_name = "./server_info/notify_{$mark_type}.json";
    	$file = fopen($file_name, "w+");                                                                                  
    	fwrite($file, $json_data);
    	fclose($file);
	}  

    $mark_type = $_POST['mark_type'];
    $data      = $_POST['data'];
	$data_info=urldecode( $data);

	save_login_data_to_file( $mark_type, $data_info );

    $result = array();
    $result["status"]="success";
    $result = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){ return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1])); }, json_encode($result));
    echo $result;
?>
