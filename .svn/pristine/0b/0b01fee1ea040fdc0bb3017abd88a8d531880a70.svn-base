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
        		$flag = 0;
			  $sql = "SELECT  *  FROM `recharge_log` ";
		      if(property_exists($data,"role_uid"))
		      {
	            if(0 == $flag)
	            {
	               $sql .= "where role_uid = '$data->id'";
	               $flag = 1;
	            }
			        else
	            {
	              $sql .= "and role_uid = '$data->id'";
	            }
		      }
		      if(property_exists($data,"channel_id"))
		      {
		        if(0 == $flag)
	            {
	               $sql .= "where channel_id = '$data->channel_id'";
	               $flag = 1;
	            }
			        else
	            {
	              $sql .= "and channel_id = '$data->channel_id'";
	            }
		      }

		      if(property_exists($data,"start_time"))
		      {
		            if(0 == $flag)
		            {
		               $sql .= "where create_time >= '$data->start_time'";
		               $flag = 1;
		            }
				        else
		            {
		              $sql .= "and create_time >= '$data->start_time'";
		            }
		        
		      }

		      if(property_exists($data,"end_time"))
		      {
		            if(0 == $flag)
		            {
		               $sql .= "where create_time >= '$data->end_time'";
		               $flag = 1;
		            }
				        else
		            {
		              $sql .= "and create_time >= '$data->end_time'";
		            }
		      }

			    $sql_result=$conn_log->query($sql);
		      $sendlist = array();
		      $i = 0;
					while($Res = $sql_result->fetch_array())
		      {
		        $row = array( 'id'=> $Res['role_uid'],'order_id'=> $Res['order_id'],'recharge_tid'=> $Res['recharge_tid'],'recharge_rmb'=> $Res['recharge_rmb'],'channel_id'=> $Res['channel_id'],'create_time'=> $Res['create_time']);
		        $sendlist[$i] = $row;
		        $i++;
		      }
		      echo json_encode($sendlist);  
		      $sql_result->close();                            
        exit(0);

	}while(0);
?>
