<?php

header('Access-Control-Allow-Origin:*');
require_once("./db_connect.php");
require_once('./tcp_client.php');
require_once('./proto/cmd.php');
require_once('./common.php');
require_once('./proto/cmd.php');
require_once('./lib/log_debug.php');

function parseStringAndModifyXML($simple_xml, $input_string, $val){
	$array_label = explode(".", $input_string);
	//echo typeof($array_label);
	$father = $simple_xml;
	$last_label = $array_label[count($array_label) - 1];
	for($i = 1; $i < count($array_label); $i++){
		$label = $array_label[$i];
		$same_name_child = getSameNameChild($father, $label);
		if(!$same_name_child){
			$father = $father->addChild($label);
		}
		else{
			$father = $same_name_child;	
		}
		if($last_label === $label){
			$dom_node = dom_import_simplexml($father);
			$dom_node->nodeValue = $val;
			return true;
		}
	}
	return false;
}

function getSameNameChild($father, $name){
	//echo "father:".$father."\n";
	foreach($father->children() as $child){
		if($child->getName() === $name){
			return $child;
		}
	}
	return false;
}

function moveMysqlToXML($table){
	$conn = get_connect("sg_gm");
	if($conn->connect_errno)
    {
        $conn = NULL;
        die("连接错误:".$conn->connect_errno);
    }

    $ret = $conn->query("SELECT name,value FROM {$table}");
	if($ret === false){
		die(mysql_error());
	}
	
	// $xml = simplexml_load_file($xml_path);
	// if($xml === false){
	$array_path = explode("/", $xml_path);
	$xml_name = $array_path[count($array_path) - 1];
	$array_name = explode(".", $xml_name);
	$xml_name_without_postfix = $array_name[0];
	$xml = new SimpleXMLElement("<?xml version=\"1.0\" encoding=\"utf-8\"?><" . $xml_name_without_postfix . "></" . $xml_name_without_postfix . ">");	
	// }
	while($array_data = $ret->fetch_array(MYSQL_NUM)){
		$name = $array_data[0];
		$value = $array_data[1];
		//echo $value . "\n";
		if(parseStringAndModifyXML($xml, $name, $value) === false){
			echo "Not Be Inserted into XML!\nname:" . $name . "\nvalue:" . $value . "\n";
		}
	}

	$tmp_path = $xml_path . '.tmp';
	if(false === $xml->saveXML($tmp_path)){
		die("save failed");
	}
	$return_val = 0;
	$null;
	exec('xmllint -format ' . $tmp_path . ' > ' . $xml_path, $null, $return_val);
	if($return_val !== 0){
		die("xmllint failed");
	}
	mysql_close($con);
	return true;
}


function transferXMLToServer($path, $server_type, $server_id){
	$xml = simplexml_load_file($path);
	if($xml === false){
		die("open xml failed");
	}
  
	$conn = get_connect("sg_gm");
	if($conn->connect_errno)
    {
        $conn = NULL;
        die("连接错误:".$conn->connect_errno);
    }
    $client = new tcp_client;	
	$ip=0;
	$port=0;
	$ret_mysql = get_ip_port_by_sid($conn, $server_id, $ip, $port);
	if(!$ret_mysql){
		echo "server_id:" . $server_id;
		die(mysql_error());
	}
	log_debug("ip:" . $ip . "port:" . $port);
	$ret_connect_server = $client->connect($ip, $port);                          
	if ( $ret_connect_server != 0 ){                                                                                 
		die("error:连不上gmserver");
	}
	log_debug("send xml, size:" . strlen($xml->saveXML()));
	log_debug($xml->saveXML());
	$send_json_data = json_encode(array('oper_name' => $xml->saveXML(), 'server_type' => $server_type));
	$ret_send = $client->send(3313, $send_json_data);
	if($ret_send != 0){
		die('error:发送数据失败');
	}
	$client->close();
}

$server_name = $_POST['server_name'];
$server_type = $_POST['server_type'];
$server_id = $_POST['server_id'];

if(moveMysqlToXML($server_name)){
	transferXMLToServer("../../tmp/server.xml", $server_type, $server_id);
}

echo '{}';

?>