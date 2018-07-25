<?php

require_once("../sg_gm/db_connect.php");

class MysqlOperationParameter
{
	public $m_table_name;
	public $m_col_name;
	public $m_col_value;
	public $m_filter_col_name;
	public $m_filter_col_value;
}

class MysqlObj
{
	private $m_db_con;
	public function __construct($db_con){
		$this->m_db_con = $db_con;
	}
	
	private function GenSql($label, $para){
		switch($label){
			case 'INSERT':
				return "INSERT INTO {$para['tabel_name']} SET {$para['col_name']} = \'{$para['m_col_value']}\';";
				break;
			case 'UPDATE':
				return "UPDATE {$para['tabel_name']} SET {$para['col_name']}=\'{$para['col_value']}\' where {$para['filter_col_name']}={$para['filter_col_value']};";
				break;
			case 'SELECT':
				{
					$col_range = '';
					if(array_key_exists('col_name', $para) === false){
						$col_range = '*';	
					}
					else{
						$col_range = $para['col_name'];
					}
					$sql = "SELECT {$col_range} FROM {$para['table_name']}";
					if(array_key_exists('filter_col_name', $para) === true){
						$sql .= " WHERE {$para['filter_col_name']}={$para['filter_col_value']};";
					}
					else{
						$sql .= ';';
					}
					return $sql;
				};
				break;
			default:
				throw new Exception("Wrong Label:" . $label);
		}
	}
	//No difference between single query and multi query.Sacrifice execution speed for coding time.
	private function Query($type, $para_array, &$result_array){
		mysqli_autocommit($this->m_db_con, FALSE);
	    foreach($para_array as $para){
	        $sql = $this->GenSql($type, $para);
	        $result = $this->m_db_con->query($sql);
	        if($result == false){
	            mysqli_rollback($this->m_db_con);
	            throw new Exception("QUERY FAILED:" .$sql . $this->m_db_con->error);
	        }
			if($type === 'SELECT'){
				while($array_data = mysqli_fetch_array($result, MYSQLI_ASSOC)){
					array_push($result_array, $array_data);
				}
			}
	    }
	    mysqli_commit($this->m_db_con);
	}
	public function UPDATE($para_array){
		$this->Query('UPDATE', $para_array);
	}
	public function INSERT($para_array){
		$this->Query('INSERT', $para_array);
	}
	public function SELECT($para_array){
		$arr = array();
		$this->Query('SELECT', $para_array, $arr);
		return $arr;
	}
}

?>
