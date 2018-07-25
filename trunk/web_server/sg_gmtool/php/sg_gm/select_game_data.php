<?php
header("Content-Type:text/html;charset=utf-8");
require_once('db_connect.php');

    $data_info=isset($_POST["data"])?$_POST["data"]:"";
    $data = json_decode($data_info);
    
    $datemin = $data->time_min;
    $datemax = $data->time_max;
    $query_type = $data->query_type;
    try{
        $mysqli = get_connect("statistics");
        if ($mysqli->connect_errno) {
            printf("Connect failed: %s\n", $mysqli->connect_error);
            exit(0);
        }

        $strsql = null;
        switch ($query_type) 
        {
            case 'new_role':
                $strsql = "SELECT  `time`, `mobile`,`reg_mobile`, `account`, `role`, `rate`  FROM `new_user_log` where `time` >= '$datemin' and `time` <= '$datemax' order by `time`";
                break;
            case 'active_role':
                $strsql = "SELECT  *  FROM `active_user_log` where `time` >= '$datemin' and `time` <= '$datemax' order by `time`";
                break;
            case 'retained_role':
                $strsql = "SELECT  `time`, `second_day`, `seventh_day`, `thirtieth_day`  FROM `remain_user_log` where `time` >= '$datemin' and `time` <= '$datemax' order by `time`";
                break;
            case 'loss_role':
                 $strsql = "SELECT  `time`, `leave_count`, `leave_rate`, `return_count`  FROM `leave_retrun_user_log` where `time` >= '$datemin' and `time` <= '$datemax' order by `time`";
                break;
            default:
                break;
        }

       
        $result = $mysqli->multi_query($strsql);
        if ($result != false) {
            $i = 0;
            do {
                $store_result = $mysqli->store_result();
                if($store_result != false)
                {
                    $sendlist = array();
                    switch ($query_type) 
                    {
                        case 'new_role':
                            while($Res = $store_result->fetch_array())
                            {
                                $row = array( 'time'=> $Res['time'],'mobile'=> $Res['mobile'],'reg_mobile'=> $Res['reg_mobile'],'account'=> $Res['account'],'role'=> $Res['role'],'rate'=> $Res['rate']);
                                $sendlist[$i] = $row;
                                $i++;
                            }
                            echo json_encode($sendlist);
                            break;
                        case 'active_role':
                            while($Res = $store_result->fetch_array())
                            {
                                $row = array( 'time'=> $Res['time'],'dau'=> $Res['dau'],'wau'=> $Res['wau'],'mau'=> $Res['mau'],'dm'=> $Res['dau/mau']);
                                $sendlist[$i] = $row;
                                $i++;
                            }
                            echo json_encode($sendlist);
                            break;
                        case 'retained_role':
                            while($Res = $store_result->fetch_array())
                            {
                                $row = array( 'time'=> $Res['time'],'second_day'=> $Res['second_day'],'seventh_day'=> $Res['seventh_day'],'thirtieth_day'=> $Res['thirtieth_day']);
                                $sendlist[$i] = $row;
                                $i++;
                            }
                            echo json_encode($sendlist);
                            break;
                        case 'loss_role':
                            while($Res = $store_result->fetch_array())
                            {
                                $row = array( 'time'=> $Res['time'],'leave_count'=> $Res['leave_count'],'leave_rate'=> $Res['leave_rate'],'return_count'=> $Res['return_count']);
                                $sendlist[$i] = $row;
                                $i++;
                            }
                            echo json_encode($sendlist);
                            break;
                        default:
                            break;
                    }
                }
            } while ($mysqli->next_result());
             
            $store_result->close();
            exit(0);
        } else {
            print "query error";    
        }
    }catch (Exception $exception)
    {
        $zero = 0;
        echo($zero);
        exit(0);
    }
?>