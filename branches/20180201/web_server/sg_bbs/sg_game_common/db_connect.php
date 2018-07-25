<?php


$conn = new mysqli('192.168.1.212','root','Sanguo1!','sg_bbs');
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
