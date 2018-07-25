CREATE TABLE `new_user_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `time` date COMMENT '日期',
    `mobile` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '新增设备数量',
    `reg_mobile` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '注册设备数量',
    `account` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '新增账号数量',
    `role` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '新增角色数量',
    `rate` float NOT NULL DEFAULT '0' COMMENT '设备账号转化率',
    PRIMARY KEY (`id`),
    UNIQUE (`time`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='新增玩家';

CREATE TABLE IF NOT EXISTS `active_user_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `time` date COMMENT '日期',
    `dau` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '日活跃数量',
    `wau` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '周活跃数量',
    `mau` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '月活跃数量',
    `dau/mau` float NOT NULL DEFAULT '0' COMMENT '日活跃占月活跃的百分比',
    PRIMARY KEY (`id`),
    UNIQUE (`time`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='活跃玩家(账号)';

CREATE TABLE IF NOT EXISTS `remain_user_log` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `time` date DEFAULT NULL,
  `second_day` float NOT NULL DEFAULT '0',
  `seventh_day` float NOT NULL DEFAULT '0',
  `thirtieth_day` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE (`time`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='留存玩家';

CREATE TABLE IF NOT EXISTS `leave_retrun_user_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `time` date,
    `leave_count` int(11) unsigned NOT NULL DEFAULT '0',
    `leave_rate` float NOT NULL DEFAULT '0.00',
    `return_count` int(11) unsigned NOT NULL DEFAULT '0',
	`interval_days` int(11) unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`id`),
    UNIQUE (`time`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='流失和回流玩家';

CREATE TABLE IF NOT EXISTS `role_level_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
    `level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '等级',
    `level_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '等级数量',
    `role_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色总数',
    `rate` float NOT NULL DEFAULT '0' COMMENT '比例',
    PRIMARY KEY (`id`),
    UNIQUE (`level`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='角色等级统计';