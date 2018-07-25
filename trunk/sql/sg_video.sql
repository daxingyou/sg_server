CREATE TABLE IF NOT EXISTS `cross_arena` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '录像id',
	`version` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '版本号',
	`group_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '分组id',
	`top` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否上榜 0 不上榜（默认值）1 上榜',
	`fight_data_base` blob COMMENT '对阵基础数据',
	`combat_data` blob COMMENT '玩家战斗数据',
  	PRIMARY KEY (`rk`),
	INDEX (`group_id`)
)
COMMENT='跨服竞技录像' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `cross_arena_role` (
	`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
	`uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'uid',
	`video_id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '录像id',
	`fight_data_base` blob COMMENT '对阵基础数据',
	PRIMARY KEY(`id`),
	INDEX (`uid`)
)
COMMENT='跨服录像个人索引' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `tower` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '录像id',
	`version` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '版本号',
	`group_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '服务器id',
	`layer` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '层数',
	`fighting` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '战力',
	`first` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否首通 0 不是（默认值）1 是',
	`limit` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否极限 0 不是（默认值）1 是',
	`recent` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否最近 0 不是（默认值）1 是',
	`fight_data_base` blob COMMENT '对阵基础数据',
	`combat_data` blob COMMENT '玩家战斗数据',
  	PRIMARY KEY (`rk`),
	INDEX (`group_id`)
)
COMMENT='爬塔录像' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';
 