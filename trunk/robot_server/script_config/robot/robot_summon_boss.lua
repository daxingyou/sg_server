-- robot_fight.lua 
-- robot fight event
-- version 

c_info( "load robot summon boss event" )

-- 注册状态
state_summon_wait_info = "state_summon_wait_info"
state_summon_check_over = "state_summon_check_over"

-- 注册事件
summon_request_boss_info_event = 1001
summon_check_boss_exist_event = 1002
summon_boss_event = 1003
summon_check_wait_event = 1004

-- 任务执行函数 -------------------------------------------------------------------
local task_summon_boss = function(fsm)
	if fsm.currstate == idle_state then
		fsm:post_event(summon_request_boss_info_event)
	elseif fsm.currstate == state_summon_wait_info then
		fsm:post_event(summon_check_boss_exist_event)
	elseif fsm.currstate == state_summon_check_over then
		--fsm:post_event(summon_boss_event)
	else
		print ('summon boss task failed state:', fsm.currstate)
	end
end

-- 任务完成执行函数 ---------------------------------------------------------------
local task_done_summon_boss = function(fsm)
	fsm.publiccd = 1000
	if fsm.boss_id ~= nil and fsm.boss_id ~= 0 then
		--print("filed boss:"..fsm.boss_id.." is gone!")
	else
		--print("field boss is all gone!")
	end
end

-- 状态迁移触发函数 --------------------------------------------------------------

local ai_action_wait_boss_info = function(fsm)
	fsm.manager:c_request_boss_info(fsm.boss_id)
	fsm.publiccd = 2000
end

local ai_action_summon_boss = function(fsm)
	local id = 1;
	if fsm.boss_id == 0 then
		id = fsm.manager:c_random_boss_id()
	else
		id = fsm.boss_id
	end

	--print("--------------------------summon boss------------------------- "..id)
	fsm.manager:c_gm_command('/fieldboss '..id)
	fsm.publiccd = 10000
end

local ai_action_next_check_wait = function(fsm)
	fsm.publiccd = 10000
end

-- 状态转移守卫 -------------------------------------------------------------------
local master_summon_boss_guard = function(fsm, from, evt, to)
	if from == idle_state and evt == summon_request_boss_info_event then
		fsm.manager:c_clear_field_boss_exist_flag()
		return true
		
	elseif from == state_summon_wait_info and evt == summon_check_boss_exist_event then

		if fsm.manager:c_get_field_boss_exist_flag() == -1 then
			return false
		elseif fsm.manager:c_get_field_boss_exist_flag() == 1 then
			fsm:post_event(summon_check_wait_event)
		else
			fsm:post_event(summon_boss_event)
		end
		return true

	elseif from == state_summon_check_over and evt == summon_boss_event then
		return true

	elseif from == state_summon_check_over and evt == summon_check_wait_event then
		return true
	else
		print("master_summon_boss_guard state error", from, evt, to)
	end
end

-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
-- 召唤世界boss
Robot_SummonBoss = function(fsm, boss_id)
	if boss_id ~= nil then 
		fsm.boss_id = boss_id 
	else
		fsm.boss_id = 0
	end
	
	fsm:add_task(task_summon_boss, task_done_summon_boss)
	
	fsm:add_transition(idle_state,					summon_request_boss_info_event,	state_summon_wait_info,		master_summon_boss_guard,	ai_action_wait_boss_info)
	fsm:add_transition(state_summon_wait_info,		summon_check_boss_exist_event,	state_summon_check_over,	master_summon_boss_guard,	void_)
	fsm:add_transition(state_summon_check_over,		summon_boss_event,				idle_state,					master_summon_boss_guard,	ai_action_summon_boss)
	fsm:add_transition(state_summon_check_over,		summon_check_wait_event,		idle_state,					master_summon_boss_guard,	ai_action_next_check_wait)
end