<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./tcp_client.php');
	require_once('./proto/cmd.php');
	require_once('db_connect.php');
	require_once('./common.php');

	//$data_info="{\"sid\": [\"7\"], \"role_did\": \"1836756\", \"manage_type\": \"0\", \"start_time\": \"1522393256\", \"end_time\": \"1522393262\" }";
	$data_info=isset($_POST["data"])?$_POST["data"]:"";
	$data = json_decode($data_info);
  
	do{
		$conn = get_connect("sg_gm");
		if($conn->connect_errno)
        {
            $conn = NULL;
            echo("Á¬½Ó´íÎó:".$conn->connect_errno);
            continue;
        }
        else
        {
            $conn->set_charset("utf8");
        }
		$ret_data=array();
		foreach( $data->sid as $sid ){
			  $sql = "SELECT  *  FROM `manage_role` where `server_id` = '$sid'";
		      if(property_exists($data,"id"))
		      {
		        $sql .= "and id = '$data->id'";
		      }
		      if(property_exists($data,"manage_type"))
		      {
		        $sql .= "and manage_type = '$data->manage_type'";
		      }
		      if(property_exists($data,"start_time"))
		      {
		        $sql .= "and oper_time >= '$data->start_time'";
		      }
		      if(property_exists($data,"end_time"))
		      {
		        $sql .= "and oper_time <= '$data->end_time'";
		      }

			  $sql_result=$conn->query($sql);
		      $sendlist = array();
		      $i = 0;
					while($Res = $sql_result->fetch_array())
		      {
		        $row = array( 'id'=> $Res['id'],'server_id'=> $Res['server_id'],'manage_type'=> $Res['manage_type'],'offset_time'=> $Res['offset_time'],'oper_time'=> $Res['oper_time'],'reason'=> $Res['reason'],'expired_time'=> $Res['expired_time'],'oper_name'=> $Res['oper_name']);
		        $sendlist[$i] = $row;
		        $i++;
		      }
					echo json_encode($sendlist);    
					$sql_result->close();                                                                                           
		}
        exit(0);

	}while(0);
?>
