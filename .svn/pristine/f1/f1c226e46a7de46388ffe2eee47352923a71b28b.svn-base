<?php
header("Content-Type:text/html;charset=utf-8");
require_once("../sg_gm/db_connect.php");

    try{
		$db_con = get_connect("sg_gm");
		if($db_con->connect_errno)
		{
			$db_con = NULL;
			die("连接错误:".$db_con->connect_errno);
		}
		else
		{
			$db_con->set_charset("utf8");
		}

		$sql = "SELECT * FROM `gm_server_list`";
        $result = $db_con->multi_query($sql);

        if($result != false)
        {
            $i = 0;
            $sendlist = array();
            do {
                $store_result = $db_con->store_result();
                if($store_result != false)
                {
                    while ($Res = $store_result->fetch_array())
                    {
						$row = array( 'server_id'=> $Res['server_id'],'server_name'=> urlencode($Res['server_name']),'ip'=> $Res['ip'],'port'=> $Res['port']);
						$sendlist[$i] = $row;
						$i++;
                    }
                    $store_result->close();
                }
            } while ($db_con->next_result());
			
			$server_info_json = json_encode($sendlist);
			$server_info_result = urldecode($server_info_json);
			echo $server_info_result;
        }

    }catch (Exception $exception)
    {
        $zero = 0;
        echo($zero);
        exit(0);
    }
?>