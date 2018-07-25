-- robot_fight.lua 
-- robot fight event
-- version 

c_info( "load robot fight boss event" )

-- 注册状态
state_fight_boss_wait = "state_fight_boss_wait"
state_fight_boss_jump = "state_fight_boss_jump"
state_fight_boss_move = "state_fight_boss_move"
state_fight_boss_arrive = "state_fight_boss_arrive"
state_fight_boss_start = "state_fight_boss_start"
state_fight_boss_end = "state_fight_boss_end"

-- 注册事件
boss_request_boss_info_event = 901
boss_get_boss_data_event = 902
boss_jump_scene_event = 903
boss_move_to_target_event = 904
boss_try_fight_event = 905
boss_auto_fight_event = 906
boss_not_find_event = 907
boss_fight_again_event = 908

-- 任务执行函数 -------------------------------------------------------------------
local task_fight_field_boss = function(fsm)
	if fsm.currstate == idle_state then
		fsm:post_event(boss_request_boss_info_event)
	elseif fsm.currstate == state_fight_boss_wait then
		fsm:post_event(boss_get_boss_data_event)
	elseif fsm.currstate == state_fight_boss_jump then
		fsm:post_event(boss_jump_scene_event)
	elseif fsm.currstate == state_fight_boss_move then
		fsm:post_event(boss_move_to_target_event)
	elseif fsm.currstate == state_fight_boss_arrive then
		fsm:post_event(boss_try_fight_event)
	elseif fsm.currstate == state_fight_boss_start then
		fsm:post_event(boss_auto_fight_event)
	elseif fsm.currstate == state_fight_boss_end then
		fsm:post_event(boss_fight_again_event)
	else
		print ('field_boss failed state:', fsm.currstate)
	end
end

-- 任务完成执行函数 ---------------------------------------------------------------
local task_done_fight_field_boss = function(fsm)
	fsm.publiccd = 1000
	if fsm.boss_id ~= nil and fsm.boss_id ~= 0 then
		print("filed boss:"..fsm.boss_id.." is gone!")
	else
		print("field boss is all gone!")
	end
end

-- 状态迁移触发函数 --------------------------------------------------------------
local ai_action_wait_boss_info = function(fsm)
	fsm.manager:c_request_boss_info(fsm.boss_id)
	fsm.publiccd = 2000
end

local ai_action_jump_boss_scene = function(fsm)
	if fsm.manager:c_get_scene() ~= fsm.map_id then
		fsm.manager:c_jump_scene(fsm.map_id)
		fsm.publiccd = 2000
	end
end

local ai_action_move_to_boss_scene = function(fsm)
	if not fsm.manager:c_move_around(fsm.pos_x, fsm.pos_y, 2) then
		fsm.manager:c_fast_move(fsm.pos_x + 1, fsm.pos_y) --x坐标偏移1
		fsm.publiccd = 2000
	end
end

local ai_action_auto_fight = function(fsm)
	fsm.manager:c_try_auto_fight()
end

local ai_action_fight_end = function(fsm)
	--print("field boss [", fsm.monster_tid, "] fight end")
	fsm.publiccd = 40000
end

local ai_action_no_boss_end = function(fsm)
	fsm.publiccd = 10000
	--fsm:done_current_task()
end

-- 状态转移守卫 -------------------------------------------------------------------
local master_request_boss_guard = function(fsm,from,evt,to)
	if from == idle_state and evt == boss_request_boss_info_event then
		fsm.monster_tid = 0
		fsm.map_id = 0
		fsm.pos_x = 0
		fsm.pos_y = 0
		return true
	else
		print("master_get_boss_guard state error", from, evt, to)
	end
end

