<?php
    // 测试php是否可以拿到数据库中的数据

    // 做个路由 action为url中的参数
    // query_type 为 1 新增玩家 2 活跃玩家 3 玩家留存 4 玩家流失 5 付费转化
    
    //$data_info=isset($_POST["data"])?$_POST["data"]:"";
    //$data = json_decode($data_info);
    
    $datemin = $_GET['datemin'];
    $datemax = $_GET['datemax'];
    $query_type = $_GET['query_type'];

    //$datemin = $data->datemin;
    //$datemax = $data->datemax;
    //$query_type = $data->query_type;

   // $datemin = "2017-10-25";
    //$datemax = "2017-10-25"; 
   // $time_min = strtotime($datemin);
   // $time_max = strtotime($datemax);
   // if ($time_min == $time_max) {
   //     $time_min = $time_min - 86400;
    //    $datemin = date("Y-m-d H:i:s",$time_min);
   // }else {
   //      $time_min = $time_min - 86400;
   //      $datemin = date("Y-m-d H:i:s",$time_min);
   // }
    //$query_type = "2";
    //elseif (($time_max - $time_min) == 86400) {
      //  $time_min = $time_max - 86400;
        //$datemin = date("Y-m-d H:i:s",$time_min);
    //}
    //echo $datemin;
    try{
        // $dbname="root";
        // $dbpass="Sanguo1!";
        // $dbhost="47.100.130.174";
        // $dbpost = "3306";
        // $dbdatabase="sg_statistics";
        //$db_connect = mysql_connect($dbhost,$dbname,$dbpass,$dbpost);

        //选择一个需要操作的数据库
        //mysql_select_db($dbdatabase,$db_connect);
        $mysqli = get_connect("statistics");
        // $mysqli = new mysqli($data->log_host, $data->log_user, $data->log_pwd);
        if ($mysqli->connect_errno) {
            printf("Connect failed: %s\n", $mysqli->connect_error);
            exit(0);
        }
        //$mysqli->select_db($data->log_db);

        $strsql = null;
        switch ($query_type) 
        {
            case '1':
                $strsql = "SELECT  `time`, `mobile`, `account`, `role`, `rate`  FROM `new_user_log` where `time` >= '$datemin' and `time` <= '$datemax'";
                 //$strsql = "SELECT  `time`, `mobile`, `account`, `role`, `rate`  FROM `new_user_log` where `time` >= '$datemin' and `time` <= '$datemax'";
                break;
            case '2':
                $strsql = "SELECT  *  FROM `active_user_log` where `time` >= '$datemin' and `time` <= '$datemax'";
                //$strsql = "SELECT  `time`, `dau`, `wau`, `mau`, `dau/mau`  FROM `active_user_log` where `time` >= '$datemin' and `time` <= '$datemax'";
                //$strsql = "SELECT  `time`, `dau`, `wau`, `mau`, `dau/mau`  FROM `active_user_log` where `time` >= '$datemin' and `time` <= '$datemax'";
                break;
            case '3':
                $strsql = "SELECT  `time`, `second_day`, `seventh_day`, `thirtieth_day`  FROM `remain_user_log` where `time` >= '$datemin' and `time` <= '$datemax'";
                 //echo "strsql 3";
                break;
            case '4':
                 $strsql = "SELECT  `time`, `leave_count`, `leave_rate`, `return_count`  FROM `leave_retrun_user_log` where `time` >= '$datemin' and `time` <= '$datemax'";
                  //echo "strsql 4";
                break;
            //case '5':
              //  strsql = "SELECT  `time`, `dau`, `wau`, `mau`, `dau/mau`  FROM `active_user_log` where `time` >= '$datemin' and `time` <= '$datemax'";
               // break;
            default:
                # code...
                break;
        }

        // 获取查询结果
        //$result = mysql_query($strsql);
        //$list = mysql_fetch_array($result);
       // if(!empty($list))
        //{
        $result = $mysqli->query($strsql);
        //if ($result = $mysqli->query($strsql)) 
        //{
        //    printf("Select returned %d rows.\n", $result->num_rows);
            //while ($row = $result->fetch_row())
            //{
             //   println("$row[0]  $row[1]  $row[2]");
           // }
         //       while ($row = $result->fetch_array()) {
         //           echo($row['time'] . "-" . $row['mobile'] . "-" . $row['account']);
         //       }
            //$result->close();
        //}
            $i = 0;
             switch ($query_type) 
             {
                case '1':
                    $sendlist = array();
                     while($Res = $result->fetch_array())
                     {
                         $row = array( 'time'=> $Res['time'],'mobile'=> $Res['mobile'],'account'=> $Res['account'],'role'=> $Res['role'],'rate'=> $Res['rate']);
                         $sendlist[$i] = $row;
                         $i++;
                        //echo 'time:',$Res['time'] ,'mobile:',$Res['mobile'],'account:',$Res['account'],'role:',$Res['role'],'rate:',$Res['rate'];
                     }
                    echo json_encode($sendlist);
                    break;
                case '2':
                     $sendlist = array();
                     while($Res = $result->fetch_array())
                     {
                         $row = array( 'time'=> $Res['time'],'dau'=> $Res['dau'],'wau'=> $Res['wau'],'mau'=> $Res['mau'],'dm'=> $Res['dau/mau']);
                         $sendlist[$i] = $row;
                         $i++;
                         //echo 'time:',$Res['time'] ,'dau:',$Res['dau'],'wau:',$Res['wau'],'mau:',$Res['mau'],'dm:',$Res['dau/mau'];
                     }
                     echo json_encode($sendlist);
                    break;
                case '3':
                     $sendlist = array();
                     while($Res = $result->fetch_array())
                     {
                         $row = array( 'time'=> $Res['time'],'second_day'=> $Res['second_day'],'seventh_day'=> $Res['seventh_day'],'thirtieth_day'=> $Res['thirtieth_day']);
                         $sendlist[$i] = $row;
                         $i++;
                         //echo 'time:',$Res['time'] ,'second_day:',$Res['second_day'],'seventh_day:',$Res['seventh_day'],'thirtieth_day:',$Res['thirtieth_day'];
                     }
                     echo json_encode($sendlist);
                    break;
                case '4':
                     $sendlist = array();
                      while($Res = $result->fetch_array())
                     {
                         $row = array( 'time'=> $Res['time'],'leave_count'=> $Res['leave_count'],'leave_rate'=> $Res['leave_rate'],'return_count'=> $Res['return_count']);
                         $sendlist[$i] = $row;
                         $i++;
                         //echo 'time:',$Res['time'] ,'leave_count:',$Res['leave_count'],'leave_rate:',$Res['leave_rate'],'return_count:',$Res['return_count'];
                     }
                     echo json_encode($sendlist);
                    break;
                case '5':
                    # code...
                    break;
                default:
                    # code...
                    break;
            }
            //echo json_encode($sendlist);
            $result->close();
            exit(0);
        //}
    }catch (Exception $exception)
    {
        $zero = 0;
        echo($zero);
        //$result->close();
        exit(0);
    }
?>