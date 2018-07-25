<?php
header("Content-Type:text/html;charset=utf-8");

$mark_type= $_GET['mark_type'];
$file_name="./server_info/channel_{$mark_type}.json";
$data = file_get_contents($file_name);
if ($data == false )
{
	echo "{\"status\":\"failed\", \"message\":\"NULL\"}";
}else
{
	echo "{\"status\":\"success\", \"message\":{$data}}";
}

?>
