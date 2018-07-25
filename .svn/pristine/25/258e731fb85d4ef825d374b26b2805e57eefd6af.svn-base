<?php

require_once("../sg_gm/db_connect.php");
require_once("../sg_gm/lib/mysql_obj.php");

/*
@json format
{
    data : [{
        table_name : $table_name,
        col_name : $col_name,
        col_value : $col_value,
        filter_col_name : $filter_col_name,
        filter_col_value : $filter_col_value,
    }, {...}, ...]
}
*/

try{
    $db_con = get_connect("server_config");
    $mysql_obj = new MysqlObj($db_con);
    //TODO
    $mysql_obj->UPDATE($_POST);
    echo "{}";  

}catch (Exception $exception)
{
    die($exception);
}

?>