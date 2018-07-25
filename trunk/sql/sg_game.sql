CREATE TABLE IF NOT EXISTS `role` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`gm` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '0是非GM 1是GM',
  	`name` varchar(20) NOT NULL DEFAULT '' COMMENT '角色名',
  	`last_login_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '上次上线时间',
    `last_logout_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '上次下线时间',
    `continue_login_days` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '连续登录天数',
    `cumulative_login_days` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '累计登陆天数',
	`update_day_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '每日更新的时间',
    `update_week_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '每周更新的时间',
  	`map_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '地图tid',
  	`pos_x` float NOT NULL DEFAULT '0' COMMENT '坐标X',
  	`pos_y` float NOT NULL DEFAULT '0' COMMENT '坐标Y',
	`dir` float NOT NULL DEFAULT '0' COMMENT '朝向',
	`last_map_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上个静态场景地图tid',
  	`last_pos_x` float NOT NULL DEFAULT '0' COMMENT '上个静态场景坐标X',
  	`last_pos_y` float NOT NULL DEFAULT '0' COMMENT '上个静态场景坐标Y',
	`last_dir` float NOT NULL DEFAULT '0' COMMENT '上个静态场景朝向',
  	`create_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '角色创建时间',
  	`last_power_update_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '上次体力的更新时间',
	`fighting` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '战斗力',
  	`city_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '当前城市ID',
	`garrison_city` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '挂机城市tid',
	`offline_reward_calc_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '挂机离线奖励',
	`luck_gift_gold` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '红包返还的金币',
	`hero_uid` bigint(20) NOT NULL DEFAULT '0' COMMENT '主公武将的uid',
	`task_type_data` varchar(128) DEFAULT '' COMMENT '任务类型次数',
	`exp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '经验',
	`level` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '等级',
	`plugin` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '模型',
	`function_code` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '功能开启',
	`npc_code` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '控制npc的显示隐藏',
	`collect_code` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '控制采集物的显示隐藏',
	`role_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '初始创建角色选择',
	`channel_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '渠道id',
	`head_frame` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '头像和边框',
  	PRIMARY KEY (`rk`)
)
 COMMENT='角色表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

 CREATE TABLE IF NOT EXISTS `role_ex` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`cur_form_id` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '当前使用的阵型',
  	`formation` varchar(1000) NOT NULL DEFAULT '' COMMENT '布阵数据',
	`equip_smelt_box` varchar(500) NOT NULL DEFAULT '' COMMENT '已领取的熔炼度宝箱',
	`equip_smelt_value` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '熔炼度',
	`guide` varchar(256) NOT NULL DEFAULT '' COMMENT '准备完成和真正完成的新手引导列表', 
	`tactic` varchar(256) NOT NULL DEFAULT '' COMMENT '拥有的阵法数据',
	`buff` varchar(256) NOT NULL DEFAULT '' COMMENT '携带的buff数据',
	`trade_point` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '跑商贸易点',
	`trade_last_reset_buy_num_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '跑商上一次重置贸易购买数量时间',
	`trade_week_total_gain` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '跑商周累计盈利',
	`page_pass_list` varchar(500) NOT NULL DEFAULT '' COMMENT '章节通关列表',
	`active_reward_task_list` varchar(500) NOT NULL DEFAULT '' COMMENT '活跃奖励任务列表',
	`page_task_pass_list` varchar(500) NOT NULL DEFAULT '' COMMENT '章节任务通关列表',
	`patrol_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '巡逻场次',
	`level_reward` varchar(256) NOT NULL DEFAULT '' COMMENT '等级奖励',
	`equip_lucky` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '装备打造幸运值',
	`time_long_multi_list` varchar(500) NOT NULL DEFAULT '' COMMENT '时长多倍列表',
	`multi_state` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '开启状态',
	`mode_state` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色状态',
	`page_elite_pass_list` varchar(500) NOT NULL DEFAULT '' COMMENT '章节精通关列表',
	`level_reward_mail` varchar(256) NOT NULL DEFAULT '' COMMENT '等级奖励邮件',
	`quick_fight_list` varchar(500) NOT NULL DEFAULT '' COMMENT '快速扫荡列表',
	`reset_quick_fight_list` varchar(500) NOT NULL DEFAULT '' COMMENT '充值扫荡精英副本',
	`lifeforce_label` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '命师称号',
	`head_frame_list` varchar(600) NOT NULL DEFAULT '' COMMENT '头像和边框列表',
	`wusheng_score` int(10) NOT NULL DEFAULT '0' COMMENT '武圣宝库积分',
	`wusheng_score_today` int(10) NOT NULL DEFAULT '0' COMMENT '武圣宝库今日积分',
	`wusheng_lucky` int(10) NOT NULL DEFAULT '0' COMMENT '武圣宝库幸运值',
	`general_event_data` varchar(600) NOT NULL DEFAULT '0' COMMENT '天下大势状态',
	`general_event_red` varchar(600) NOT NULL DEFAULT '0' COMMENT '天下大势红点数据',
	`career_id` varchar(256) NOT NULL DEFAULT '' COMMENT '已经领取的成长历程奖励',
	`change_name` int(10) NOT NULL DEFAULT '0' COMMENT '是否改过名字',
	`title_id` varchar(256) NOT NULL DEFAULT '' COMMENT '已经获得的称号',
	`cur_title` int(10) NOT NULL DEFAULT '0' COMMENT '当前使用的称号',
	`wusheng_today_buy` int(10) NOT NULL DEFAULT '0' COMMENT '武圣宝库今日购买次数',
	`handbook` varchar(256) NOT NULL DEFAULT '' COMMENT '已经领取的收集图鉴奖励',
  	PRIMARY KEY (`rk`)
)
COMMENT='角色额外数据表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

 CREATE TABLE IF NOT EXISTS `role_money` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`yuanbao` bigint(10) unsigned NOT NULL DEFAULT '0' COMMENT '元宝',
  	`silver` bigint(10) unsigned NOT NULL DEFAULT '0' COMMENT '银币',
  	`copper` bigint(10) unsigned NOT NULL DEFAULT '0' COMMENT '铜币',
	`errantry` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '侠义值',
	`energy` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '活力值',
	`prestige` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '声望',
	`prestige_total` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '累计声望',
	`prestige_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '声望等级',
	`prestige_today` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '今日获得的声望',
	`achieve_point` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '成就点',
	`achieve_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '成就等级',
	`achieve_recv_reward_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '成就领取等级',
	`lifeforce` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '命力值',
	`lifestar_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '命星等级',
	`lifestate_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '境界等级',
	`life_stone` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '充值金额',
	`contribution` bigint(10) unsigned NOT NULL DEFAULT '0' COMMENT '功勋',
	`herosoul` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '将魂',
	`honor` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '荣誉',
	`honor_week` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '本周获得的荣誉',
	`honor_limit` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '本周获得的荣誉上限',
	`multi_point` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '多倍点数',
	`role_unlock` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '主角解锁',
	`vigour` bigint(10) unsigned NOT NULL DEFAULT '0' COMMENT '精力',
	`vigour_buy` bigint(10) unsigned NOT NULL DEFAULT '0' COMMENT '精力购买次数',
	`vigour_time` bigint(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次精力增加时间',
	`friendship_point` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '友情点',
  	PRIMARY KEY (`rk`)
)
COMMENT='角色资源表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `role_fight` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`fight_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '战斗ID',
	`fight_study_step` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '切磋阶段',
	`fight_study_step_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '切磋阶段时间',
	`fight_study_target` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '切磋目标',
	`fight_study_attacker` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '切磋发起者',
	`fight_study_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '切磋时间',
  	`pk_punishment` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'PK惩罚时间',
	`pk_value` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'PK值',
	`pk_value_reduce_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '还剩多少时间降低PK值',
	`pk_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '剩余发起PK次数',
	`target_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '剩余被攻击次数',
	`auto_fight_flag` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '自动战斗标记',
	`ignore_lineup_flag` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '忽略布阵标记',
	
  	PRIMARY KEY (`rk`)
)
COMMENT='角色战斗表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `role_tower` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`record_tower` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '最高层数',
	`current_tower` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '当前层数',
	`reset_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '重置次数',
	`max_reset_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '最大重置次数',
	`auto_fight_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '扫荡结束时间',
	`current_achieve` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '当前爬塔成就',
	`first_day` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '通关第一层的时间',
	`tower_trigger_data` blob COMMENT '隐藏关卡数据',
	`trigger_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '剩余触发次数',
	`trigger_fight_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '剩余攻打次数',
	`send_mail_flag` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '发送扫荡邮件标记',
  	PRIMARY KEY (`rk`)
)
COMMENT='角色爬塔表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `random_shop` (
	`rk` varchar(32) NOT NULL DEFAULT '0' COMMENT '角色id+shop_id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`role_uid` bigint(20) NOT NULL DEFAULT '0' COMMENT '角色uid',
  	`shop_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '商店id',
	`free_refresh` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '免费刷新次数',
	`pay_refresh` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '付费刷新次数',
	`next_refresh_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '下次刷新时间',
	`shop_item` varchar(512) NOT NULL DEFAULT '' COMMENT '商店物品和状态',
  	PRIMARY KEY (`rk`),
	INDEX (`role_uid`)
)
COMMENT='角色随机商店表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `country_bounty` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`country_bounty` varchar(500) NOT NULL DEFAULT '' COMMENT '国家悬赏数据',
	`star_box` varchar(500) NOT NULL DEFAULT '' COMMENT '已领取的星级宝箱',
	`refresh_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '剩余刷新次数',
	`star_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '星数',
	`accept_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '接取等级',
  	PRIMARY KEY (`rk`)
)
COMMENT='国家悬赏表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `expedition` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
  	`expedition_data` blob COMMENT '远征数据',
  	PRIMARY KEY (`rk`)
)
COMMENT='远征表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `activity` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
  	`activities_data` blob COMMENT '活动数据',
  	PRIMARY KEY (`rk`)
)
COMMENT='活动表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `luckydraw` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
  	`luckydraw_data` blob COMMENT '抽卡数据',
  	PRIMARY KEY (`rk`)
)
COMMENT='抽卡表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `role_mount` (
	`rk` varchar(32) NOT NULL DEFAULT '0' COMMENT '角色id+mount_tid',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`role_uid` bigint(20) NOT NULL DEFAULT '0' COMMENT '角色uid',
  	`mount_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '坐骑tid',
  	`mount_state` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '坐骑状态',
  	PRIMARY KEY (`rk`),
    INDEX (`role_uid`)
)
COMMENT='角色坐骑数据表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `heroes` (
	`rk` bigint(20) unsigned NOT NULL COMMENT '武将唯一id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`role_uid` bigint(20) unsigned NOT NULL COMMENT '武将所属主公id',
	`hero_id` int(10) unsigned NOT NULL COMMENT '武将对应配置表id',
	`level` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '武将或主公等级',
	`exp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '武将或主公经验',
	`species` int(10) unsigned NOT NULL COMMENT '武将或主公职业',
	`country` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '武将所属阵营',
	`grade` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '武将品阶',
	`star` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '武将星级',
	`degree_point` int(10) unsigned NOT NULL COMMENT '当前武将段位点数 10点为1段，当前星级多少段读表',
	`talent_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '觉悟等级',
	`talent_point` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '当前觉悟已激活的点数',
	`talent_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '天赋点数',
	`exp_today` int(10) unsigned NOT NULL COMMENT '今日已获得的经验',
	`talent_use` int(10) unsigned NOT NULL COMMENT '已经使用的天赋点数',
	`skill_data` blob COMMENT '技能数据',
	`talent_data` blob COMMENT '天赋数据',
	`wakeup` int(10) unsigned NOT NULL COMMENT '觉醒标识0为未觉醒1为已觉醒',
	`active_talent_use` int(10) unsigned NOT NULL COMMENT '觉醒已经使用的天赋点数',
	`equip` varchar(256) NOT NULL DEFAULT '' COMMENT '装备信息',
	`plugins` varchar(256) NOT NULL DEFAULT '' COMMENT '拥有的皮肤',
	`current_plugin` int(10) unsigned NOT NULL COMMENT '当前使用的皮肤',
	`hero_state` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '武将状态(解锁等)',
	`delete_flag` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '删除标记',
	`last_equip_plan` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '最近使用的装备方案',
	`nick_name` varchar(64) NOT NULL DEFAULT '' COMMENT '昵称',
	`famous` varchar(256) NOT NULL DEFAULT '' COMMENT '名将册',
    PRIMARY KEY (`rk`),
    INDEX (`role_uid`)
)
COMMENT='武将表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `form` (
	`rk` bigint(20) unsigned NOT NULL COMMENT '布阵唯一id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`pos_info` varchar(500) NOT NULL,
	`role_uid` bigint(20) unsigned NOT NULL,
	`formation_index` int(10) unsigned NOT NULL,
    PRIMARY KEY (`rk`),
	INDEX (`role_uid`)
)
COMMENT='武将阵形表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `item` (
    `rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家唯一UID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
    `tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '策划表ID',
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0',
    `num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '数量',
    `weared` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '是否已穿戴',
	`package` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '背包编号',
	`param` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '参数',
	`param2` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '参数',
	`find_role` varchar(20) NOT NULL DEFAULT '' COMMENT '发现人',
	`hero_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '已穿戴为所属武将uid，没穿戴为0',
    `attr` varchar(128) NOT NULL DEFAULT '' COMMENT '基础属性',
	`exattr` varchar(256) NOT NULL DEFAULT '' COMMENT '判词（附加属性）',
    `remake_exattr` varchar(256) NOT NULL DEFAULT '' COMMENT '重铸的判词',
	`remake_exattr_bonus` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '判词重铸记录次数',
	`name` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '装备词缀ID',
	`special` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '装备特技ID',
	`strengthen_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '强化等级',
	`strmaster_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '强化大师等级',
	`remake_material` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '洗练材料',
	`lock_material` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '洗练锁定材料',
	`strengthen_material` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '强化材料',
	`strengthen_material_ex` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '高级强化材料',
	`strengthen_money` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '强化消耗铜币',
	`state` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '0为正常1为绑定2为冻结',
	`expired_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '冻结到期时间',
	`remain_remake_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '剩余改造次数',
	`add_remake_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '已经增加过几次改造次数',
    PRIMARY KEY (`rk`),
    INDEX (`role_uid`)
) 
COMMENT='道具表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `task` (
	`rk` varchar(32) NOT NULL DEFAULT '0' COMMENT '任务唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`cur_task_list` blob COMMENT '当前任务列表',
	`comp_task_list` blob COMMENT '已完成任务列表',
	`shilian_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '试炼任务次数',
	`shilian_seq` varchar(256) NOT NULL DEFAULT '' COMMENT '试炼随机序列',
	`circle_task_comp_list` varchar(256) NOT NULL DEFAULT '' COMMENT '环任务完成记录',
	`task_event` blob COMMENT '任务事件列表',
	PRIMARY KEY (`rk`)
)
COMMENT='保存用户任务信息' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `liandan` (
    `rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家唯一UID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`all_liandan_data` blob COMMENT '炼丹炉信息',
    PRIMARY KEY (`rk`)
)
COMMENT='炼丹炉表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `equip_plan` (
    `rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家唯一UID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`all_equip_plan_data` blob COMMENT '装备方案信息',
    PRIMARY KEY (`rk`)
)
COMMENT='装备方案表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `country` (
	`rk` int(10) unsigned NOT NULL COMMENT '国家id（配置表id）',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`name` varchar(20) NOT NULL DEFAULT '' COMMENT '国家名称',
	`year_name` varchar(20) NOT NULL DEFAULT '' COMMENT '国家年号',
	`flag_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '国家旗帜',
	`flag_list` varchar(256) NOT NULL DEFAULT '' COMMENT '已经解锁的旗帜',
	`free_times` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '名称免费修改次数',
	`free_times2` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '年号免费修改次数',
	`money` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '国库资金',
	`level_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '国家实力排行',
	`power` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '当前国力',
	`notice` varchar(128) NOT NULL DEFAULT '' COMMENT '国家公告',
	`king_family` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '国王家族',
	`state` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '王城战状态',
	`family_defensive` bigint(20) NOT NULL DEFAULT '0' COMMENT '防守方家族id',
	`family_defensive_helper` bigint(20) NOT NULL DEFAULT '0' COMMENT '防守方助战家族id',
	`defensive_cost` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '防守方的竞价资金',
	`defensive_notice_user` varchar(20) NOT NULL DEFAULT '0' COMMENT '战前宣言的玩家名称',
	`defensive_notice_officer` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '战前宣言的玩家职位',
	`defensive_notice` varchar(128) NOT NULL DEFAULT '' COMMENT '战前宣言',
	`defensive_like_num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '战前宣言的点赞',
	`defensive_dislike_num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '战前宣言的踩数',
	`family_attack` bigint(20) NOT NULL DEFAULT '0' COMMENT '进攻方家族id',
	`family_attack_helper` bigint(20) NOT NULL DEFAULT '0' COMMENT '进攻方助战家族id',
	`attack_cost` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '进攻方的竞价资金',
	`attack_notice_user` varchar(20) NOT NULL DEFAULT '0' COMMENT '战前宣言的玩家名称',
	`attack_notice_officer` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '战前宣言的玩家职位',
	`attack_notice` varchar(128) NOT NULL DEFAULT '' COMMENT '战前宣言',
	`attack_like_num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '战前宣言的点赞',
	`attack_dislike_num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '战前宣言的踩数',
	`state_end_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '当前状态结束时间',
	`war_start_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '本轮战斗开始时间',
	`king_luckydraw_info` varchar(256) NOT NULL DEFAULT '0' COMMENT '国王开启祈福礼包信息',
	`king_luckydraw_money` varchar(256) NOT NULL DEFAULT '0' COMMENT '国王累积钱的信息',
	 PRIMARY KEY (`rk`)
)
COMMENT='国家' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `country_office` (
	`rk` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '国家官员',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`country_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '国家id',
	`country_officer_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '国家官职',
	`role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色uid',
	`reappoint_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '下次任命时间',
  	PRIMARY KEY (`rk`),
	INDEX (`country_id`)
)
COMMENT='国家官员' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `country_affire` (
	`rk` bigint(20) unsigned NOT NULL COMMENT '政务id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`country_id` int(10) unsigned NOT NULL COMMENT '国家id',
	`affire_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '政务类型',
	`status` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '状态',
	`start_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '开始时间',
	`params` varchar(1024) NOT NULL DEFAULT '0' COMMENT '政务参数',
	PRIMARY KEY (`rk`),
    INDEX (`country_id`)
)
COMMENT='国家政务' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `country_trans_record` (
	`rk` bigint(20) unsigned NOT NULL COMMENT '交易id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`country_id` int(10) unsigned NOT NULL COMMENT '国家id',
	`role_uid` int(10) unsigned NOT NULL COMMENT '交易人角色id',
	`type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '交易类型[0-9]收入[>=10]支出',
	`count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '数量',
	`cur_cnt` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '数量',
	`time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '交易时间',
	PRIMARY KEY (`rk`),
    INDEX (`country_id`)

)
COMMENT='国家收支记录' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `king_war_apply` (
    `rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '家族id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`king_war_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '国家id',
    `bid_cost` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '竞价的家族资金',
	`bid_state` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '竞价的资金状态',
	`bid_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '报名时间',
	PRIMARY KEY (`rk`),
	INDEX (`king_war_id`)
) 
COMMENT='王城争夺报名表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `family` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '家族id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`level` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '家族等级',
	`create_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族创建时间',
	`country` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '所属国家',
	`loyalty` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '忠诚度',
	`quit_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '退出同盟时间',
	`quit_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '每月退出同盟次数',
	`money` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族资金',
	`auto_newbie` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '是否自动加为新军',
	`name` varchar(20) NOT NULL DEFAULT '0' COMMENT '家族名称',
	`declaration` varchar(80) NOT NULL DEFAULT '0' COMMENT '家族宣言',
	`notice` varchar(100) NOT NULL DEFAULT '0' COMMENT '家族公告',
	`prosperity` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族繁荣度',
	`prestige` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族威望',
	`territory` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族领地',
	`impeacher` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '发起家族弹劾玩家uid',
	`impeach_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '弹劾时间',
	`impeacher_cnt` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '响应弹劾人数',
	`online_prosperity_add_daily` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '在线繁荣度每日加成',
	`chat_prosperity_add_daily` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '聊天繁荣度每日加成',
	`task_prosperity_add_daily` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '任务繁荣度每日加成',
	`creator` varchar(20) NOT NULL DEFAULT '' COMMENT '创建家族玩家',
	`apply_family` varchar(1024) NOT NULL DEFAULT '' COMMENT '申请家族列表',
	`prestige_add_daily` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族威望每日加成',
	`matain_cost` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族维护费用',
	`is_low_maintenance` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '是否处于低维护状态',
	`shop_refresh` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族商店刷新时间',
	`random_shop` varchar(512) NOT NULL DEFAULT '' COMMENT '家族商店物品信息',
	PRIMARY KEY (`rk`),
    INDEX (`name`)

)
COMMENT='家族' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `family_build` (
	`rk` varchar(32) NOT NULL DEFAULT '0' COMMENT '家族id+建设类型',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '家族id',
	`type` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '建设类型',
	`level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '建设等级',
	`cd_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次升级时间',
	PRIMARY KEY (`rk`),
	INDEX (`id`)
)
COMMENT='家族建设' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `family_member` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`family_id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '家族id',
	`family_officer_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族职位',
	`total_family_contribution` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '历史家族贡献',
	`join_family_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '加入家族时间',
	`impeach_ack` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '是否已响应弹劾',
	`prayer_contribution` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '祈愿贡献',
	`last_prayer_day_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次祈愿每日更新时间',
	`prayer_times` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '祈愿次数',
	`prayer_rwd_times` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '祈愿奖励次数',
	`last_prayer_week_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次祈愿每周更新时间',
	`prayer_gift_times` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '祈愿赠予次数',
	`prayer_hero_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '祈愿武将tid',
	`prayer_hero_num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '祈愿武将当前数量',
	`prayer_hero_max` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '祈愿武将最大数量',
	`prayer_start_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '祈愿开始时间',
	`prayer_complete_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '祈愿完成后删除这条记录的时间',
	`prayer_gift_log` varchar(512) NOT NULL DEFAULT '' COMMENT '本次祈愿赠予玩家记录',
	`prayer_log` blob COMMENT '本周祈愿个人记录（每周会清理）',
	`shop_refresh` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族商店刷新时间',
	`random_shop_limit` varchar(512) NOT NULL DEFAULT '' COMMENT '家族商店限购信息',
  	PRIMARY KEY (`rk`),
	INDEX (`family_id`)
)
COMMENT='家族成员' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `family_war` (
	`rk` varchar(32) NOT NULL DEFAULT '0' COMMENT '家族id+时间',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '编号',
	`week` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '周时间唯一id',
	`family_id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '家族id',
	`family_name` varchar(20) NOT NULL DEFAULT '0' COMMENT '家族名称',
	`family_prestige` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族历史威望',
	`family_score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族积分',
	`family_rank` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族排名',
	`result` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '胜负情况',
  	PRIMARY KEY (`rk`),
	INDEX (`week`)
)
COMMENT='家族战数据' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `account_ban` (
	`rk` varchar(64) NOT NULL DEFAULT '' COMMENT '被封的账号',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`rt` bigint(20) NOT NULL DEFAULT '0' COMMENT '数据时间',
	PRIMARY KEY (`rk`)
)
COMMENT='黑名单' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `ip_access` (
	`rk` varchar(32) NOT NULL DEFAULT '' COMMENT '允许登录的ip',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`rt` bigint(20) NOT NULL DEFAULT '0' COMMENT '数据时间',
	PRIMARY KEY (`rk`)
)
COMMENT='准入IP' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `global_user` (
	`rk` bigint(20) NOT NULL DEFAULT '0' COMMENT '数据主键',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`rt` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '数据更新时间',
	`name` varchar(20) NOT NULL DEFAULT '' COMMENT '角色名',
	`create_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
	`country_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '国家id',
	`country_officer_type` varchar(50) NOT NULL DEFAULT '' COMMENT '国家官职',
	`family_id` bigint(20) NOT NULL DEFAULT '0' COMMENT '家族id',
	`family_officer_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族官职',
	`family_contribution` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族贡献',
	`last_login_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次上线时间',
	`last_logout_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次下线时间',
	`level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '等级',
	`fighting` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '玩家战斗力',
	`fighting_record` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '玩家战斗力历史最高',
	`hero_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '玩家主公英雄 tid',
	`scene_id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色所在场景',
	`map_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色所在场景的地图id',
	`last_map_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色最后静态场景的地图id',
	`update_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '每日更新记录',
	`channel_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '渠道id',
	`did` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '数字id',
	`plugin` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '玩家模型ID',
	`ban_time` int(10) NOT NULL DEFAULT '0' COMMENT '封号时间',
	`silence_time` int(10) NOT NULL DEFAULT '0' COMMENT '禁言时间',
	`charm` int(10) NOT NULL DEFAULT '0' COMMENT '魅力值',
	`troop_friend_count` int(10) NOT NULL DEFAULT '0' COMMENT '每日组队好友推荐次数',
	PRIMARY KEY (`rk`),
	INDEX (`name`)
)
COMMENT='全局玩家' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `global_server_data` (
	`rk` int(10) unsigned NOT NULL COMMENT '数据类型',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
  	`data` blob COMMENT '二进制数据',
  	PRIMARY KEY (`rk`)
)
COMMENT='全局杂项表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `city` (
	`rk` int(10) unsigned NOT NULL COMMENT '城市id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`prosperity_value` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '繁荣度',
	`hold_value` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '占有度',
	`hold_country` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '占有国家',
	PRIMARY KEY (`rk`)
)
COMMENT='城市' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `role_trade_item` (
	`rk` varchar(32) NOT NULL DEFAULT '0' COMMENT '角色id+item_tid',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`role_uid` bigint(20) NOT NULL DEFAULT '0' COMMENT '角色uid',
	`item_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '物品ID',
	`item_buy_num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '物品购买的数量',
	`item_max_buy_num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '物品最大购买的数量',
	`item_max_buy_num_addi` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '物品额外最大购买数量',
	`item_buy_price` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '物品购买的价格',
	PRIMARY KEY (`rk`),
    INDEX (`role_uid`)
)
COMMENT='角色贸易物品信息' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `levelsort` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '等级',
	`save_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '保存时间',
	PRIMARY KEY (`rk`)
)
COMMENT='等级排行榜' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `towersort` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '战队等级',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '当前层数',
	`cost_day` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '天数',
	`save_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '保存时间',
	PRIMARY KEY (`rk`)
)
COMMENT='爬塔排行榜' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `criminalsort` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'pk值',
	`save_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '保存时间',
	PRIMARY KEY (`rk`)
)
COMMENT='通缉排行榜' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `achievesort` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '成就等级',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '成就积分',
	`save_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '保存时间',
	PRIMARY KEY (`rk`)
)
COMMENT='成就排行榜' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `familysort` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族威望',
	`save_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '保存时间',
	PRIMARY KEY (`rk`)
)
COMMENT='家族排行榜' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `charmsort` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '魅力值',
	`save_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '保存时间',
	PRIMARY KEY (`rk`)
)
COMMENT='魅力排行榜' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `recruitsort` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '招募次数',
	`save_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '保存时间',
	PRIMARY KEY (`rk`)
)
COMMENT='招募次数排行榜' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `coppersort` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '消耗铜币',
	`save_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '保存时间',
	PRIMARY KEY (`rk`)
)
COMMENT='消耗铜币排行榜' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `goldsort` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '消耗金币',
	`save_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '保存时间',
	PRIMARY KEY (`rk`)
)
COMMENT='消耗金币排行榜' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `powersort` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '战力值',
	`save_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '保存时间',
	PRIMARY KEY (`rk`)
)
COMMENT='战力排行榜' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `item_limit` (
	`rk` varchar(32) NOT NULL DEFAULT '0' COMMENT 'item_tid',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
  	`item_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'item_tid',
  	`item_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '物品累计数量',
  	PRIMARY KEY (`rk`)
)
COMMENT='世界道具限制表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `friend` (
	`rk` varchar(40) NOT NULL DEFAULT '0' COMMENT '玩家uid + 好友uid',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
  	`role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家uid',
  	`friend_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '好友uid',
	`friend_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '好友类型',
	`time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '加好友时间',
	`contact_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '最近联系时间',
	`is_top` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '是否为置顶好友',
	`top_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '置顶时间',
	`shadow_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次好友化身邀请时间',
	`shadow_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次好友化身邀请次数',
	`friendliness` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '亲密度',
	`friendliness_added_today` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '日增亲密度',
	`friendliness_refresh_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次刷新日增亲密度上限的时间',
	`can_get_fp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '可以获取的友情点',
	`last_get_fp_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次获取友情点的时间',
	`last_sent_fp_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次赠送友情点的时间',
  	PRIMARY KEY (`rk`),
	INDEX (`role_uid`)
)
COMMENT='friend' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `friend_related` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家uid',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`initialization` tinyint(3) NOT NULL DEFAULT '0' COMMENT '是否初始化',
	`fp_has_sent_today` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '今日已经送给好友友情点的次数',
	`refresh_sent_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次刷新赠送次数的时间戳',
	`fp_has_received_today` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '今日已经收取好友友情点的次数',
	`refresh_received_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次刷新领取次数的时间戳',
	`receive_limit` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '每日领取友情点的次数限制',
	`send_limit` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '每日赠送友情点的次数限制',
	`got_relation_award` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '已领取的羁绊奖励',
  	PRIMARY KEY (`rk`)
)
COMMENT='friend_related`' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `mail` (
    `rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
    `state` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '状态',
	`type` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '邮件类型',
	`global_id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '全局邮件ID（如果是全局邮件）',
	`to_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '收信人id',
	`from_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '发信人id',
	`from_name` varchar(20) NOT NULL DEFAULT '' COMMENT '发信人名字',
	`create_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '创建邮件时间',
	`del_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '删除邮件时间',
	`content_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '内容id',
	`title` varchar(64) NOT NULL DEFAULT '' COMMENT '标题',
	`content` varchar(256) NOT NULL DEFAULT '' COMMENT '内容',
	`items` varchar(1024) NOT NULL DEFAULT '' COMMENT '附带道具',
	`item_state` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '附加道具状态',
	`item_do_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '邮件领取限制',
	`item_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '邮件道具来源类型',
	PRIMARY KEY (`rk`),
	INDEX (`to_uid`)
) 
COMMENT='邮件表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `global_mail` (
    `rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`type` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '全局邮件类型',
	`to_uids` varchar(1024) NOT NULL DEFAULT '' COMMENT '发送给指定的人（最多85个人）',
	`param1` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '附加参数1',
	`param2` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '附加参数2',
    `start_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '邮件起始时间',
	`end_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '邮件结束时间',
	`from_name` varchar(20) NOT NULL DEFAULT '' COMMENT '发信人名字',
	`title` varchar(64) NOT NULL DEFAULT '' COMMENT '标题',
	`content` varchar(256) NOT NULL DEFAULT '' COMMENT '内容',
	`items` varchar(1024) NOT NULL DEFAULT '' COMMENT '附带道具',
	`desc` varchar(256) NOT NULL DEFAULT '' COMMENT '备注',
	PRIMARY KEY (`rk`)
) 
COMMENT='全局邮件表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `arena_role_data` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家UID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`win_times` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '赢的次数',
	`lose_times` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '输的次数',
	`is_start` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '是否参加竞技场',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '竞技场分数',
	`arena_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '竞技场段位',	
	`all_fight_times` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '总战斗场次',	
	`all_win_times` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '总胜利场次',	
	`honor` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '荣誉',	
	`honor_limit` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '荣誉上限',	
	`best_win_times` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '单轮最高胜场',
	`last_reset_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次重置时间',
	PRIMARY KEY (`rk`)
)
COMMENT='玩家竞技场数据表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `arena_fight_data` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家UID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`update_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
	`fight_value` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '玩家战斗力',
	`fight_data` blob COMMENT '战斗数据',
	PRIMARY KEY (`rk`)
)
COMMENT='玩家竞技场镜像战斗数据表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `shop_goods` (
	`rk` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '货物id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`buy_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '已购买次数',
  	`start_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上架时间',
	`end_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '下架时间',
	`refresh_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '刷新时间',
  	PRIMARY KEY (`rk`)
)
COMMENT='商店货物表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `user_goods` (
	`rk` varchar(32) NOT NULL DEFAULT '0' COMMENT '玩家id+货物id',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`goods_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '货物id',
	`role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家uid',
	`buy_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '已购买次数',
	`refresh_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '刷新时间',
  	PRIMARY KEY (`rk`)
)
COMMENT='玩家购买货物表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `challenge` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`challenger_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '发起玩家uid',
	`target_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '应战玩家uid',
	`challenge_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '约战时间',
	`state` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '状态',	
	PRIMARY KEY (`rk`),
	INDEX(`challenger_uid`),
	INDEX(`target_uid`),
	INDEX(`challenge_time`)
)
COMMENT='皇城约战表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `achieve` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家uid',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`achieve_data` blob COMMENT '成就数据',
	PRIMARY KEY (`rk`)
)
COMMENT='成就' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `role_dungeon` (
	`rk` varchar(32) NOT NULL DEFAULT '0' COMMENT 'uid+副本表ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
    `role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家uid',
    `dungeon_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '副本表ID',
    `total_pass_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '通关次数',
	`rwd_list` varchar(512) NOT NULL DEFAULT '' COMMENT '奖励怪物计数',
	`refresh_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '刷新时间',
	`best_score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '历史最佳得分',
	`best_pass_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '历史最佳时间',
	`best_death_value` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '历史最佳死亡次数',
	PRIMARY KEY (`rk`),
    INDEX (`role_uid`)
) 
COMMENT='玩家副本数据' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `role_dungeon_times` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家uid',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`times_data` blob COMMENT '副本次数数据',
	PRIMARY KEY (`rk`)
)
COMMENT='副本次数表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `family_skill` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家uid',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`all_family_skill_data` blob COMMENT '家族技能数据',
	PRIMARY KEY (`rk`)
)
COMMENT='家族技能表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `goods` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
    `seller_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '出售人ID',
    `item_tid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '物品ID',
    `item_num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '物品数量',
	`equip_info` blob COMMENT '装备数据',
	`item_price` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '单价',
	`selled_num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '已售数量',
	`profit` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '收益',
	`state` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '状态',
	`expired_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '到期时间',
	`heat` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '热度',
	`rarity` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '珍品标识',
	PRIMARY KEY (`rk`)
) 
COMMENT='寄售行数据' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `rarity_price` (
	`rk` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '物品ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`deal_price` varchar(256) NOT NULL DEFAULT '' COMMENT '最近成交价格',
	PRIMARY KEY (`rk`)
) 
COMMENT='珍品成交表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `goods_care` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
    `goods_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '商品ID',
	`role_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '关注人ID',
	PRIMARY KEY (`rk`)
) 
COMMENT='寄售行关注表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `active_reward` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家uid',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`active_reward_data` blob COMMENT '活跃奖励数据',
	PRIMARY KEY (`rk`)
)
COMMENT='活跃奖励' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `offline_role_data` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家uid',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`offline_role_data` blob COMMENT '离线数据',
	PRIMARY KEY (`rk`)
)
COMMENT='角色离线数据表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `redbag` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
    `sender_uid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '发送者ID',
	`send_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '总数量',
	`money` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '总金额',
	`rest_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '剩余数量',
	`rest_money` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '剩余总金额',
    `send_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '发送时间',
    `finish_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '领完消耗时间',
	`draw_data` blob COMMENT '领取信息',
	PRIMARY KEY (`rk`)
) 
COMMENT='红包数据' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `role_simple_data` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家uid',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`simple_role_data` blob COMMENT '简易数据',
	PRIMARY KEY (`rk`)
)
COMMENT='查看玩家信息数据表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `offline_arena` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家uid',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`cur_class` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '当前级别',
	`cur_rank` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '当前排名',
	`old_record_rank` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '旧的最高排名',
	`record_rank` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '当前最高排名',
	`old_class` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '旧的级别',
	`fight_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '攻打次数',
	`buy_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '购买次数',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '今日积分',
	`score_award` varchar(256) NOT NULL DEFAULT '' COMMENT '积分奖励',
	`fight_data` blob COMMENT '防守阵容数据',
	`record_data` blob COMMENT '战报数据',
	PRIMARY KEY (`rk`)
)
COMMENT='离线竞技数据表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `patrol_hire` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`map_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '场景ID',
	`begin_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次巡逻结算时间',
	`fast_patrol_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '快速巡逻次数',
  	PRIMARY KEY (`rk`)
)
COMMENT='雇佣巡逻表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `general_info` (
	`rk` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '目标ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '完成数量',
	PRIMARY KEY (`rk`)
) 
COMMENT='目标统计数据表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';


