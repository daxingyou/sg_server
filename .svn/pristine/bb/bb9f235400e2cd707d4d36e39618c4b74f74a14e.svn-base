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

                if ($data->mark_type == "") {
                        break;
                }
                
                if ($data->bbs_url != "") {
                        $sql = "insert into bbs_info set mark_type = '$data->mark_type', bbs_url='$data->bbs_url'";
                        $sql_result=$conn->query($sql);
                }
                        
                if ($data->plist_url != "") {
                        $sql = "insert into plist_info set mark_type = '$data->mark_type', plist_url='$data->plist_url'";
                        $sql_result=$conn->query($sql);
                }

                if ($data->notice_url != "") {
                        $sql = "insert into notice_info set mark_type = '$data->mark_type', notice_url='$data->notice_url' ";
                        $sql_result=$conn->query($sql);
                }

                if ($data->res_url != "") {
                        $sql = "insert into res_info set mark_type = '$data->mark_type', res_url='$data->res_url'";
                        $sql_result=$conn->query($sql);
                }

                if ($data->compatible_version != "") {
                        $sql = "insert into compatible_version_info set mark_type = '$data->mark_type', version='$data->compatible_version'";
                        $sql_result=$conn->query($sql);
                }

                if ($data->combat_url != "") {
                        $sql = "insert into combat_view set mark_type = '$data->mark_type', combat_url='$data->combat_url'";
                        $sql_result=$conn->query($sql);
                }
                echo json_encode("success");                                                                                               
	}while(0);
?>
