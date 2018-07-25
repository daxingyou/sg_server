<?php
require('./db_config.php');

$conf = NULL;
$db_info = new Global_DB_Conf();
$conf = $db_info->get_cdk_db_info();
    
$conn = new mysqli($conf['db_host'],$conf['db_user'],$conf['db_pwd'],$conf['db_name']);
if($conn->connect_errno)
 {
    die("连接错误:".$conn->connect_errno);
}
else
{
    $conn->set_charset("utf8");
}
?>

