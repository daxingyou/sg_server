<?php
header("Content-Type:text/html;charset=utf-8");
require_once("./sg_bbs_log.php");


class hero_strategy_create_comment
{
	public static function run($conn,$role_uid,$hero_tid,$author_info,$comment_info)
	{
		$hero_max_comments_num = 20;	// 每个武将最大评论数
		$hero_top_comments_num = 10;	// 置顶评论
		
		$conn->query("set @result");
		$sql_update = "CALL update_role_new_comment('$role_uid','$hero_tid',@result)";
		$sql_result=$conn->query($sql_update);
		
		$obj->reply_code = "0";
			
		if($sql_result != false)
		{
			$sql_value_res = $conn->query('select @result');
			$sql_value = $sql_value_res->fetch_row();
			$value_result = $sql_value[0];
			
			if($value_result == 0)
			{		
				// 创建评论
				$sql="CALL create_comment('$hero_tid','$role_uid','$author_info','$comment_info')";
				$sql_result=$conn->query($sql);

				// 评论成功检查更新 超过最大评论数 替换掉最老的一条非置顶评论
				if($sql_result != false)
				{
					$un_top_comments_num = $hero_max_comments_num - $hero_top_comments_num;
					$sql="select id,create_time from hero_strategy_comments where hero_strategy_comments.hero_tid = $hero_tid order by like_num desc limit $hero_top_comments_num,$un_top_comments_num;";
					$sql_result=$conn->query($sql);

					if($sql_result->num_rows > ($hero_max_comments_num-$hero_top_comments_num))
					{
						$old_create_time = date("Y-m-d H:i:s",time());
						$old_remove_id = 0;
							
						while ($row = $sql_result->fetch_array())
						{
							$comment_create_time = $row['create_time'];
							$comment_id = $row['id'];
							if($comment_create_time < $old_create_time)
							{
								$old_create_time = $comment_create_time;
								$old_remove_id = $comment_id;
							}
						}
						$sql="CALL delete_comment('$old_remove_id')";
						$sql_result=$conn->query($sql);
						if($sql_result == false)
						{
							write_log("hero_strategy_create_comment $sql error:$conn->error $conn->errno");
						}
					}
				}
				else
				{
					$obj->reply_code = "10130018";
					write_log("hero_strategy_create_comment $sql error:$conn->error $conn->errno");

					//add by hy 评论失败,删除评论记录
					$sql_del_role_new_comment_data=" delete from role_new_comment_data where role_uid = '$role_uid' and hero_tid = '$hero_tid'";
					$result=$conn->query($sql_del_role_new_comment_data);
        			if ($result == false ){
						write_log("hero_strategy_create_comment $sql_del_role_new_comment_data error:$conn->error $conn->errno");
        			}		
				}
			}
			else
			{
				$obj->reply_code = "10130015";
			}			
		}
		else
		{
			$obj->reply_code = "10000019";
			write_log("hero_strategy_create_comment $sql_update error:$conn->error $conn->errno");
		}
		
		//$obj_str = json_encode($obj);
		//$final_str = base64_encode($obj_str);
		//echo $final_str;
		echo json_encode($obj);
	}
}

?>
