CREATE TABLE IF NOT EXISTS `mobile_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT '设备唯一整型ID',
    `uuid` varchar(50) NOT NULL DEFAULT '' COMMENT '设备唯一标示符',
    `device_model` varchar(20) NOT NULL DEFAULT '' COMMENT '设备型号',
    `device_sys_version` varchar(20) NOT NULL DEFAULT '' COMMENT '操作系统',
    `cpu_type` varchar(20) NOT NULL DEFAULT '' COMMENT 'cpu类型',
    `cpu_count` int(10) NOT NULL DEFAULT '0' COMMENT 'cpu数量（单核还是多核）',
    `memory_size` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '设备内存',
    `grph_device_name` varchar(20) NOT NULL DEFAULT '' COMMENT '设备显卡型号',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    `create_date` date COMMENT '记录日期',
    PRIMARY KEY (`id`),
    UNIQUE (`uuid`),
    INDEX (`create_date`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='设备日志';

CREATE TABLE IF NOT EXISTS `account_log` (
    `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT '自增主键',
    `account_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '账号唯一ID',
    `channel_id` int(10) unsigned NOT NULL COMMENT '渠道编号id',
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '账户角色',
    `role_did` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '账户角色显示ID',
    `mobile_uuid` varchar(50) NOT NULL DEFAULT '' COMMENT '设备唯一标示符',
    `source_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '来源',
    `source_type_desc` varchar(1024) NOT NULL DEFAULT '' COMMENT '类型描述',
    `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '记录时间',
    `create_date` date COMMENT '记录日期',
    PRIMARY KEY (`id`),
    INDEX (`account_uid`),
    INDEX (`mobile_uuid`),
    INDEX (`role_did`),
    INDEX (`source_type`),
    INDEX (`create_date`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='账户日志';