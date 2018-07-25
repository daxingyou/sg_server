CREATE TABLE IF NOT EXISTS `role_exp_level_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`old_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '老等级',
    `old_exp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '老经验',
	`up_exp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '增加/减少经验',
	`new_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '新等级',
    `new_exp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '新经验',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='角色经验等级日志';

CREATE TABLE IF NOT EXISTS `hero_exp_level_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`role_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色等级',
    `hero_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '武将唯一ID',
	`hero_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '武将策划表ID',
	`old_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '老等级',
    `old_exp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '老经验',
	`up_exp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '增加经验',
    `new_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '新等级',
    `new_exp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '新经验',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`hero_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='武将经验等级日志';

CREATE TABLE IF NOT EXISTS `login_log` (
    `id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '自增主键',
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色UID',
    `gate_id` int(11) NOT NULL DEFAULT '0' COMMENT 'GateServer的ID',
    `game_id` int(11) NOT NULL DEFAULT '0' COMMENT 'GameServer的ID',
    `source_type` tinyint(1) NOT NULL DEFAULT '0' COMMENT '类型',
    `source_type_desc` varchar(1024) NOT NULL DEFAULT '' COMMENT '类型描述',
    `online_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '登出时在线时长(秒)',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY (`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='登入登出日志';

CREATE TABLE IF NOT EXISTS `item_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `role_level` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '角色等级',
    `item_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '道具唯一ID',
    `item_tid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '道具表ID',
    `old_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化前数目',
    `up_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化的数目',
    `new_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化后的数目',
    `op_type` tinyint(1) NOT NULL DEFAULT '0' COMMENT '增加或者减少:1表示增加，2表示扣除',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='道具日志';

CREATE TABLE IF NOT EXISTS `money_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `role_level` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '角色等级',
    `old_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化前数目',
    `up_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化的数目',
    `new_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化后的数目',
    `money_type` tinyint(1) NOT NULL DEFAULT '0' COMMENT '钱币类型:1金币，2隐蔽，3铜币',
    `op_type` tinyint(1) NOT NULL DEFAULT '0' COMMENT '增加或者减少:1表示增加，2表示扣除',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='钱币日志';

CREATE TABLE IF NOT EXISTS `hero_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `role_level` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '角色等级',
    `hero_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '武将唯一ID',
    `hero_tid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '武将表ID',  
    `op_type` tinyint(1) NOT NULL DEFAULT '0' COMMENT '增加或者减少:1表示增加，2表示扣除',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `old_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化前数目',
    `up_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化的数目',
    `new_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化后的数目',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='武将日志';

CREATE TABLE IF NOT EXISTS `task_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `task_id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '任务ID',
	`task_type` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '任务类型',
    `task_state` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '任务状态',
    `task_progress` varchar(500) NOT NULL COMMENT '任务进度',
    `accept_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '接取时间',
    `op_type` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '操作类型',
    `op_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '操作类型描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='任务日志';

CREATE TABLE IF NOT EXISTS `prestige_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `role_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色等级',
    `old_prestige` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '变化前声望点数',
    `old_prestige_total` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '变化前声望累计',
    `old_prestige_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '变化前声望等级',
    `up_prestige` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '变化的声望',
    `new_prestige` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '变化后声望点数',
    `new_prestige_total` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '变化后声望累计',
    `new_prestige_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '变化后声望等级',
    `op_type` tinyint(1) NOT NULL DEFAULT '0' COMMENT '增加或者减少:1表示增加，2表示扣除',
    `source_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '来源',
    `source_type_desc` varchar(1024) NOT NULL DEFAULT '' COMMENT '来源描述',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='声望日志';

CREATE TABLE IF NOT EXISTS `family_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
	`family_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '家族唯一ID',
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `target_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
	`source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
	`source_param2` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数2',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`family_uid`),
	INDEX(`role_uid`),
	INDEX(`target_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='家族日志';

CREATE TABLE IF NOT EXISTS `country_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
	`country_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '国家id',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
	`source_param1` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '来源参数',
	`source_param2` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
	`source_param3` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`country_id`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='国家日志';

CREATE TABLE IF NOT EXISTS `friend_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `friend_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '好友唯一ID',
    `old_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '变化前好友类型',
    `new_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '变化后好友类型',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='好友日志';

CREATE TABLE IF NOT EXISTS `mail_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `mail_id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '邮件唯一ID',
    `type` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '邮件类型',
    `item_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '邮件来源类型',
    `items` varchar(1024) NOT NULL DEFAULT '' COMMENT '附带道具',
    `item_state` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '附加道具状态',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(128) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='邮件日志';

CREATE TABLE IF NOT EXISTS `pk_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `role_level` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '角色等级',
    `old_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化前数目',
    `up_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化的数目',
    `new_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化后的数目',
    `value_type` tinyint(1) NOT NULL DEFAULT '0' COMMENT '钱币类型:1PK值 2侠义值',
    `op_type` tinyint(1) NOT NULL DEFAULT '0' COMMENT '增加或者减少:1表示增加，2表示扣除',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='PK日志';

CREATE TABLE IF NOT EXISTS `shop_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `goods_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '商品ID',
    `goods_num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '商品数量',
    `cost_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '消耗货币类型',
    `cost_value` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '消耗货币数量',
    `item_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '获得道具id',
    `item_num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '获得道具数量',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='商店日志';

CREATE TABLE IF NOT EXISTS `random_shop_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `shop_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '商店ID',
    `goods_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '商品ID',
    `cost_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '消耗货币类型',
    `cost_value` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '消耗货币数量',
	`cost_item` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '消耗道具id',
    `cost_item_value` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '消耗道具数量',
    `old_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化前数目',
    `new_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化后的数目',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='随机商店日志';

CREATE TABLE IF NOT EXISTS `equip_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `role_level` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '角色等级',
    `item_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '道具唯一ID',
    `item_tid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '道具表ID',
    `hero_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '已穿戴为所属武将uid，没穿戴为0',
    `attr` varchar(128) NOT NULL DEFAULT '' COMMENT '基础属性',
    `exattr` varchar(256) NOT NULL DEFAULT '' COMMENT '判词（附加属性）',
    `remake_exattr` varchar(256) NOT NULL DEFAULT '' COMMENT '重铸的判词',
    `op_type` tinyint(1) NOT NULL DEFAULT '0' COMMENT '增加或者减少或者修改:1表示增加，2表示扣除，3表示修改',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='装备日志';

CREATE TABLE IF NOT EXISTS `lifeforce_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `old_lifestate_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '升级前境界等级',
    `old_lifestar_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '升级前命星等级',
    `up_lifestate` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '境界等级变化量',
    `up_lifestar` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '命星等级变化量',
    `new_lifestate_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '升级后境界等级',
    `new_lifestar_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '升级后命星等级',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='命力日志';


CREATE TABLE IF NOT EXISTS `achieve_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `achieve_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '成就ID',
    `old_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '升级前等级',
    `up_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '等级改变值',
    `new_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '升级后等级',
    `old_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '升级前进度',
    `new_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '升级后进度',
    `old_achieve_point` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '升级前积分',
    `add_achieve_point` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '积分改变值',
    `new_achieve_point` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '升级后积分',
    `old_achieve_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '升级前成就等级',
    `up_achieve_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '成就等级改变值',
    `new_achieve_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '升级后成就等级',
    `old_reward_get_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '旧的奖励领取等级',
    `new_reward_get_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '新的奖励领取等级',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='成就日志';

CREATE TABLE IF NOT EXISTS `dungeon_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `dungeon_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '副本id',
    `create_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '开启类型',
    `create_id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '开启id(组队为队伍id，不组队是uid)',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='副本日志';

CREATE TABLE IF NOT EXISTS `role_buff_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `buff_id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'buffID',
    `op_type` tinyint(1) NOT NULL DEFAULT '0' COMMENT '增加或者减少或者修改:1表示增加，2表示扣除，3表示修改',
    `speed` int(11) NOT NULL DEFAULT '0' COMMENT '角色真实速度',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='buff日志';

CREATE TABLE IF NOT EXISTS `goods_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `role_name` varchar(20) NOT NULL DEFAULT '' COMMENT '角色名',
    `goods_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '商品唯一ID',
    `item_tid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '道具表ID',
    `item_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '道具数量',
    `price` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '出售价格',
    `selled_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '已经出售的数量',
    `profit` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '未领取的收益',
    `goods_state` int(11) NOT NULL DEFAULT '0' COMMENT '商品状态common.goods_state',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='寄售行日志';

CREATE TABLE IF NOT EXISTS `trade_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `item_tid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '道具表tid',
    `price` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '出售价格',
	`num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '出售数量',
	`tax` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '扣税',
    `total_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '当前数量',
    `total_cost` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '当前总花费',
    `gain` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '利润',
    `trade_id` int(11) NOT NULL DEFAULT '0' COMMENT '跑商ID',
    `op_type` int(11) NOT NULL DEFAULT '0' COMMENT '操作类型',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='跑商日志';

CREATE TABLE IF NOT EXISTS `new_guide_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `new_guide_id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '新手引导ID',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='新手引导日志';

CREATE TABLE IF NOT EXISTS `recharge_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `order_id` varchar(32) NOT NULL DEFAULT '' COMMENT '订单号',
    `recharge_tid` int(11) NOT NULL DEFAULT '0' COMMENT '充值id',
    `recharge_rmb` decimal(13,2) NOT NULL DEFAULT '0.00' COMMENT '充值金额',
	`channel_id` int(11) NOT NULL DEFAULT '0' COMMENT '充值渠道ID',
    `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '充值时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`order_id`),
    INDEX(`recharge_tid`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='充值日志';

CREATE TABLE IF NOT EXISTS `rmb_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`role_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色等级',
    `old_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化前数目',
    `up_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化的数目',
    `new_num` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化后的数目',
    `op_type` tinyint(1) NOT NULL DEFAULT '0' COMMENT '增加或者减少:1表示增加，2表示扣除',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '消费时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='人民币消费日志';

CREATE TABLE IF NOT EXISTS `expedition_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
    `role_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色等级',
    `source_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '来源',
    `source_type_desc` varchar(1024) NOT NULL DEFAULT '' COMMENT '来源描述',
    `source_param` int(11) NOT NULL DEFAULT '0' COMMENT '来源参数',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='远征日志';

CREATE TABLE IF NOT EXISTS `scene_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`old_map_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '旧地图id',
	`new_map_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '新地图id',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='场景日志';

CREATE TABLE IF NOT EXISTS `combat_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '发起战斗玩家UID',
    `role_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色等级',
    `fight_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '战斗类型',
    `fight_type_desc` varchar(20) NOT NULL DEFAULT '0' COMMENT '战斗类型描述',
    `combat_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '战斗方式',
    `combat_type_desc` varchar(20) NOT NULL DEFAULT '0' COMMENT '战斗方式描述',
    `team_member` varchar(128) NOT NULL DEFAULT '' COMMENT '队友信息',
    `targets` varchar(128) NOT NULL DEFAULT '' COMMENT '目标信息',
    `fight_uid` bigint(20) NOT NULL DEFAULT '0' COMMENT '战斗UID',
    `is_win` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '是否赢了',
    `start_time` timestamp NULL DEFAULT NULL COMMENT '开始时间',
    `fight_time` int(11) NOT NULL DEFAULT '0' COMMENT '战斗时长(秒)',
    `finish_time` timestamp NULL DEFAULT NULL COMMENT '结束时间',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`fight_type`),
    INDEX(`combat_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='战斗日志';

CREATE TABLE IF NOT EXISTS `redbag_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家ID',
    `role_name` varchar(20) NOT NULL DEFAULT '' COMMENT '角色名',
    `redbag_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '红包唯一ID',
    `total_count` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '总数量',
    `total_money` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '总金额',
	`draw_money` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '抢得金额',
    `rest_count` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '剩余数量',
    `rest_money` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '剩余金额',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
    `source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='红包日志';

CREATE TABLE IF NOT EXISTS `patrol_hire_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家ID',
    `begin_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '开始时间',
    `end_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '结束时间',
	`count` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '巡逻次数',
    `total_exp` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '累计获得经验',
    `use_multi_point` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '使用的多倍点数',
    `map_id` int(11) NOT NULL DEFAULT '0' COMMENT '巡逻场景',
    `source_type` int(11) NOT NULL DEFAULT '0' COMMENT '来源类型',
	`source_type_desc` varchar(255) NOT NULL DEFAULT '0' COMMENT '来源描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`source_type`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='雇佣巡逻日志';


CREATE TABLE IF NOT EXISTS `offline_arena_rank_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家ID',
    `role_name` varchar(20) NOT NULL DEFAULT '' COMMENT '角色名',
	`role_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色等级',
	`op_type` tinyint(1) NOT NULL DEFAULT '0' COMMENT '1表示首次进入 2表示分组晋级 3表示挑战成功排名上升 4表示被挑战排名下降',
	`old_class` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化前分组',
    `old_rank` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化前排名',
	`new_class` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化后分组',
    `new_rank` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '变化后排名',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    PRIMARY KEY(`id`),
    INDEX(`role_uid`),
    INDEX(`create_time`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='离线竞技排名变化日志';

CREATE TABLE IF NOT EXISTS `daily_statistics_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `type` varchar(255) DEFAULT NULL COMMENT '类型',
    `data` text COMMENT '分析结果',
    `date` date DEFAULT NULL COMMENT '针对日期',
    PRIMARY KEY(`id`),
    INDEX(`date`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='玩家留存分析结果日志';
