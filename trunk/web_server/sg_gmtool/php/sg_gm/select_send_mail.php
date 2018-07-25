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
			  $sql = "SELECT  *  FROM `send_mail_log` where `server_id` = '$sid'";
		      if(property_exists($data,"id"))
		      {
		        $sql .= "and id = '$data->id'";
		      }
		      if(property_exists($data,"mail_type"))
		      {
		        $sql .= "and mail_type = '$data->mail_type'";
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
		        $row = array( 'id'=> $Res['id'],'mail_type'=> $Res['mail_type'],'server_id'=> $Res['server_id'],'title'=> $Res['title'],'content'=> $Res['content'],'oper_time'=> $Res['oper_time'],'send_name'=> $Res['send_name'],'send_time'=> $Res['send_time'],'end_time'=> $Res['end_time'],'on_time'=> $Res['on_time'],'items'=> $Res['items'],'limit_level'=> $Res['limit_level'],'limit_cond'=> $Res['limit_cond'],'oper_name'=> $Res['oper_name']);
		        $sendlist[$i] = $row;
		        $i++;
		      }
		      echo json_encode($sendlist);                                                                                               
		}
		$sql_result->close();
        exit(0);

	}while(0);
?>
