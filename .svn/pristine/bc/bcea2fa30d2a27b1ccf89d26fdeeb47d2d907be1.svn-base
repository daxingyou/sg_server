<?php

require_once("../sg_gm/db_connect.php");
require_once("../util/mysql_obj.php");

$db_con = get_connect($_POST['database']);
if($db_con->connect_errno){
	throw new Exception("NO DB CONNECTION " . $db_con->connect_error);
}
$mysql_obj = new MysqlObj($db_con);
echo json_encode($mysql_obj->SELECT($_POST['data']));

?>
