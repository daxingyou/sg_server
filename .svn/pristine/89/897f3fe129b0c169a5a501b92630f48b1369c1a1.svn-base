<?php
header("Content-Type:text/html;charset=utf-8");
require_once("../sg_gm/db_connect.php");

    $mark_type = $_POST['mark_type'];
    try{

        $db_con = get_connect("sg_up");
        if($db_con->connect_errno)
        {
            $db_con = NULL;
            die("连接错误:".$db_con->connect_errno);
        }
        else
        {
            $db_con->set_charset("utf8");
        }

        $sql = "SELECT server_id FROM login_server_list WHERE mark_type = '$mark_type'";
        
        $result = $db_con->multi_query($sql);
        if($result != false)
        {
            $i = 0;
            $sendlist = array();
            do 
            {
                $store_result = $db_con->store_result();
                if($store_result != false)
                {
                    while ($Res = $store_result->fetch_array())
                    {
                        $row = array('server_id'=> $Res['server_id']);
                        $sendlist[$i] = $row;
                        $i++;
                    }
                }
            } while ($db_con->next_result());
            $store_result->close();
            $server_info_json = json_encode($sendlist);
            $server_info_result = urldecode($server_info_json);
            echo $server_info_result;
        }
        else
        {
            print "query error";    
        }
        
    }catch (Exception $exception)
    {
        $zero = 0;
        echo($zero);
        exit(0);
    }
?>