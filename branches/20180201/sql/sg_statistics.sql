CREATE DATABASE IF NOT EXISTS `sg_statistics` DEFAULT CHARSET utf8 COLLATE utf8_general_ci;

USE `sg_statistics`;

#################################################################################################################
#####新增玩家
#################################################################################################################
DROP TABLE IF EXISTS`new_user_log`;
CREATE TABLE `new_user_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `time` date COMMENT '日期',
    `mobile` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '新增设备数量',
    `reg_mobile` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '注册设备数量',
    `account` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '新增账号数量',
    `role` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '新增角色数量',
    `rate` float NOT NULL DEFAULT '0' COMMENT '设备账号转化率',
    PRIMARY KEY (`id`),
    UNIQUE (`time`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='新增玩家';

DELIMITER // 
DROP PROCEDURE IF EXISTS `sp_new_user` //
CREATE PROCEDURE `sp_new_user`(start_time VARCHAR(20), end_time VARCHAR(20))
    COMMENT '新增玩家(设备|账号|角色)'
BEGIN

DECLARE diff_day INT(11) DEFAULT DATEDIFF(DATE(end_time), DATE(start_time));
DECLARE i INT(11)  DEFAULT 0;
DECLARE find_date DATE;

DECLARE new_mobile_count INT(11) DEFAULT 0;
DECLARE new_account_count INT(11) DEFAULT 0;
DECLARE new_role_count INT(11) DEFAULT 0;
DECLARE reg_mobile_count INT(11) DEFAULT 0;
DECLARE cov_rate FLOAT DEFAULT 0;

WHILE i <= diff_day do
    SET find_date = DATE_SUB(DATE(end_time), INTERVAL i DAY);
    
    #新增设备
    SET new_mobile_count = 0;
    SELECT COUNT(`id`) INTO new_mobile_count FROM sg_glog.`mobile_log`  WHERE `create_date` = find_date;
    
    #新增账号 注册设备
    SET new_account_count = 0;
    SET reg_mobile_count = 0;
    SELECT COUNT(DISTINCT `account_uid`), COUNT(DISTINCT `mobile_uuid`) INTO new_account_count, reg_mobile_count FROM sg_glog.`account_log` WHERE `source_type` = 1560 AND `create_date` = find_date;
    
    #设备账号转化率
    SET cov_rate = 0;
    IF reg_mobile_count > 0 AND reg_mobile_count <= new_mobile_count THEN
        SET cov_rate = ROUND(reg_mobile_count / new_mobile_count * 100, 1);
    ELSE
        SET cov_rate = 0;
    END IF;
    
    #新增角色
    SET new_role_count = 0;
    SELECT COUNT(DISTINCT a.`role_uid`) INTO new_role_count FROM sg_glog.`account_log` AS a WHERE a.`source_type` = 1562 AND a.`create_date` = find_date;
    
    INSERT IGNORE INTO `new_user_log`(`time`, `mobile`, `account`, `role`, `rate`) VALUES(find_date, new_mobile_count, new_account_count, new_role_count, cov_rate);
    
    SET i = i + 1;
END WHILE;

#SELECT `time` AS '日期', `mobile` AS '新增设备', `account` AS '新增账号', `role` AS '新增角色', `rate` AS '设备账号转化率(%)' FROM `new_user_log` ORDER BY `time`;

END //
DELIMITER ;

#################################################################################################################
#####活跃玩家
#################################################################################################################
DROP TABLE IF EXISTS`active_user_log`;
CREATE TABLE `active_user_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `time` date COMMENT '日期',
    `dau` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '日活跃数量',
    `wau` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '周活跃数量',
    `mau` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '月活跃数量',
    `dau/mau` float NOT NULL DEFAULT '0.00' COMMENT '日活跃占月活跃的百分比',
    PRIMARY KEY (`id`),
    UNIQUE (`time`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='活跃玩家(账号)';

DELIMITER // 
DROP PROCEDURE IF EXISTS `sp_active_user` //
CREATE PROCEDURE `sp_active_user`(start_time VARCHAR(20), end_time VARCHAR(20))
    COMMENT '活跃玩家(账号)'
BEGIN 

#当日往前推n日(当日计入天数)期间内，登陆过游戏的玩家总数量，按照玩家ID排重。

DECLARE i INT(11) DEFAULT 0;
DECLARE diff_day INT(11) DEFAULT DATEDIFF(DATE(end_time), DATE(start_time));
DECLARE find_date DATE;

DECLARE find_min_date DATE;
DECLARE find_max_date DATE;
DECLARE dau_count INT(11) DEFAULT 0;
DECLARE wau_count INT(11) DEFAULT 0;
DECLARE mau_count INT(11) DEFAULT 0;
DECLARE rate FLOAT DEFAULT 0;

WHILE i <= diff_day do
    SET find_date = DATE_SUB(DATE(end_time), INTERVAL i DAY);
    
    #dau_count 6.30
    SET dau_count = 0;
    SELECT COUNT(DISTINCT `account_uid`) INTO dau_count FROM sg_glog.`account_log` WHERE `create_date` = find_date;
    
    #wau_count 6.30~6.24
    SET find_max_date = find_date;
    SET find_min_date = DATE_SUB(find_date, INTERVAL 6 DAY);
    SET wau_count = 0;
    SELECT COUNT(DISTINCT `account_uid`) INTO wau_count FROM sg_glog.`account_log` WHERE `create_date` >= find_min_date AND `create_date` <= find_max_date;
    
    #mau_count 6.30~6.1
    SET find_max_date = find_date;
    SET find_min_date = DATE_SUB(find_date, INTERVAL 29 DAY);  #6.17
    SET mau_count = 0;
    SELECT COUNT(DISTINCT `account_uid`) INTO mau_count FROM sg_glog.`account_log` WHERE `create_date` >= find_min_date AND `create_date` <= find_max_date;
    
    #dau_count/mau_count
    IF dau_count > 0 AND dau_count <= mau_count THEN
        SET rate = ROUND(dau_count / mau_count, 2);
    ELSE
        SET rate = 0;
    END IF;
    
    INSERT IGNORE INTO `active_user_log`(`time`, `dau`, `wau`, `mau`, `dau/mau`) VALUES(find_date, dau_count, wau_count, mau_count, rate);
    
    SET i = i + 1;
END WHILE;

#SELECT `time` AS '日期', `dau` AS 'DAU(账号)', `wau` AS 'WAU(账号)', `mau` AS 'MAU(账号)', `dau/mau` AS 'DAU/MAU' FROM `active_user_log` ORDER BY `time`;

END //
DELIMITER ;

#################################################################################################################
#####留存玩家
#################################################################################################################
DROP TABLE IF EXISTS`remain_user_log`;
CREATE TABLE IF NOT EXISTS `remain_user_log` (
  `id` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT,
  `time` date DEFAULT NULL,
  `second_day` float NOT NULL DEFAULT '0',
  `seventh_day` float NOT NULL DEFAULT '0',
  `thirtieth_day` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `time` (`time`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='留存玩家';

DELIMITER // 
DROP PROCEDURE IF EXISTS `sp_remain_user` //
CREATE PROCEDURE `sp_remain_user`(start_time VARCHAR(20), end_time VARCHAR(20))
    COMMENT '留存玩家(账号)'
BEGIN 

#留存率：当日往后推第n日(当日不计入天数)活跃玩家数量占当日新增玩家数量的比例。
#查询7月9日的次日留存：7.9活跃-->7.10留存
#查询7月9日的7日留存：7.9活跃-->7.8留存
#查询7月9日的30日留存：7.9活跃-->7.31留存

DECLARE diff_day INT(11) DEFAULT DATEDIFF(DATE(end_time), DATE(start_time));
DECLARE i INT(11) DEFAULT 0;

DECLARE cur_date VARCHAR(20);
DECLARE cur_count INT(11) DEFAULT 0;

DECLARE next_date DATE;
DECLARE next_count INT(11) DEFAULT 0;

DECLARE second_rate FLOAT DEFAULT 0;
DECLARE seventh_rate FLOAT DEFAULT 0;
DECLARE thirtieth_rate FLOAT DEFAULT 0;

WHILE i <= diff_day DO
    SET cur_date = DATE_SUB(DATE(end_time), INTERVAL i DAY);
    
    #当日新增:7.1 
    SELECT COUNT(DISTINCT `account_uid`) INTO cur_count FROM sg_glog.`account_log` AS a WHERE `source_type` = 1560 AND cur_date = `create_date`;
    
    IF cur_count > 0 THEN           

        #次日活跃:7.2
        SET next_date = DATE_ADD(cur_date, INTERVAL 1 DAY);
        SELECT COUNT(DISTINCT a.`account_uid`) INTO next_count FROM sg_glog.`account_log` AS a 
        LEFT JOIN sg_glog.`account_log` AS b 
        ON a.`account_uid` = b.`account_uid`   
        WHERE b.`create_date` = next_date AND a.`source_type` = 1560 AND a.`create_date` = cur_date;        
        IF next_count > 0 AND next_count <= cur_count THEN
            SET second_rate = ROUND(next_count / cur_count * 100, 1);
        ELSE
            SET second_rate = 0;
        END IF;
        
        #7日活跃:7.8
        SET next_date = DATE_ADD(cur_date, INTERVAL 7 DAY);
        SELECT COUNT(DISTINCT a.`account_uid`) INTO next_count FROM sg_glog.`account_log` AS a 
        LEFT JOIN sg_glog.`account_log` AS b 
        ON a.`account_uid` = b.`account_uid`   
        WHERE b.`create_date` = next_date AND a.`source_type` = 1560 AND a.`create_date` = cur_date;  
        IF next_count > 0 AND next_count <= cur_count THEN
            SET seventh_rate = ROUND(next_count / cur_count * 100, 1);
        ELSE
            SET seventh_rate = 0;
        END IF;
        
        #30日活跃:7.31
        SET next_date = DATE_ADD(cur_date, INTERVAL 30 DAY);
        SELECT COUNT(DISTINCT a.`account_uid`) INTO next_count FROM sg_glog.`account_log` AS a 
        LEFT JOIN sg_glog.`account_log` AS b 
        ON a.`account_uid` = b.`account_uid`   
        WHERE b.`create_date` = next_date AND a.`source_type` = 1560 AND a.`create_date` = cur_date;   
        IF next_count > 0 AND next_count <= cur_count THEN
            SET thirtieth_rate = ROUND(next_count / cur_count * 100, 1);
        ELSE
            SET thirtieth_rate = 0;
        END IF; 
    END IF;
  
    INSERT IGNORE INTO `remain_user_log`(`time`, `second_day`, `seventh_day`, `thirtieth_day`) VALUES(cur_date, second_rate, seventh_rate, thirtieth_rate);
        
    SET i = i + 1;
END WHILE;

#SELECT `time` AS '日期', `second_day` AS '次日留存(%)', `seventh_day` AS '7日留存(%)', `thirtieth_day` AS '30日留存(%)' FROM `remain_user_log` ORDER BY `time`;

END //
DELIMITER ;

#################################################################################################################
#####流失和回流玩家
#################################################################################################################
DROP TABLE IF EXISTS`leave_retrun_user_log`;
CREATE TABLE IF NOT EXISTS `leave_retrun_user_log` (
    `id` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT,
    `time` date,
    `leave_count` int(11) UNSIGNED NOT NULL DEFAULT '0',
    `leave_rate` float NOT NULL DEFAULT '0.00',
    `return_count` int(11) UNSIGNED NOT NULL DEFAULT '0',
    PRIMARY KEY (`id`),
    UNIQUE (`time`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='流失和回流玩家';

DELIMITER // 
DROP PROCEDURE IF EXISTS `sp_leave_return_user` //
CREATE PROCEDURE `sp_leave_return_user`(start_time VARCHAR(20), end_time VARCHAR(20))
    COMMENT '流失和回流玩家(账号)'
BEGIN 

#流失：当日活跃账号在当日往后推(当日不计入天数)连续n日(默认7日)未登陆游戏的账号，即为流失账号
#查询7月9日的7日流失：7.9活跃-->7.10~7.16流失

#回流：当日活跃账号在当日往后推(当日不计入天数)连续n日(默认7日)未登陆游戏的账号，在第n+1日登陆了游戏，即为回流账号
#查询7月9日的7日回流：7.9活跃-->7.10~7.16流失-->7.17回流

DECLARE diff_day INT(11) DEFAULT DATEDIFF(DATE(end_time), DATE(start_time));
DECLARE i INT(11) DEFAULT 0;

#默认连续7日未登陆游戏为流失标准
DECLARE continue_day INT(11) DEFAULT 7;

DECLARE cur_date DATE;
DECLARE active_count INT(11) DEFAULT 0;

DECLARE leave_min_date DATE;
DECLARE leave_max_date DATE;
DECLARE lct INT(11) DEFAULT 0;
DECLARE lrt FLOAT DEFAULT 0;

DECLARE return_date DATE;
DECLARE rct INT(11) DEFAULT 0;

WHILE i <= diff_day do
    SET cur_date = DATE_SUB(DATE(end_time), INTERVAL i DAY); #7.9 活跃
    SET leave_min_date = DATE_ADD(cur_date, INTERVAL 1 DAY); #7.10 流失
    SET leave_max_date = DATE_ADD(cur_date, INTERVAL continue_day DAY);#7.16 流失
    SET return_date = DATE_ADD(cur_date, INTERVAL continue_day+1 DAY);#7.17 回流
    
    SET active_count = 0;
    SET lct = 0;
    SET lrt = 0;
    SET rct = 0;
     
    #当日活跃账号
    SELECT COUNT(DISTINCT `account_uid`) INTO active_count FROM sg_glog.`account_log` WHERE `create_date` = cur_date;

    #连续n日流失账号
    SELECT COUNT(DISTINCT a.`account_uid`) INTO lct FROM sg_glog.`account_log` AS a 
    LEFT JOIN sg_glog.`account_log` AS b 
    ON a.`account_uid` = b.`account_uid` AND b.`create_date` >= leave_min_date AND b.`create_date` <= leave_max_date
    WHERE b.`account_uid` IS NULL AND cur_date = a.`create_date`;
    
    #流失率
    IF lct > 0 AND lct <= active_count THEN
        SET lrt = ROUND(lct / active_count * 100, 1);
    ELSE
        SET lrt = 0;
    END IF;
    
    #第n+1日回流账号
    SELECT COUNT(r.`account_uid`) INTO rct FROM sg_glog.`account_log` AS r 
    LEFT JOIN (SELECT DISTINCT a.`account_uid` FROM sg_glog.`account_log` AS a 
                LEFT JOIN sg_glog.`account_log` AS b 
                ON a.`account_uid` = b.`account_uid` AND b.`create_date` >= leave_min_date AND b.`create_date` <= leave_max_date
                WHERE b.`account_uid` IS NULL AND cur_date = a.`create_date`) AS l 
    ON r.`account_uid` = l.`account_uid` 
    WHERE l.`account_uid` IS NOT NULL AND r.`create_date` = return_date;
    
    INSERT IGNORE INTO `leave_retrun_user_log`(`time`, `leave_count`, `leave_rate`, `return_count`) 
    VALUES(cur_date, lct, lrt, rct);
    
    SET i = i + 1;
END WHILE;

#SELECT `time` AS '日期', `leave_count` AS '7日流失数(账号)', `leave_rate` AS '7日流失率(%)', `return_count` AS '7日回流数(账号)' FROM `leave_retrun_user_log` ORDER BY `time`;

END //
DELIMITER ;

