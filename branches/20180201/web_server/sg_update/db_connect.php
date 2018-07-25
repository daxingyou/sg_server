<?php

$conn = new mysqli('192.168.1.178','root','Sanguo1!','huangyang_trunk_sg_update');
if($conn->connect_errno)
{
	$conn = NULL;
	die("连接错误:".$conn->connect_errno);
}
else
{
	$conn->set_charset("utf8");
}

?>
