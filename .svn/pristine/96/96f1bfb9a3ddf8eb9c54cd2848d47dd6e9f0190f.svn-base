-- robot_fight.lua 
-- robot fight event
-- version 

c_info( "load robot fight event" )

-- 注册状态
role_wait_fight_state = "role_wait_fight_state"
role_fighting_state = "role_fighting_state"

-- 注册事件
role_send_fight_event = 801
role_try_fight_event = 802
role_auto_fight_event = 803

-- 任务执行函数 -------------------------------------------------------------------
-- 开启一场战斗
local task_open_fight = function(fsm)
	if fsm.fight_times == 0 then 
		fsm:done_current_task()		-- 任务结束
	else
		if fsm.currstate == idle_state then
			local gm_str = '/pve '..fsm.fight_id
			fsm.manager:c_gm_command(gm_str)
			fsm:post_event(role_send_fight_event)
		elseif fsm.currstate == role_wait_fight_state then
			fsm:post_event(role_try_fight_event)
		elseif fsm.currstate == role_fighting_state then			-- 移动到这个怪附近
			fsm:post_event(role_auto_fight_event)
		else
			print ('fight failed state:', fsm.currstate)
		end
	end
end

-- 任务完成执行函数 ---------------------------------------------------------------
local task_fight_done = function(fsm)
	fsm.publiccd = 1000
end

-- 状态迁移触发函数 --------------------------------------------------------------
local ai_action_enter_fight = function(fsm)
	if fsm.fight_times > 0 then fsm.fight_times = fsm.fight_times - 1 end
	fsm.manager:c_try_auto_fight()
end

local ai_action_wait_fight = function(fsm)
	fsm.publiccd = 2000
end

-- 状态转移守卫 -------------------------------------------------------------------
local master_fight_guard = function(fsm,from,evt,to)
	if from == idle_state and evt == role_send_fight_event then
		return true
	elseif from == role_wait_fight_state and evt == role_try_fight_event then
		return fsm.manager:c_is_fighting()
	elseif from == role_fighting_state and evt == role_auto_fight_event then
		local is_fight_end = fsm.manager:c_is_fighting()
		--if is_fight_end then print ("fight end") end
		return not is_fight_end
	else
		print("master_fight_guard state error", from, evt, to)
	end
end

-------------------------------------------------------------------------------
-- 战斗测试
Robot_Test_fight = function(fsm, fight_id, times)
	fsm.fight_id = fight_id
	fsm.fight_times = times
	
	fsm:add_task(task_open_fight, task_fight_done)				-- 尝试开启gm战斗
	
	fsm:add_transition(idle_state,role_send_fight_event,role_wait_fight_state,master_fight_guard,ai_action_wait_fight)
	fsm:add_transition(role_wait_fight_state,role_try_fight_event,role_fighting_state,master_fight_guard,ai_action_enter_fight)
	fsm:add_transition(role_fighting_state,role_auto_fight_event,idle_state,master_fight_guard,void_)
end

local max_fight_id = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,120001}

Robot_Random_fight = function(fsm, times)
	local rand_fight = max_fight_id[math.random(#max_fight_id)]
	Robot_Test_fight(fsm, rand_fight, times)
end
