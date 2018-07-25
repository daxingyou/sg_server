<?php
header("Content-Type:text/html;charset=utf-8");
require_once("sg_game_common/sg_bbs_log.php");

class hero_strategy_like
{
	public static function run($conn,$role_uid,$comment_id,$is_unlike)
	{
		//$max_interact_num = 10;

		$obj->reply_code = "0";
		$obj->comment_id = $comment_id;

		$conn->query("set @result");
		$conn->query("set @new_like_num");
		//$conn->query("set @new_unlike_num");
		
		$is_unlike = 0;		// 目前只有点赞 所以写死 为了兼容跟扩展 接口参数不变

//		if($is_unlike == 1)
//		{
//			$sql = "CALL comment_unlike('$comment_id','$role_uid','$max_interact_num',@result,@new_like_num,@new_unlike_num)";
//		}
//		else
		{
			//$sql = "CALL comment_like('$comment_id','$role_uid','$max_interact_num',@result,@new_like_num,@new_unlike_num)";
			$sql = "CALL comment_like('$comment_id','$role_uid',@result,@new_like_num)";
		}
		$sql_result = $conn->query($sql);

		if($sql_result != false)
		{
			$sql_value_array_res[1] = $conn->query('select @result');
			$sql_value_array_res[2] = $conn->query('select @new_like_num');
			//$sql_value_array_res[3] = $conn->query('select @new_unlike_num');
			//FOR($i=1;$i<=3;$i++)
			FOR($i=1;$i<=2;$i++)
			{
				$sql_value_array[$i] = $sql_value_array_res[$i]->fetch_row();
			}
			$value_result = $sql_value_array[1][0];

			if($value_result == 1)
			{
				$obj->reply_code = "10130014";
			}
			else if($value_result == 2)
			{
				$obj->reply_code = "10130013";
			}
			else
			{
				$obj->new_like_num = $sql_value_array[2][0];
				//$obj->new_unlike_num = $sql_value_array[3][0];
			}
		}
		else
		{
			$obj->reply_code = "10000019";
			write_log("hero_strategy_like sql error:$conn->error $conn->errno");
		}
		
		//$obj_str = json_encode($obj);
		//$final_str = base64_encode($obj_str);
		//echo $final_str;
		echo json_encode($obj);
	}
}
?>
