<?php
	header('Access-Control-Allow-Origin:*');
	require_once('./tcp_client.php');
	require_once('./proto/cmd.php');
	require_once("db_connect.php");
	require_once('./common.php');

	$data_info=isset($_POST["data"])?$_POST["data"]:"";
	$data = json_decode($data_info);
  
	do{
		$conn = get_connect("sg_gm");
		$ret_data=array();
		foreach( $data->sid as $sid ){
			  $sql = "SELECT  *  FROM `send_notice_log` where `server_id` = '$sid'";
		      if(property_exists($data,"notice_type"))
		      {
		        $sql .= "and notice_type = '$data->notice_type'";
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
		        $row = array( 'notice_type'=> $Res['notice_type'],'tick'=> $Res['tick'],'server_id'=> $Res['server_id'],'start_time'=> $Res['start_time'],'end_time'=> $Res['end_time'],'oper_time'=> $Res['oper_time'],'notice'=> $Res['notice'],'oper_name'=> $Res['oper_name']);
		        $sendlist[$i] = $row;
		        $i++;
		      }
		      echo json_encode($sendlist);                                                                                               
		}
		$sql_result->close();
        exit(0);

	}while(0);
?>
