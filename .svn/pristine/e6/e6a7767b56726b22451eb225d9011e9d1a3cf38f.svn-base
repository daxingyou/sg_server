<?php
require('db_config.php');

function get_connect($type,$param = "")
{   
    $conf = NULL;
    //$db_info = new Global_DB_Conf();
    switch ($type) {
        case 'sg_up':
        {
            $conf = Global_DB_Conf::get_sg_up_db_info();
        }
        break;
        case 'sg_gm':
        {
            $conf = Global_DB_Conf::get_sg_gm_db_info();
        }
        break;
        case 'sg_log':
        {
            $conf = Global_DB_Conf::get_sg_log_db_info($param);
        }
        break;
        case 'statistics':
        {
            $conf = Global_DB_Conf::get_sg_statistics_db_info();
        }
        break;
        case 'server_config':
        {
            $conf = Global_DB_Conf::get_server_db_info();
        }
        break;
        default:
            break;
    }

    $conn = new mysqli($conf['db_host'],$conf['db_user'],$conf['db_pwd'],$conf['db_name']);
    if($conn->connect_errno)
    {
        //$conn = NULL;
        throw new Exception("Connect db{$type} error:" . $conn->connect_errno);
    }
    else
    {
        $conn->set_charset("utf8");
        return $conn;
    }
}


?>