local master_move_to_boss_guard = function(fsm,from,evt,to)
	if from == state_fight_boss_wait and evt == boss_get_boss_data_event then
		local monster_id = fsm.manager:c_get_boss_monster_id()
		if monster_id == 0 then
			--print("----------------------------------boss not find");
			fsm:post_event(boss_not_find_event)
			return false
		else
			fsm.monster_tid = monster_id
			fsm.map_id = fsm.manager:c_get_boss_map_id()
			fsm.pos_x = fsm.manager:c_get_boss_pos_x()
			fsm.pos_y = fsm.manager:c_get_boss_pos_y()			
			return true
		end
	elseif from == state_fight_boss_jump and evt == boss_jump_scene_event then
		return fsm.manager:c_get_scene() == fsm.map_id
	elseif from == state_fight_boss_move and evt == boss_move_to_target_event then
		return fsm.manager:c_move_around(fsm.pos_x, fsm.pos_y, 2)
	elseif from == state_fight_boss_arrive and evt == boss_not_find_event then
		return true
	elseif from == state_fight_boss_wait and evt == boss_not_find_event then
		return true
	else
		print("master_move_to_boss_guard state error", from, evt, to)
	end
end

local master_fight_boss_guard = function(fsm,from,evt,to)
	if from == state_fight_boss_arrive and evt == boss_try_fight_event then
		if fsm.manager:c_is_fighting() then
			return true
		else
			if fsm.manager:c_try_attack_field_boss(fsm.monster_tid) == false then
				fsm:post_event(boss_not_find_event)
			end
			
			-- wait for 
			fsm.publiccd = 1000
		end
		return false
	elseif from == state_fight_boss_start and evt == boss_auto_fight_event then
		return not fsm.manager:c_is_fighting()
	elseif from == state_fight_boss_end and evt == boss_fight_again_event then
		if fsm.manager:c_is_fighting() then
			return true
		else
			if fsm.manager:c_try_attack_field_boss(fsm.monster_tid) == false then
				fsm:post_event(boss_not_find_event)
			end
			
			-- wait for 
			fsm.publiccd = 1000
		end
		return false
	elseif from == state_fight_boss_end and evt == boss_not_find_event then
		return true
	else
		print("master_fight_boss_guard state error", from, evt, to)
	end
end

-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
-- 世界boss测试
Robot_FightBoss = function(fsm, boss_id)
	if boss_id ~= nil then 
		fsm.boss_id = boss_id 
	else
		fsm.boss_id = 0
	end
	
	fsm:add_task(task_fight_field_boss, task_done_fight_field_boss)
	
	fsm:add_transition(idle_state,				boss_request_boss_info_event,	state_fight_boss_wait,		master_request_boss_guard,	ai_action_wait_boss_info)
	fsm:add_transition(state_fight_boss_wait,	boss_get_boss_data_event,		state_fight_boss_jump,		master_move_to_boss_guard,	ai_action_jump_boss_scene)
	fsm:add_transition(state_fight_boss_wait,	boss_not_find_event,			idle_state,					master_move_to_boss_guard,	ai_action_no_boss_end)
	fsm:add_transition(state_fight_boss_jump,	boss_jump_scene_event,			state_fight_boss_move,		master_move_to_boss_guard,	ai_action_move_to_boss_scene)
	fsm:add_transition(state_fight_boss_move,	boss_move_to_target_event,		state_fight_boss_arrive,	master_move_to_boss_guard,	void_)
	fsm:add_transition(state_fight_boss_arrive, boss_not_find_event,			idle_state,					master_move_to_boss_guard,	void_)
	fsm:add_transition(state_fight_boss_arrive, boss_try_fight_event,			state_fight_boss_start,		master_fight_boss_guard,	ai_action_auto_fight)
	fsm:add_transition(state_fight_boss_start,	boss_auto_fight_event,			state_fight_boss_end,		master_fight_boss_guard,	ai_action_fight_end)
	fsm:add_transition(state_fight_boss_end,	boss_fight_again_event,			state_fight_boss_start,		master_fight_boss_guard,	ai_action_auto_fight)
	fsm:add_transition(state_fight_boss_end,	boss_not_find_event,			idle_state,					master_fight_boss_guard,	void_)
end