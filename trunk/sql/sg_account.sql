CREATE TABLE IF NOT EXISTS `account` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT '账号唯一ID',
    `guid` varchar(50) NOT NULL DEFAULT '' COMMENT '渠道账号唯一ID',
    `channel_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '渠道ID',
    `regist_time` datetime DEFAULT NULL COMMENT '注册时间',
    PRIMARY KEY (`id`),
    INDEX (`guid`),
    INDEX (`channel_id`)
) 
COMMENT='账户表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `role` (
    `uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `account_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '账号唯一ID',
    `channel_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '渠道ID',
    `name` varchar(20) NOT NULL DEFAULT '' COMMENT '角色名',
    `species` int(10) unsigned NOT NULL COMMENT '职业',
    `create_time` varchar(64) NOT NULL DEFAULT '' COMMENT '创建时间',
    `server_id` int(10) unsigned NOT NULL COMMENT '服务器ID',
    `level` int(10) unsigned NOT NULL COMMENT '玩家等级',
    `plugin` int(10) unsigned NOT NULL COMMENT '模型ID',
    PRIMARY KEY (`uid`),
    INDEX (`account_uid`)
) 
COMMENT='角色表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';
