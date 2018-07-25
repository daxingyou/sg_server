<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./tcp_client.php');

	#$sid=$_POST['sid'];
	
	$ret = 0;
	do{
		$client = new tcp_client;	
		$ret= $client->connect("47.100.130.174", "10066" );
		$ret=$client->send(3303, "");
		$client->close();	
	}while(0);
	echo $ret;
?>
