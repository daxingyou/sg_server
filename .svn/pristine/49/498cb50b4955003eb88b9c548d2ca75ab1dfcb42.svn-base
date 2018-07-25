<?php
header("Content-Type:text/html;charset=utf-8");
require_once("sg_game_common/db_connect.php");
require_once("sg_game_common/sg_bbs_log.php");
require_once ("hero_strategy_create_comment.php");
require_once ("hero_strategy_get_comments.php");
require_once ("hero_strategy_like.php");


$cmd = urldecode($_GET['cmd']);
$role_uid = urldecode($_GET['role_uid']);

if($cmd == 1)
{
	$hero_tid = urldecode($_GET['hero_tid']);
	$author_info = urldecode($_GET['author_info']);
	$comment_info = urldecode($_GET['comment_info']);
	
	hero_strategy_create_comment::run($conn,$role_uid,$hero_tid,$author_info,$comment_info);
}
else if($cmd == 2)
{
	$hero_tid = urldecode($_GET['hero_tid']);
	$start_pos = urldecode($_GET['start_pos']);
	$get_num = urldecode($_GET['get_num']);
	$is_hot = urldecode($_GET['is_hot']);
	
	hero_strategy_get_comments::run($conn,$role_uid,$hero_tid,$start_pos,$get_num,$is_hot);

}
else if($cmd == 3)
{
	$comment_id = urldecode($_GET['comment_id']);
	$is_unlike = urldecode($_GET['is_unlike']);
	
	hero_strategy_like::run($conn,$role_uid,$comment_id,$is_unlike);
}

?>