<?php
// EXECUTE test_new.php first 

require_once('./probuff/message/pb_message.php');
require_once('pb_proto_gamecmd.php');
require_once('tp_netclient.php');

//set_time_limit(0);

// 获取数据
$g_notice = $_POST['notice'];
$g_server = $_POST['combo_zone'];

// 网络连接
$ip = "192.168.1.210";
$port = "51701";


$nettcp = new TP_NetClient_TCP;

if($nettcp->connect($ip,$port,60) == false)
{
	echo "ERROR!";
	echo $nettcp->code;
	echo $nettcp->msg;
	return;
}

// 定义消息结构
$g_header = new sg_msg_head;

$g_header->set_MsgID(10004);
$g_header->set_KeyLen(0);
$g_header->set_SessionKey("");

$g_admin = new PandaGMNoticeReq;
$g_admin_ack = new PandaGMNoticeResp;

$g_admin->set_content($g_notice);

$send_result = $nettcp->probuffsend($g_header, $g_admin);
if($send_result != false)
{
	$recv_buff = $nettcp->recv();
	$g_admin_ack->ParseFromString($recv_buff);		
	
	if($g_admin_ack->MsgResult() == 0)
	{
		echo "成功 ！！！";
	}
	else
	{
		echo "失败 ！！！";
	}
}
$nettcp->close();
?>