CREATE TABLE IF NOT EXISTS `arena_score_rank` (
    `rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '唯一ID',
	`rt` bigint(20) NOT NULL DEFAULT '0' COMMENT '数据时间',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '玩家积分',
	`stage` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '段位',
	`time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '最近积分更新时间',
	`server_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '玩家所在服务器id',
	`fighting_value` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '战斗力',
	`name` varchar(20) NOT NULL DEFAULT '' COMMENT '角色名',
	`fight_heroes` blob COMMENT '战斗的英雄',
	`hero_equips` blob COMMENT '英雄的装备',
	 PRIMARY KEY (`rk`)
) 
COMMENT='玩家积分排行表' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `arena_score_rank_history` (
    `rk` varchar(22) NOT NULL DEFAULT '' COMMENT '唯一ID',
	`rt` bigint(20) NOT NULL DEFAULT '0' COMMENT '数据时间',
	`score` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '玩家积分',
	`stage` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '段位',
	`time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '最近积分更新时间',
	`server_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '玩家所在服务器id',
	`rank` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '玩家排名',
	`num` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '排行榜期数',
	`name` varchar(20) NOT NULL DEFAULT '' COMMENT '角色名',
	`fight_heroes` blob COMMENT '战斗的英雄',
	`hero_equips` blob COMMENT '英雄的装备',
	PRIMARY KEY (`rk`)
) 
COMMENT='玩家积分历史排行表' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `world_cup` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '唯一ID',
	`rt` bigint(20) NOT NULL DEFAULT '0' COMMENT '数据时间',
	`season` int(10)  NOT NULL DEFAULT '0' COMMENT '赛季',
	`round` int(10) NOT NULL DEFAULT '0' COMMENT '轮次',
	`battle_id` int(10) NOT NULL DEFAULT '0' COMMENT '对阵id',
	`player_a` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家ID',
	`score_a` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '比分',
	`player_b` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '玩家ID',
	`score_b` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '比分',
	`winner_id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '胜利者ID',
	`state` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '状态',
	`odds_a` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '投注玩家1的赔率',
	`bet_a` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '投注玩家1的总注',
	`vote_a` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '投注玩家1的总人数',
	`odds_b` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '投注玩家2的赔率',
	`bet_b` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '投注玩家2的总注',
	`vote_b` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '投注玩家2的总人数',
	 PRIMARY KEY (`rk`),
	 INDEX (`season`)
) 
COMMENT='演武大赛表' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';

CREATE TABLE IF NOT EXISTS `world_cup_bet` (
	`rk` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '唯一ID',
	`rt` bigint(20) NOT NULL DEFAULT '0' COMMENT '数据时间',
	`player_uid` int(10)  NOT NULL DEFAULT '0' COMMENT '投注玩家的唯一ID',
	`battle_uid` int(10)  NOT NULL DEFAULT '0' COMMENT '投注场次的唯一ID',
	`winner` bigint(20) NOT NULL DEFAULT '0' COMMENT '投注内容（谁赢）',
	`odds` int(10) NOT NULL DEFAULT '0' COMMENT '投注赔率',
	`bet` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '赌注',
	`result` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '结果（0为结果还没出，1为赢，2为输）',
	`state` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '状态(0为待领取1为已领取)',
	 PRIMARY KEY (`rk`),
	 INDEX (`battle_uid`)
) 
COMMENT='演武大赛投注表' ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE='utf8_general_ci';
