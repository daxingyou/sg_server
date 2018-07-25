<?php 
	header('Content-type: application/json');
	require_once("db_connect.php");

	$errcode_num=array();
	$errcode_num['cdkey_exchange_ok']=0;
	$errcode_num['notice_unknown']=10000019;
	$errcode_num['cdkey_has_used']=1015001;
	$errcode_num['cdkey_user_used']=1015002;
	$errcode_num['cdkey_not_found']=1015003;


	 #获取post参数
	$user_id=isset($_GET["role_uid"])?$_GET["role_uid"]:0;
	$key_code=isset($_GET["key_code"])?$_GET["key_code"]:"";
	
	$result = array();
    $result["status"]="failed";
    $result["message"]->code=$errcode_num['notice_unknown'];
    $result["message"]->msg="unknow error";
	$conn = get_connect("sg_up");
	mysqli_query($conn, 'BEGIN');
	do{
		if ( $user_id == 0 ) {
			$result["message"]->code=$errcode_num['notice_unknown'];
			$result["message"]->msg="uid is null";
			break;
		}

		if ( $key_code == "" ) {
			$result["message"]->code=$errcode_num['notice_unknown'];
			$result["message"]->msg="key_code is null";
			break;
		}

		#连接数据库
		$sql = "select  key_code, key_id, key_type, use_count, max_use_count, limit_time  from cdkey where key_code = '$key_code' for update";
			$sql_result=$conn->query($sql);	
			if ($sql_result == false ){	
				$result["message"]->code=$errcode_num['cdkey_not_found'];
				$result["message"]->msg="key_code {$key_code} is not exist";
				break;
			}

			if( $row=$sql_result->fetch_array() ){	
				$key_code=$row['key_code']; 		
				$key_id=$row['key_id'];
				$key_type=$row['key_type']; 		
				$use_count=$row['use_count'];
				$max_use_count=$row['max_use_count']; 
				$limit_time=$row['limit_time']; 
				
				#使用次数	
				if ( $use_count >= $max_use_count ) {
					$result["message"]->code=$errcode_num['cdkey_has_used'];
					$result["message"]->msg="key_code is be max used";
					break;
				}
				#超时
				$now=time();
				if ( $now > $limit_time){
					$result["message"]->code=$errcode_num['cdkey_not_found'];
					$result["message"]->msg="time is out";
					break;
				}

				#判断玩家是否用过这个礼包
				$sql = "select user_id from use_cdkey where user_id='$user_id' and key_type='$key_type'";
				$sql_result=$conn->query($sql);	
				if ($sql_result == false || $sql_result->fetch_array() ){
					$result["message"]->code=$errcode_num['cdkey_user_used'];
					$result["message"]->msg="user hase used this cdkey";
					break;
				}

				#写入cdkey数据库
				$sql = "update cdkey set use_count=use_count+1 where key_code = '$key_code'";
				$sql_result=$conn->query($sql);	
				if ($sql_result == false ){	
					$result["message"]->code=$errcode_num['notice_unknown'];
					$result["message"]->msg="update table cdkey error ";
					break;
				}

				#写入玩家数据库
				$sql = "insert into use_cdkey( user_id, key_code, key_type, use_time ) values ('$user_id', '$key_code', '$key_type', '$now')";
				$sql_result=$conn->query($sql);	
				if ($sql_result == false ){	
					$result["message"]->code=$errcode_num['notice_unknown'];
					$result["message"]->msg="update use_cdkey error ";
					break;
				}

    			$result["status"]="success";
    			$result["key_id"]=$key_id;
				$result["message"]->code=$errcode_num['cdkey_exchange_ok'];
				$result["message"]->msg="get cd_key success";
				break;
			}else{
				$result["message"]->code=$errcode_num['cdkey_not_found'];
				$result["message"]->msg="key_code {$key_code} is not exist";
				break;
			}
	}while(0);
	mysqli_query($conn, 'COMMIT'); 
	$result = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){ return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1])); }, json_encode($result));
	echo $result;
?>
