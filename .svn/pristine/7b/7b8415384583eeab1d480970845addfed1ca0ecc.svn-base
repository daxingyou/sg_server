<?php

require_once("../sg_gm/db_connect.php");

class MysqlOperationParameter
{
	public $m_table_name;
	public $m_col_name;
	public $m_col_value;
	public $m_filter_col_name;
	public $m_filter_col_value;
	/*
	@json format
	{
		table_name : $table_name,
		col_name : $col_name,
		col_value : $col_value,
		filter_col_name : $filter_col_name,
		filter_col_value : $filter_col_value,
	}
	*/
	public function __construct($json_data){

	}
	public function __construct($table_name, $col_name, $col_value, $filter_col_name = null, $filter_col_value = null){

	}
}

class MysqlObj
{
	private $m_db_con;
	public function __construct($db_con){
		$m_db_con = $db_con;
	}
	
	private function GenSql($label, $para){
		switch($label){
			case 'INSERT':
				return "INSERT INTO {$para->m_tabel_name} SET {$m_col_name} = '{$m_col_value}';";
				break;
			case 'UPDATE':
				return "UPDATE {$para->m_tabel_name} SET {$m_col_name}='{$m_col_value}' where {$m_filter_col_name}={m_filter_col_value};";
				break;
		}
	}
	//No difference between single query and multi query.Sacrifice execution speed for coding time.
	private function Query($type, $para_array){
		mysqli_autocommit($m_db_con, FALSE);
	    foreach($para_array as $para){
	        $sql = GenSql($type, $para)
	        $result = $db_con->query($sql);
	        if($result == false){
	            mysqli_rollback($db_con);
	            die("更新mysql错误" . $sql . mysql_error());
	        }
	    }
	    mysqli_commit($m_db_con);
	}
	public function UPDATE($para_array){
		Query('UPDATE', $para_array)
	}
	public function INSERT($para_array){
		Query('INSERT', $para_array)
	}
}

?>