<?php
header("Content-Type:text/html;charset=utf-8");
require_once("sg_game_common/sg_bbs_log.php");

function JSON($array) {
 arrayRecursive($array, 'urlencode', true);
 $json = json_encode($array);
 return urldecode($json);
}

class hero_strategy_get_comments
{
	public static function run($conn,$role_uid,$hero_tid,$start_pos,$get_num,$is_hot)
	{
		// 获取评论时 更新玩家的评论记录 清除冗余数据(玩家的点赞 评论记录)
		$sql_update = "CALL update_role_data()";
		$sql_result=$conn->multi_query($sql_update);
		if($sql_result == false)
		{
			write_log("hero_strategy_get_comments sql_update error:$conn->error $conn->errno!");
		}
		
		$sql="CALL get_comments('$hero_tid','$start_pos','$get_num','$is_hot')";
		$sql_result=$conn->multi_query($sql);

		$obj->reply_code = "0";

		if($sql_result != false)
		{
			$obj->is_hot = $is_hot;
			$comment_count = 0;
			
			do {
				$store_result = $conn->store_result();
				if($store_result != false)
				{
					while ($row = $store_result->fetch_array())
					{
						$obj->comments_list[$comment_count]->comment_single->id = $row['id'];
						$obj->comments_list[$comment_count]->comment_single->hero_tid = $row['hero_tid'];
						$obj->comments_list[$comment_count]->comment_single->like_num = $row['like_num'];
						$obj->comments_list[$comment_count]->comment_single->unlike_num = $row['unlike_num'];
						$obj->comments_list[$comment_count]->comment_single->author_info = urlencode($row['author_info']);
						$obj->comments_list[$comment_count]->comment_single->comment_info = urlencode($row['comment_info']);
						$obj->comments_list[$comment_count]->comment_single->create_time = $row['create_time'];
						$obj->comments_list[$comment_count]->comment_single->has_like = 0;
						$comment_count++;
					}
					$store_result->close();
				}
			} while ($conn->next_result());
		}
		else
		{
			$obj->reply_code = "10000019";
			write_log("hero_strategy_get_comments sql error:$conn->error $conn->errno");
		}
		
		if(isset($obj->comments_list))
		{
			for($i=0; $i< count($obj->comments_list); $i++)
			{
				$sql = "select comment_id from role_like_data where role_uid = $role_uid and comment_id = " . $obj->comments_list[$i]->comment_single->id . ";";
				$sql_result=$conn->multi_query($sql);
				if($sql_result != false)
				{
					do {
						$store_result = $conn->store_result();
						if($store_result != false)
						{
							if($store_result->num_rows > 0)
							{
								$obj->comments_list[$i]->comment_single->has_like = 1;
							}
							$store_result->close();
						}
					} while ($conn->next_result());
				}
				else
				{
					$obj->reply_code = "10000019";
					write_log("hero_strategy_get_comments sql error:$conn->error $conn->errno");
				}
			}
		}
		
		//$str = json_encode($obj);
		//$str = preg_replace_callback("#\\\u([0-9a-f]{4})#i",function($matchs){return iconv('UCS-2BE', 'UTF-8', pack('H4', $matchs[1]));},$str);
		//$str = iconv("GB2312","UTF-8",$str);
		//$test = json_decode($str);
		//echo $test->comments_list[1]->comment_single->author_info;
		
		$obj_str = json_encode($obj);
		$url_str = urldecode($obj_str);
		//$final_str = base64_encode($url_str);
		echo $url_str;
	}
}
?>