<?php
header("Content-Type:text/html;charset=utf-8");
require_once("../sg_gm/db_connect.php");

    $query_type = $_POST['query_type'];
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

        $sql = "";

        if ($query_type == "mark") {
            $sql = "SELECT  mark_type,platform_name,channel_name, version,compatible_version FROM platform_info";
        } else if($query_type == "mark_account"){
            $sql = "SELECT  mark_type, server_name, ip , port,id FROM account_server_list";
        }
        else if($query_type == "mark_login"){
            $sql = "SELECT  mark_type, server_name, server_id, status , label, id FROM login_server_list";
        }
        else if($query_type == "mark_url"){
            $sql = "SELECT * FROM bbs_info;SELECT * FROM plist_info;SELECT * FROM notice_info;SELECT * FROM res_info;SELECT * FROM combat_view";
        }
        $result = $db_con->multi_query($sql);
        if($result != false)
        {
            $i = 0;
            $j = 0;
            $sendlist = array();
            do 
            {
                $store_result = $db_con->store_result();
                if($store_result != false)
                {
                    if ($query_type == "mark") 
                    {
                        while ($Res = $store_result->fetch_array())
                        {
                            $row = array('mark_type'=> $Res['mark_type'],'platform_name'=> urlencode($Res['platform_name']),'channel_name'=> urlencode($Res['channel_name']),'version'=> $Res['version'],'compatible_version'=> $Res['compatible_version']);
                            $sendlist[$i] = $row;
                            $i++;
                        }
                    } else if($query_type == "mark_account")
                    {
                        while ($Res = $store_result->fetch_array())
                        {
                            $row = array('mark_type'=> $Res['mark_type'],'server_name'=> urlencode($Res['server_name']),'ip'=> $Res['ip'],'port'=> $Res['port'],'id'=> $Res['id']);
                            $sendlist[$i] = $row;
                            $i++;
                        }
                    }else if ($query_type == "mark_login") 
                    {
                        while ($Res = $store_result->fetch_array())
                        {
                            $row = array('mark_type'=> $Res['mark_type'],'server_name'=> urlencode($Res['server_name']),'server_id'=> $Res['server_id'],'status'=> $Res['status'],'label'=> $Res['label'],'id'=> $Res['id']);
                            $sendlist[$i] = $row;
                            $i++;
                        }
                    }
                    else if ($query_type == "mark_url") 
                    {
                        $list = array();
                        while ($Res = $store_result->fetch_array())
                        {
                            switch ($j) 
                            {
                                case '0':
                                {
                                    $row = array('mark_type'=> $Res['mark_type'],'bbs_url'=> $Res['bbs_url'],'id'=> $Res['id']);
                                    $list[$i] = $row;
                                    $i++;
                                }
                                break;
                                case '1':
                                {
                                    $row = array('mark_type'=> $Res['mark_type'],'plist_url'=> $Res['plist_url'],'id'=> $Res['id']);
                                    $list[$i] = $row;
                                    $i++;
                                }
                                break;
                                case '2':
                                {
                                    $row = array('mark_type'=> $Res['mark_type'],'notice_url'=> $Res['notice_url'],'id'=> $Res['id']);
                                    $list[$i] = $row;
                                    $i++;
                                }
                                break;
                                case '3':
                                {
                                    $row = array('mark_type'=> $Res['mark_type'],'res_url'=> $Res['res_url'],'id'=> $Res['id']);
                                    $list[$i] = $row;
                                    $i++;
                                }
                                break;
                                case '4':
                                {
                                    $row = array('mark_type'=> $Res['mark_type'],'combat_url'=> $Res['combat_url'],'id'=> $Res['id']);
                                    $list[$i] = $row;
                                    $i++;
                                }
                                break;
                                default:
                                    break;
                            }
                        }
                        $sendlist[$j] = $list;
                        $j++;
                        $i = 0;
                        //print $sendlist;
                    }
                }
                //mysqli_free_result($db_con);
            } while ($db_con->next_result());
            $store_result->close();
            $server_info_json = json_encode($sendlist);
            //echo $server_info_json;
            //print $server_info_json;
            $server_info_result = urldecode($server_info_json);
            //print $server_info_result;
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