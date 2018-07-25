-- robot_arena.lua 
-- robot arena event
-- version 

c_info( "load robot shop buy event" )

-- 注册状态
shop_month_card_bought_state="shop_month_card_bought"
arena_fight_state="arena_fight"

-- 注册事件
shop_buy_month_card_event=1101
arena_match_event=1102
arena_end_event=1103

-- 测试哪种月卡，ID
local month_card_id = 1

-- 任务执行函数 -------------------------------------------------------------------
-- 初始化
local task_shop_buy = function(fsm)
	if fsm.currstate == idle_state then
		fsm.manager:c_buy_month_card(month_card_id)
		--fsm:post_event(arena_join_event)

	--elseif fsm.currstate == arena_join_state then
	--	fsm:post_event(arena_match_event)
		
	--elseif fsm.currstate == arena_match_state then
	--	fsm:post_event(arena_wait_fight_event)

	--elseif fsm.currstate == arena_fight_state then
	--	fsm:post_event(arena_end_event)

	else
		print ('field_boss failed state:', fsm.currstate)
	end
end

-- 任务完成执行函数 ---------------------------------------------------------------
-- 初始化结束
local task_shop_buy_done = function(fsm)
	print("task shop buy down!")
end


-- 状态迁移触发函数 --------------------------------------------------------------
local ai_action_arena = function(fsm)
	if fsm.task[fsm.taskfront] then fsm.task[fsm.taskfront].done_action(fsm) end
end

-- 状态转移守卫 -------------------------------------------------------------------
local master_shop_buy = function(fsm, from, evt, to)
	if from == idle_state and evt == arena_buy_month_card_event then
		return false
	--elseif from == arena_join_state and evt == arena_match_event then
	--	return fsm.manager:c_is_arena_matched()
	--elseif from == arena_match_state and evt == arena_wait_fight_event then
	--	return fsm.manager:c_is_fighting()
	--elseif from == arena_fight_state and evt == arena_end_event then
	--	return not fsm.manager:c_is_fighting()
	end
end

-----------------------------------------------------------------------------------
-- 竞技场测试
Robot_ShopBuy = function(fsm)
	fsm:add_task(task_shop_buy, task_shop_buy_done)				-- 先加入竞技场
	
	fsm:add_transition(idle_state,						shop_buy_month_card_event,	shop_month_card_bought_state,	master_shop_buy,	void_)
	--fsm:add_transition(shop_month_card_bought_state,	arena_match_event,	arena_fight_state,	master_match_arena,	void_)
	--fsm:add_transition(arena_fight_state,	arena_end_event,	idle_state,			master_fight_end,	void_)
end
