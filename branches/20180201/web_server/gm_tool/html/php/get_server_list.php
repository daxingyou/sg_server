<?php
header("Content-Type:text/html;charset=utf-8");
    try{
        $dbname="root";
        $dbpass="Sanguo1!";
        $dbhost="139.196.41.108";
        $dbpost = "3306";
        $dbdatabase="sg_update";

        $mysqli = new mysqli($dbhost, $dbname, $dbpass);
        if ($mysqli->connect_errno) {
            printf("Connect failed: %s\n", $mysqli->connect_error);
            exit(0);
        }
        $mysqli->select_db($dbdatabase);
        $mysqli->set_charset("utf8");
        $strsql = "SELECT  `mark_type`, `server_id`, `status`, `server_name`, `label`  FROM `login_server_list` where `server_id` = 31 and `mark_type` = 'JIUYOU_2_ANDROID_2' ";
        $result = $mysqli->multi_query($strsql);

        if($result != false)
        {
            $i = 0;
            $sendlist = array();
            do {
                $store_result = $mysqli->store_result();
                if($store_result != false)
                {
                    while ($Res = $store_result->fetch_array())
                    {
                       $row = array( 'mark_type'=> $Res['mark_type'],'server_id'=> $Res['server_id'],'status'=> $Res['status'],'server_name'=> $Res['server_name'],'label'=> $Res['label']);
                         $sendlist[$i] = $row;
                         $i++;
                    }
                    $json_data = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){ return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1])); },json_encode($sendlist));
                    //echo json_encode($sendlist);
                    echo $json_data;
                    $store_result->close();
                }
            } while ($mysqli->next_result());
        }

    }catch (Exception $exception)
    {
        $zero = 0;
        echo($zero);
        exit(0);
    }
?>