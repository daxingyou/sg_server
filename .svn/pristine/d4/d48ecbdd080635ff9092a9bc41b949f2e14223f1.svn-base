<?php

header("Content-Type:text/html;charset=utf-8");
require("db_connect.php");
require("sg_bbs_log.php");
session_start();

init_data_from_db($conn);

function save_channel_data_to_file($mark_type, $json_data)
{
	$json_data = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){ return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1])); },$json_data);
	$file_name = "./server_info/channel_{$mark_type}.json";
	$file = fopen($file_name, "w+");
	fwrite($file, $json_data);
	fclose($file);
}

function save_login_data_to_file($mark_type, $json_data)
{
	$json_data = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){ return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1])); },$json_data);
	$file_name = "./server_info/login_{$mark_type}.json";
	$file = fopen($file_name, "w+");
	fwrite($file, $json_data);
	fclose($file);
}

function save_compatile_version_to_file($mark_type, $json_data)
{
	$json_data = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){ return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1])); },$json_data);
	$file_name = "./server_info/compatile_version_{$mark_type}.json";
	$file = fopen($file_name, "w+");
	fwrite($file, $json_data);
	fclose($file);
}

function save_notice_data_to_file($channel_id, $json_data)
{
	$json_data = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){ return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1])); },$json_data);
	$file_name = "./server_info/notice_{$channel_id}.json";
	$file = fopen($file_name, "w+");
	fwrite($file, $json_data);
	fclose($file);
}


//读取数据库
function init_data_from_db($conn)
{
	$sql="select mark_type, version, platform_name, platform_type from platform_info";
	$result=$conn->query($sql);
	if ($result != false )
	{
		while( $row=$result->fetch_array() ){
			$mark_type=$row['mark_type'];	
			$server_info->version=$row['version'];
			$server_info->name=$row['platform_name'];
			$server_info->type=$row['platform_type'];

			$version_no=explode('.', $server_info->version);
			#查询res
			$server_info->res_url_list=load_res_data($conn, $mark_type, $version_no[2], $server_info->type);
			#查询plist
			$server_info->plist_url_list=load_plist_data($conn, $mark_type, $version_no[2], $server_info->type);
			#查询notice
			$server_info->notice_url_list=load_notice_data($conn, $mark_type);
			#查询account_server_lst	
			$server_info->server_list=load_account_server_list_data($conn, $mark_type);
			#查询bbs_list
			$server_info->bbs_list=load_bbs_list_data($conn, $mark_type);
			save_channel_data_to_file($mark_type, json_encode($server_info));


			#兼容版本信息
			$comp_version->comp_version_list =load_server_compatible_version_data($conn, $mark_type);
			save_compatile_version_to_file($mark_type,json_encode($comp_version));

			#写入公告信息,直接走文本
			#$notice_info=load_notice_info_data($conn, $channel_id);
			#save_notice_data_to_file($channel_id, json_encode($notice_info));
		}		
		$ret->status="success";
		$ret->message="ok";		
		echo json_encode($ret);
	}else
	{
		write_log("load_platfrom_info $sql error:$result->error $result->errno");	
		$ret->status="failed";
		$ret->message="load_platform_faild";
		echo json_encode($ret);
	}
	
	#查询登录服list
	$sql = "select distinct mark_type from login_server_list";
	$result=$conn->query($sql);
	if ($result != false ){
		$count = 0;
		while( $row=$result->fetch_array() )
		{
			$mark_type=$row['mark_type'];
			#查询登录服list
			$login_server_info->server_list=load_login_server_info_data($conn, $mark_type);		
			save_login_data_to_file($mark_type, json_encode($login_server_info));
		}
	}
}

function load_res_data($conn, $mark_type, $version, $platform_type)
{
	$sql = "select res_url from res_info where mark_type = '$mark_type'";
	$result=$conn->query($sql);
	if ($result != false ){
		$res_url_list = array();
		$count = 0;
		while( $row=$result->fetch_array() ){
			$res_url=str_replace("#VERSION_NO#",$version,$row['res_url']);
			$res_url_list[$count]=$res_url;
			$count = $count + 1;
		}
		return $res_url_list;
	}else{
		write_log("load_res_data $sql error:$result->error $result->errno");	
	}
}

