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
        if($conn->connect_errno)
        {
            $conn = NULL;
            die("连接错误:".$conn->connect_errno);
        }
        else
        {
            $conn->set_charset("utf8");
        }
                $sql = "insert into account_server_list set mark_type = '$data->mark_type', server_name='$data->server_name', ip='$data->ip', port='$data->port' ";
                $sql_result=$conn->query($sql);
        echo json_encode("success");                                                                                              
	}while(0);
?>
