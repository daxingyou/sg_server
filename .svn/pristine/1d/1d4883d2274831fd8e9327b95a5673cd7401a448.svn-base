-- robot.lua 
-- robot files entry
-- version 

c_info( "load robot event" )

--初始化函数
function InitRobotAi(_Self)
	local fsm
	fsm = state_machine.new(idle_state)
	fsm.manager = _Self	--Robot指针
	fsm.publiccd = 1000	--上线后等一会再执行lua

	-- 一些gm命令
	--Robot_do_gm(fsm, '/funcopenall')
	
	-- 测试登陆
	-- Robot_Login_test(fsm)
	
	-- 测试9屏接口
	Robot_Nine_test(fsm)
	
	-- talk
    -- Robot_TalkAbout(fsm, 3, '[EM06]发hi护甲哦啊好金富豪和繁荣爱毁容哎王浩然IP我金夫人挖秦浩然欧皇佛红藕啊哈哈[EM07]', 100)
    -- Robot_TalkAbout(fsm, 2, '[EM06]发[EM07]人口排位鸡肉片前几日破', 100)
	
	-- 测试自动战斗
    -- 参数二：战斗次数，-1代表无限
	-- Robot_Random_fight(fsm, -1)
	
	-- 召唤世界boss
	-- 参数二：取值范围0-25, 设置为0代表随机生成ID为1-25的任意一个boss，设置为1-25代表只生成ID为该值的boss
	-- Robot_SummonBoss(fsm, 0)

	-- 测试测试boss战斗
	-- Robot_do_gm(fsm, '/setlevel 40')
	-- Robot_FightBoss(fsm)
	
	-- 测试副本战斗
	-- Robot_Test_Dungeon(fsm, 3)

	-- 测试跨服战斗
	-- Robot_do_gm(fsm, '/chgtime 20190601200000')
	-- Robot_Arena(fsm)
	
	-- 结束输出
	Robot_End(fsm)
	
	return fsm
end

