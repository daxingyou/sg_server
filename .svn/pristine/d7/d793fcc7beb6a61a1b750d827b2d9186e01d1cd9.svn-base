<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./tcp_client.php');
	require_once('./proto/cmd.php');
    require_once('./common.php');
    require_once("db_connect.php");

	$data_info=isset($_POST["data"])?$_POST["data"]:"";
	$data = json_decode($data_info);
  
	do{
        $conn = get_connect("sg_up");
        //$conn = new mysqli('139.196.41.108','wsy','Wsy1985!','sg_up');
        if($conn->connect_errno)
        {
            $conn = NULL;
            die("连接错误:".$conn->connect_errno);
        }
        else
        {
            $conn->set_charset("utf8");
        }
                $sql = "update account_server_list set server_name = '$data->server_name', ip='$data->ip', port='$data->port' where id = '$data->id' and mark_type = '$data->mark_type'";
                $sql_result=$conn->query($sql);
        echo json_encode("success");                                                                                               
	}while(0);
?>
