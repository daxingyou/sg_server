<?php
header("Content-Type:text/html;charset=utf-8");

$server_list_type=$_GET['server_list_type'];
$file_name="./server_info/compatile_version_{$server_list_type}.json";
$data = file_get_contents($file_name);
if ($data == false )
{
    echo "{\"status\":\"failed\", \"message\":\"NULL\"}";
}else
{
    echo "{\"status\":\"success\", \"message\":{$data}}";
}

?>
