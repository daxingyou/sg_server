<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./tcp_client.php');
	require_once('./proto/cmd.php');
	require_once("db_connect.php");
	require_once('./common.php');

	$data_info=isset($_POST["data"])?$_POST["data"]:"";
	$data = json_decode($data_info);
   
	do{
		$conn_log = get_connect("sg_log",$data->db_type);
		//$conn_log = new mysqli($data->log_host,$data->log_user,$data->log_pwd,$data->log_db);
        if($conn_log->connect_errno)
        {
            $conn_log = NULL;
            echo("连接错误:".$conn_log->connect_errno);
            continue;
        }
        else
        {
            $conn_log->set_charset("utf8");
        }
		  $sql = "SELECT  *  FROM `rmb_log` WHERE op_type = 2";
		     if(property_exists($data,"role_uid"))
		     {
             	$sql .= "and role_uid = '$data->id'";
		     }
		     if(property_exists($data,"start_time"))
		     {
             	$sql .= "and create_time >= '$data->start_time'";
		     }
		     if(property_exists($data,"end_time"))
		     {   
             	$sql .= "and create_time >= '$data->end_time'";          
		     }

		    $sql_result=$conn_log->query($sql);
		     $sendlist = array();
		     $i = 0;
		while($Res = $sql_result->fetch_array())
		   {
		       $row = array( 'id'=> $Res['role_uid'],'up_num'=> $Res['up_num'],'source_type_desc'=> $Res['source_type_desc'],'role_level'=> $Res['role_level'],'create_time'=> $Res['create_time']);
		       $sendlist[$i] = $row;
		       $i++;
		    }
		    echo json_encode($sendlist);
		$sql_result->close();
        exit(0);

	}while(0);
?>
