<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./tcp_client.php');
	require_once('./proto/cmd.php');
    	require_once('./common.php');
    	require_once('db_connect.php');

	$data_info=isset($_POST["data"])?$_POST["data"]:"";
	$data = json_decode($data_info);
  
	do{
        $conn = get_connect("sg_up");
        //   $conn = new mysqli('139.196.41.108','wsy','Wsy1985!','sg_up');
        if($conn->connect_errno)
        {
            $conn = NULL;
            die("连接错误:".$conn->connect_errno);
        }
        else
        {
            $conn->set_charset("utf8");
        }

        if(property_exists($data,"res_url")){
            $sql = "update res_info set res_url='$data->res_url' where id = '$data->id' and mark_type='$data->mark_type'";
        }
        else if(property_exists($data,"bbs_url")) {
            $sql = "update bbs_info set bbs_url='$data->bbs_url' where id = '$data->id' and mark_type='$data->mark_type'";
        }
        else if(property_exists($data,"notice_url")) {
            $sql = "update notice_info set notice_url='$data->notice_url' where id = '$data->id' and mark_type='$data->mark_type'";
        }
        else if(property_exists($data,"plist_url")) {
            $sql = "update plist_info set plist_url='$data->plist_url' where id = '$data->id' and mark_type='$data->mark_type'";
        }
        else if(property_exists($data,"combat_url")) {
            $sql = "update combat_view set combat_url='$data->combat_url' where id = '$data->id' and mark_type='$data->mark_type'";
        }
                
        $sql_result=$conn->query($sql);
        echo json_encode("success");                                                                                               
	}while(0);
?>
