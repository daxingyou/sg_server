CREATE DATABASE IF NOT EXISTS `sg_up` DEFAULT CHARSET utf8 COLLATE utf8_general_ci;

USE `sg_up`;

CREATE TABLE IF NOT EXISTS `platform_info` (
	`mark_type` varchar(128) NOT NULL DEFAULT '0' COMMENT '渠道类型标记',
    `platform_name` varchar(32) NOT NULL DEFAULT '0' COMMENT '平台名称',
    `channel_name` varchar(32) NOT NULL DEFAULT '0' COMMENT '渠道名称',
    `version` varchar(32) NOT NULL DEFAULT '0' COMMENT '客户端最新版本',
	`compatible_version` varchar(256) NOT NULL DEFAULT '' COMMENT '兼容版本号',
	PRIMARY KEY (`mark_type`)
)
COMMENT='平台信息表' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `account_server_list` (
	`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `server_name` varchar(32) NOT NULL DEFAULT '0' COMMENT '服务器名称',
    `ip` varchar(32) NOT NULL DEFAULT '0' COMMENT '服务器ip',
    `port` varchar(32) NOT NULL DEFAULT '0' COMMENT '服务器port',
	PRIMARY KEY (`id`),
	UNIQUE KEY `catename` (`mark_type`, `ip`, `port`)  
)
COMMENT='账号服务器列表' ENGINE=InnoDB AUTO_INCREMENT=1 CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `res_info` (
	`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `res_url` varchar(128) NOT NULL DEFAULT '0' COMMENT '资源url',
	PRIMARY KEY (`id`),
	UNIQUE KEY `catename` (`mark_type`, `res_url`)  
)
COMMENT='客户端热更新资源地址' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `notice_info` (
	`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `notice_url` varchar(128) NOT NULL DEFAULT '0' COMMENT '公告url',
	PRIMARY KEY (`id`),
	UNIQUE KEY `catename` (`mark_type`, `notice_url`)  
)
COMMENT='公告地址' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `login_server_list` (
	`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `server_id` int(10) NOT NULL DEFAULT '0' COMMENT '服务器id',
    `status` int(10) NOT NULL DEFAULT '0' COMMENT '服务器状态',
    `server_name` varchar(32) NOT NULL DEFAULT '0' COMMENT '服务器名称',
    `label` int(10) NOT NULL DEFAULT '0' COMMENT '(新服/推荐)标签',
	PRIMARY KEY (`id`),
	UNIQUE KEY `catename` (`mark_type`, `server_id`) 
)
COMMENT='login服务器列表' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `plist_info` (
	`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `plist_url` varchar(128) NOT NULL DEFAULT '0' COMMENT 'plist_url',
	PRIMARY KEY (`id`),
	UNIQUE KEY `catename` (`mark_type`, `plist_url`) 
)
COMMENT='plist信息' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `bbs_info` (
	`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `bbs_url` varchar(128) NOT NULL DEFAULT '0' COMMENT 'bbs_url',
	PRIMARY KEY (`id`),
	UNIQUE KEY `catename` (`mark_type`, `bbs_url`) 
)
COMMENT='bbs信息' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `encrypt` (
	`key` varchar(128) NOT NULL DEFAULT '0' '加密key',
    `iv` varchar(128) NOT NULL DEFAULT '0' COMMENT '加密iv'
)
COMMENT='客户端加密数据' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';


CREATE TABLE IF NOT EXISTS `combat_view` (
	`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `combat_url` varchar(128) NOT NULL DEFAULT '0' COMMENT 'combat_url',
	PRIMARY KEY (`id`),
	UNIQUE KEY `catename` (`mark_type`, `combat_url`) 
)
COMMENT='战斗回放数据url' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

