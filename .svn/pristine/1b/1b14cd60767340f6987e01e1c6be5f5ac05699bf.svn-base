<?php

$sg_gm_conf=array("db_host"=>"139.196.41.108" ,"db_user" => "wsy" ,"db_pwd" => "Wsy1985!","db_name" => "sg_gm","db_post" =>"3306");

$sg_up_conf=array("db_host"=>"139.196.41.108" ,"db_user" => "wsy" ,"db_pwd" => "Wsy1985!","db_name" => "sg_up","db_post" =>"3306");

$sg_statistics_conf=array("db_host"=>"139.196.41.108" ,"db_user" => "fengtu" ,"db_pwd" => "Fengtu2018!","db_name" => "sg_statistics","db_post" =>"3306");

$sg_log_1_conf=array("db_host"=>"192.168.1.5" ,"db_user" => "root" ,"db_pwd" => "Sanguo1!","db_name" => "nei_trunk_sg_log","db_post" =>"3306");

$sg_log_2_conf=array("db_host"=>"192.168.1.6" ,"db_user" => "root" ,"db_pwd" => "Sanguo1!","db_name" => "design_trunk_sg_log","db_post" =>"3306");

$sg_log_3_conf=array("db_host"=>"139.196.41.108" ,"db_user" => "fengtu" ,"db_pwd" => "Fengtu2018!","db_name" => "wai_trunk_sg_log","db_post" =>"3306");

$server_conf = array("db_host"=>"139.196.41.108" ,"db_user" => "wsy" ,"db_pwd" => "Wsy1985!","db_name" => "sg_config","db_post" =>"3306");

class Global_DB_Conf 
{
    public  static function get_sg_gm_db_info()
    {
        return $GLOBALS['sg_gm_conf'];
    }

    public static function get_sg_up_db_info()
    {
        return $GLOBALS['sg_up_conf'];
    }

    public static function get_bbs_db_info()
    {
        return $GLOBALS['sg_bbs_conf'];
    }

    public static function get_cdk_db_info()
    {
        return $GLOBALS['sg_cdk_conf'];
    }

    public  static function get_sg_log_db_info($param)
    {
        switch ($param) {
            case '1':
                {
                    return $GLOBALS['sg_log_1_conf'];
                }
                break;
            case '2':
                {
                    return $GLOBALS['sg_log_2_conf'];
                }
                break;
            case '3':
                {
                    return $GLOBALS['sg_log_3_conf'];
                }
                break;
            default:
                break;
        }
        
    }

    public  static function get_sg_statistics_db_info()
    {
        return $GLOBALS['sg_statistics_conf'];
    }

    public  static function get_server_db_info()
    {
        return $GLOBALS['server_conf'];
    }
   
}


?>
