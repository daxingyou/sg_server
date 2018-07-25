CREATE DATABASE IF NOT EXISTS `sg_gm` DEFAULT CHARSET utf8 COLLATE utf8_general_ci;

USE `sg_gm`;

CREATE TABLE IF NOT EXISTS `cdkey` (
    `channel_id` int(10) NOT NULL DEFAULT '0' COMMENT '渠道id',
    `key_code` varchar(128) NOT NULL DEFAULT '0' COMMENT 'cdkey',
    `key_id` int(10) NOT NULL DEFAULT '0' COMMENT 'cdkey_id',
    `key_type` int(10) NOT NULL DEFAULT '0' COMMENT 'cdkey类型',
    `use_count` int(10) NOT NULL DEFAULT '0' COMMENT '已使用次数',
    `max_use_count` int(10) NOT NULL DEFAULT '0' COMMENT '可使用次数',
    `limit_time` int(10) NOT NULL DEFAULT '0' COMMENT '使用结束时间',
	 PRIMARY KEY (`key_code`)
)
COMMENT='cdkey兑换表' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `use_cdkey` (
    `user_id`  bigint(10) NOT NULL DEFAULT '0' COMMENT '玩家uid',
    `key_code` varchar(128) NOT NULL DEFAULT '0' COMMENT 'cdkey',
    `key_id` int(10) NOT NULL DEFAULT '0' COMMENT 'cdkey_id',
    `key_type` int(10) NOT NULL DEFAULT '0' COMMENT 'cdkey类型',
    `use_time` int(10) NOT NULL DEFAULT '0' COMMENT '使用时间',
	 PRIMARY KEY (`user_id`, `key_code`)
)
COMMENT='cdkey兑换表' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