CREATE TABLE IF NOT EXISTS `general_status` (
	`rk` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '大势ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`name` varchar(32) NOT NULL DEFAULT '' COMMENT '第一个名字',
	`start_time` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
	`is_speed_time` int(10) NOT NULL DEFAULT '0' COMMENT '是否加速时间',
	`status` int(10) NOT NULL DEFAULT '0' COMMENT '状态',

	PRIMARY KEY (`rk`)
) 
COMMENT='天下大势表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

							
CREATE TABLE IF NOT EXISTS `general_target_first_name` (
	`rk` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '目标ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`name` varchar(32) NOT NULL DEFAULT '' COMMENT '第一个名字',
	PRIMARY KEY (`rk`)
) 
COMMENT='天下大势第一目标达成' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `business_act` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`money_tree` varchar(64) NOT NULL DEFAULT '' COMMENT '摇钱树',	
	`sevenday_login_day` int(10) NOT NULL DEFAULT '0' COMMENT '7日登陆-登录天数',
	`sevenday_login_data` varchar(64) NOT NULL DEFAULT '' COMMENT '7日登陆数据',
	`sevenday_login_flag` int(10) NOT NULL DEFAULT '0' COMMENT '7日登陆-登陆标记',
	`month_card` varchar(256) NOT NULL DEFAULT '' COMMENT '月卡记录',
	`level_fund_buy_flag` int(10) NOT NULL DEFAULT '0' COMMENT '福利-基金购买标记',
	`level_fund_prize` varchar(256) NOT NULL DEFAULT '' COMMENT '福利-基金奖励',
	`limit_buy_day` int(10) NOT NULL DEFAULT '0' COMMENT '限购天数',
	`limit_buy_red` int(10) NOT NULL DEFAULT '0' COMMENT '限购红点',
	`limit_buy_data` varchar(256) NOT NULL DEFAULT '' COMMENT '限购数据',
	`exchange_item_start_time` int(10) NOT NULL DEFAULT '0' COMMENT '兑换开始时间',
	`exchange_item_data` varchar(256) NOT NULL DEFAULT '' COMMENT '兑换道具信息',
	`statistical_data` varchar(5000) NOT NULL DEFAULT '' COMMENT '统计数据',
	`sd_target_day` int(10) NOT NULL DEFAULT '0' COMMENT '7日目标天数',
	`sd_target_data` varchar(1000) NOT NULL DEFAULT '' COMMENT '7日目标数据',
	`recharge_total` varchar(256) NOT NULL DEFAULT '' COMMENT '已经领取的累充奖励',
	`gold_luckydraw_data` varchar(256) NOT NULL DEFAULT '' COMMENT '黄金十连抽领取记录',
	PRIMARY KEY (`rk`)
) 
COMMENT='运营活动表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';


CREATE TABLE IF NOT EXISTS `recharge` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`recharge_flag` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '充值次数',
	`total_cny` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '累计充值金额',
	`vip_level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'VIP等级',
	`recharge_reward_flag` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '充值礼包领取标记位',
	`cheap_day` varchar(256) NOT NULL DEFAULT '' COMMENT '每日特惠',
	PRIMARY KEY (`rk`)
) 
COMMENT='充值数据表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `office` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '角色唯一ID',
	`invalid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '数据是否有效 0 有效（默认值）1 失效',
	`level` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '逆命师等级',
	`exp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '逆命师经验',
	`award` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '每日奖励领取标记0为未领取1为已领取',
	PRIMARY KEY (`rk`)
)
COMMENT='逆命师数据表' ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';





