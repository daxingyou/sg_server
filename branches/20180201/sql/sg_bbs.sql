
CREATE TABLE IF NOT EXISTS `comments_id` (
	`id_index` int(10) unsigned NOT NULL DEFAULT '0',
	`id_count` bigint(20) unsigned NOT NULL DEFAULT '0',
	PRIMARY KEY (`id_index`)
)
COMMENT='评论id记录' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `hero_strategy_comments` (
	`id` bigint(20) unsigned NOT NULL DEFAULT '0',
	`hero_tid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '武将tid',
	`like_num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '点赞数',
	`unlike_num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '点踩数',
	`author_info` varchar(64) NOT NULL DEFAULT '0' COMMENT '发表评论人',
	`comment_info` varchar(128) NOT NULL DEFAULT '0' COMMENT '评论内容',
	`create_time` datetime NOT NULL COMMENT '发表时间',
	PRIMARY KEY (`id`),
	INDEX (`hero_tid`)
)
COMMENT='武将评论' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `role_like_data` (
  `role_uid` bigint(20) NOT NULL COMMENT '玩家UID',
  `comment_id` bigint(10) NOT NULL COMMENT '评论ID',
  PRIMARY KEY (`role_uid`,`comment_id`),
  INDEX (`comment_id`)
)
COMMENT='角色点赞数据' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `role_unlike_data` (
  `role_uid` bigint(20) NOT NULL COMMENT '玩家UID',
  `comment_id` bigint(10) NOT NULL COMMENT '评论ID',
  PRIMARY KEY (`role_uid`,`comment_id`),
  INDEX (`comment_id`)
)
COMMENT='角色点踩数据' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `role_new_comment_data` (
  `role_uid` bigint(20) NOT NULL COMMENT '玩家UID',
  `hero_tid` int(10) NOT NULL COMMENT '武将tid',
  PRIMARY KEY (`role_uid`,`hero_tid`),
  INDEX (`hero_tid`)
)
COMMENT='角色新发表评论数据' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `role_update` (
  `last_update_time` datetime DEFAULT NULL COMMENT '最后一次更新时间'
)
COMMENT='角色数据更新记录' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

DELIMITER // 
DROP PROCEDURE IF EXISTS `comments_id_create` //
CREATE PROCEDURE `comments_id_create`()
	COMMENT '评论id创建'
BEGIN
	DECLARE id_count int;
	SELECT COUNT(*) INTO id_count FROM comments_id;
  
	IF id_count = 0 THEN
		INSERT INTO comments_id VALUE(1,1);
	END IF;
END //
DELIMITER ;

CALL comments_id_create();

DELIMITER // 
DROP PROCEDURE IF EXISTS `role_update_create` //
CREATE PROCEDURE `role_update_create`()
	COMMENT '角色更新记录创建'
BEGIN
	DECLARE value_count int;
	SELECT COUNT(*) INTO value_count FROM role_update;
  
	IF value_count = 0 THEN
		INSERT INTO role_update VALUE('now()');
	END IF;
END //
DELIMITER ;

CALL role_update_create();

DELIMITER // 
DROP PROCEDURE IF EXISTS `comment_like` //
CREATE PROCEDURE `comment_like`(IN `comment_id` bigint,IN `role_uid` bigint,OUT `result` int,OUT `new_like_num` int)
    COMMENT '评论点赞'
BEGIN
	DECLARE role_interact_count INT DEFAULT 0;
	DECLARE role_like_count INT DEFAULT 0;
	DECLARE role_unlike_count INT DEFAULT 0;
	DECLARE role_comment_id INT DEFAULT 0;

	SELECT role_like_data.comment_id into role_comment_id FROM role_like_data WHERE role_like_data.role_uid = role_uid and role_like_data.comment_id = comment_id;
	
	IF role_comment_id > 0 THEN
		SET result = 1;	
	ELSE
		#SELECT COUNT(*) INTO role_like_count FROM role_like_data WHERE role_like_data.role_uid = role_uid;
		#SELECT COUNT(*) INTO role_unlike_count FROM role_unlike_data WHERE role_unlike_data.role_uid = role_uid;
		#SET role_interact_count = role_like_count + role_unlike_count;

		#IF role_interact_count + 1 >= max_limit THEN
		#		SET result = 2;
		#ELSE
				SET result = 0;
				INSERT INTO role_like_data VALUES(role_uid, comment_id);
				UPDATE hero_strategy_comments SET hero_strategy_comments.like_num = hero_strategy_comments.like_num + 1 WHERE hero_strategy_comments.id = comment_id;
				#SELECT hero_strategy_comments.like_num,hero_strategy_comments.unlike_num INTO new_like_num,new_unlike_num FROM hero_strategy_comments WHERE hero_strategy_comments.id = comment_id;
				SELECT hero_strategy_comments.like_num INTO new_like_num FROM hero_strategy_comments WHERE hero_strategy_comments.id = comment_id;
		#END IF;
	END IF;

END //
DELIMITER ;

DELIMITER // 
DROP PROCEDURE IF EXISTS `comment_unlike` //
CREATE PROCEDURE `comment_unlike`(IN `comment_id` bigint,IN `role_uid` bigint,IN `max_limit` int,OUT `result` int,OUT `new_like_num` int,OUT `new_unlike_num` int)
    COMMENT '评论点踩'
BEGIN
	DECLARE role_interact_count INT DEFAULT 0;
	DECLARE role_like_count INT DEFAULT 0;
	DECLARE role_unlike_count INT DEFAULT 0;
	DECLARE role_comment_id INT DEFAULT 0;

	SELECT role_unlike_data.comment_id into role_comment_id FROM role_unlike_data WHERE role_unlike_data.role_uid = role_uid and role_unlike_data.comment_id = comment_id;
	
	IF role_comment_id > 0 THEN
		SET result = 1;	
	ELSE
		SELECT COUNT(*) INTO role_like_count FROM role_unlike_data WHERE role_unlike_data.role_uid = role_uid;
		SELECT COUNT(*) INTO role_unlike_count FROM role_unlike_data WHERE role_unlike_data.role_uid = role_uid;
		SET role_interact_count = role_like_count + role_unlike_count;

		IF role_interact_count + 1 >= max_limit THEN
				SET result = 2;
		ELSE
				SET result = 0;
				INSERT INTO role_unlike_data VALUES(role_uid, comment_id);
				UPDATE hero_strategy_comments SET hero_strategy_comments.unlike_num = hero_strategy_comments.unlike_num + 1 WHERE hero_strategy_comments.id = comment_id;
				SELECT hero_strategy_comments.like_num,hero_strategy_comments.unlike_num INTO new_like_num,new_unlike_num FROM hero_strategy_comments WHERE hero_strategy_comments.id = comment_id;
		END IF;
	END IF;

END //
DELIMITER ;

DELIMITER // 
DROP PROCEDURE IF EXISTS `create_comment` //
CREATE PROCEDURE `create_comment`(IN `hero_tid` int,IN `role_uid` bigint,IN `author_info` varchar(64),IN `comment_info` varchar(128))
    COMMENT '创建评论'
BEGIN
	DECLARE new_id BIGINT DEFAULT 0;

	SELECT id_count INTO new_id FROM comments_id WHERE id_index = 1;
	INSERT INTO hero_strategy_comments VALUES(new_id, hero_tid, 0, 0, author_info, comment_info, NOW());
	SET new_id = new_id + 1;
	UPDATE comments_id SET id_count = new_id WHERE id_index = 1;
	
END //
DELIMITER ;

DELIMITER // 
DROP PROCEDURE IF EXISTS `delete_comment` //
CREATE PROCEDURE `delete_comment`(IN `comment_id` bigint)
	COMMENT '删除评论'
BEGIN
	DECLARE t_error INT DEFAULT 0;
	DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET t_error=1;

	START TRANSACTION;
		DELETE FROM hero_strategy_comments WHERE hero_strategy_comments.id = comment_id;
	IF t_error = 1 THEN
		ROLLBACK;
	ELSE
		COMMIT;
	END IF;
END //
DELIMITER ;

DELIMITER // 
DROP PROCEDURE IF EXISTS `get_comments` //
CREATE PROCEDURE `get_comments`(IN `hero_tid` int,IN `start_pos` int,IN `get_num` int,IN `is_hot` int)
    COMMENT '获取评论列表'
BEGIN
	DECLARE s_sql_select VARCHAR(2000);
	IF is_hot = 1 THEN
		SET s_sql_select = CONCAT('SELECT * FROM hero_strategy_comments WHERE hero_strategy_comments.hero_tid = ', hero_tid,' ORDER BY hero_strategy_comments.like_num DESC LIMIT ',start_pos,',',get_num);
	ELSE
		SET s_sql_select = CONCAT('SELECT * FROM hero_strategy_comments WHERE hero_strategy_comments.hero_tid = ', hero_tid,' ORDER BY hero_strategy_comments.create_time DESC LIMIT ',start_pos,',',get_num);
	END IF;

	SET @sql_select = s_sql_select;
	PREPARE stmt FROM @sql_select;
	EXECUTE stmt;
END //
DELIMITER ;

DELIMITER // 
DROP PROCEDURE IF EXISTS `get_hero_comments` //
CREATE PROCEDURE `get_hero_comments`(IN `hero_tid` int)
	COMMENT '获取英雄评论'
BEGIN
	select id,create_time from hero_strategy_comments where hero_strategy_comments.hero_tid = hero_tid order by like_num desc;
END //
DELIMITER ;

DELIMITER // 
DROP PROCEDURE IF EXISTS `update_role_data` //
CREATE PROCEDURE `update_role_data`()
	COMMENT '更新数据'
BEGIN
	DECLARE is_same_day INT DEFAULT 0;
	DECLARE t_error INT DEFAULT 0;
	DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET t_error=1;

	SELECT COUNT(*) INTO is_same_day FROM role_update WHERE DATEDIFF(role_update.last_update_time, NOW())=0;

	IF is_same_day = 0 THEN
		START TRANSACTION;
			#DELETE FROM role_like_data;
			#DELETE FROM role_unlike_data;
			DELETE FROM role_new_comment_data;
			UPDATE role_update SET role_update.last_update_time = NOW();
		IF t_error = 1 THEN
			ROLLBACK;
		ELSE
			COMMIT;
		END IF;
	END IF;
END //
DELIMITER ;

DELIMITER // 
DROP PROCEDURE IF EXISTS `update_role_new_comment` //
CREATE PROCEDURE `update_role_new_comment`(IN `role_uid` bigint,IN `hero_tid` int,OUT `result` int)
	COMMENT '更新新评论'
BEGIN
	DECLARE c_role_uid BIGINT DEFAULT 0;

	SELECT role_new_comment_data.role_uid INTO c_role_uid FROM role_new_comment_data WHERE role_new_comment_data.role_uid = role_uid AND role_new_comment_data.hero_tid = hero_tid;
	
	IF c_role_uid > 0 THEN
		SET result = 1;
	ELSE
		SET result = 0;
		INSERT INTO role_new_comment_data VALUES(role_uid, hero_tid);
	END IF;

END //
DELIMITER ;