function load_notice_data($conn, $mark_type)
{
	$sql = "select notice_url from notice_info where mark_type = '$mark_type'";
	$result=$conn->query($sql);
	if ($result != false ){
		$notice_url_list = array();
		$count = 0;
		while( $row=$result->fetch_array() ){
			$notice_url_list[$count]=$row['notice_url'];
			$count = $count + 1;
		}
		return $notice_url_list;
	}else{
		write_log("load_notice_data $sql error:$result->error $result->errno");	
	}
}

function load_plist_data($conn, $mark_type, $version, $platform_type)
{
	$sql = "select plist_url from plist_info where mark_type = '$mark_type'";
	$result=$conn->query($sql);
	if ($result != false ){
		$plist_url=array();
		$count = 0;
		while( $row=$result->fetch_array() ){
			$plist_url=str_replace("#VERSION_NO#",$version, $row['plist_url']);
			$plist_url_list[$count]=$plist_url;
			$count = $count + 1;
		}
		return $plist_url_list;
	}else{
		write_log("load_plist_data $sql error:$result->error $result->errno");	
	}
}

function load_account_server_list_data($conn, $mark_type)
{
	$sql = "select ip, port from account_server_list where mark_type = '$mark_type' ";
	$result=$conn->query($sql);
	if ($result != false ){
		$server_list=array();
		$count = 0;
		while( $row=$result->fetch_array() ){
			$server_list[$count]->ip=$row['ip'];
			$server_list[$count]->port=$row['port'];
			$count = $count + 1;
		}
		return $server_list;
	}else{
		write_log("load_account_server_list_data $sql error:$result->error $result->errno");	
	}
}

#
function load_login_server_info_data($conn, $mark_type)
{
	$sql = "select server_id, status, server_name, label, mark_type from login_server_list where mark_type = '$mark_type' and status != '0'";
	$result=$conn->query($sql);
	if ($result != false ){
		$login_server_list=array();
		$count = 0;
		while( $row=$result->fetch_array() ){
       		$login_server_list[$count]->server_id=$row['server_id'];
           	$login_server_list[$count]->status=$row['status'];
           	$login_server_list[$count]->name=$row['server_name'];
           	$login_server_list[$count]->label=$row['label'];
           	$login_server_list[$count]->server_list_type=$row['mark_type'];
		   	$count = $count + 1;
       	}
       	return $login_server_list;
   	}else{
		write_log("load_login_server_info_data $sql error:$result->error $result->errno");	
	}
}

function load_server_compatible_version_data($conn, $mark_type)
{
	$sql = "select version from compatible_version_info where mark_type = '$mark_type'";
   	$result=$conn->query($sql);
   	if ($result != false ){
		$version_list = array();
		$count = 0;
       	while( $row=$result->fetch_array() ){
       		$version_list[$count]=$row['version'];
           	$count = $count + 1;
      	}
       	return $version_list;
   	}else{
		write_log("load_server_compatible_version_data $sql error:$result->error $result->errno");	
	}
}

function load_bbs_list_data($conn, $mark_type)
{
	$sql = "select bbs_url from bbs_info where mark_type = '$mark_type' ";
   	$result=$conn->query($sql);
   	if ($result != false ){
		$bbs_url_list=array();	
		$count = 0;
       	while( $row=$result->fetch_array() ){
        	$bbs_url_list[$count]=$row['bbs_url'];
           	$count = $count + 1;
       	}
       	return $bbs_url_list;
   	}else{
		write_log("load_bbs_list_data $sql error:$result->error $result->errno");	
	}
}

function load_notice_info_data($conn, $mark_type)
{
	$sql = "select notice from notice_data where mark_type = '$mark_type'";
   	$result=$conn->query($sql);
   	if ($result != false ){
       $count = 0;
       while( $row=$result->fetch_array() ){
           $notice_data=$row['notice'];
       }
       return $notice_data;
   }else {
		write_log("load_notice_info_data $sql error:$result->error $result->errno");	
	}
}

?>

