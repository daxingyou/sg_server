CREATE DATABASE IF NOT EXISTS `sg_up` DEFAULT CHARSET utf8 COLLATE utf8_general_ci;

USE `sg_up`;

CREATE TABLE IF NOT EXISTS `platform_info` (
    `channel_id` int(10) NOT NULL DEFAULT '0' COMMENT '渠道id',
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `platform_type` int(10) NOT NULL DEFAULT '0' COMMENT '平台类型',
    `platform_name` varchar(32) NOT NULL DEFAULT '0' COMMENT '平台名称',
    `channel_name` varchar(32) NOT NULL DEFAULT '0' COMMENT '渠道名称',
    `version` varchar(32) NOT NULL DEFAULT '0' COMMENT '客户端最新版本',
	 PRIMARY KEY (`channel_id`, `platform_type`)
)
COMMENT='平台信息表' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `account_server_list` (
    `channel_id` int(10) NOT NULL DEFAULT '0' COMMENT '渠道id',
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `server_name` varchar(32) NOT NULL DEFAULT '0' COMMENT '服务器名称',
    `server_id` int(10) NOT NULL DEFAULT '0' COMMENT '服务器id',
    `ip` varchar(32) NOT NULL DEFAULT '0' COMMENT '服务器ip',
    `port` varchar(32) NOT NULL DEFAULT '0' COMMENT '服务器port'
)
COMMENT='账号服务器列表' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `res_info` (
    `channel_id` int(10) NOT NULL DEFAULT '0' COMMENT '渠道id',
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `platform_type` int(10) NOT NULL DEFAULT '0' COMMENT '平台类型',
    `res_url` varchar(128) NOT NULL DEFAULT '0' COMMENT '资源url'
)
COMMENT='客户端热更新资源地址' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `notice_info` (
    `channel_id` int(10) NOT NULL DEFAULT '0' COMMENT '渠道id',
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `notice_url` varchar(128) NOT NULL DEFAULT '0' COMMENT '公告url'
)
COMMENT='公告地址' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `login_server_list` (
    `channel_id` int(10) NOT NULL DEFAULT '0' COMMENT '渠道id',
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `server_id` int(10) NOT NULL DEFAULT '0' COMMENT '服务器id',
    `status` int(10) NOT NULL DEFAULT '0' COMMENT '服务器状态',
    `server_name` varchar(32) NOT NULL DEFAULT '0' COMMENT '服务器名称',
    `label` int(10) NOT NULL DEFAULT '0' COMMENT '(新服/推荐)标签',
	 PRIMARY KEY (`channel_id`, `server_id`)
)
COMMENT='login服务器列表' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `compatible_version_info` (
    `channel_id` int(10) NOT NULL DEFAULT '0' COMMENT '渠道id',
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `version` varchar(128) NOT NULL DEFAULT '0' COMMENT 'version'
)
COMMENT='服务端兼容版本信息' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `plist_info` (
    `channel_id` int(10) NOT NULL DEFAULT '0' COMMENT '渠道id',
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `platform_type` int(10) NOT NULL DEFAULT '0' COMMENT '平台类型',
    `plist_url` varchar(128) NOT NULL DEFAULT '0' COMMENT 'plist_url'
)
COMMENT='plist信息' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `bbs_info` (
    `channel_id` int(10) NOT NULL DEFAULT '0' COMMENT '渠道id',
	`mark_type` varchar(128) NOT NULL DEFAULT '0' '渠道类型标记',
    `bbs_url` varchar(128) NOT NULL DEFAULT '0' COMMENT 'bbs_url'
)
COMMENT='bbs信息' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';
