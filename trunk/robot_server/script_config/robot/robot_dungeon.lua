-- robot_dungeon.lua 
-- robot dungeon event
-- version 

c_info( "load robot dungeon event" )

-- 注册状态
dungeon_open_state = "open_dungeon"
dungeon_leave_state = "leave_dungeon"
dungeon_fight_move = "dungeon_fight_move"
dungeon_fight_arrive = "dungeon_fight_arrive"
dungeon_fight_try = "dungeon_fight_try"
dungeon_fight_start = "dungeon_fight_start"

-- 注册事件
dungeon_open_dungeon_event = 701
dungeon_leave_dungeon_event = 702
dungeon_fast_move_to_target = 703
dungeon_try_enter_fight = 704
dungeon_try_start_auto_fight = 705

-- 任务执行函数 -------------------------------------------------------------------
-- 打开副本
local task_open_dungeon = function(fsm)
	if fsm.currstate ~= dungeon_open_state then
		fsm.dungeon_id = fsm.task[fsm.taskfront].task_param1	-- 记录下副本id
		
		local is_in_dungeon = fsm.manager:c_is_in_dungeon(fsm.dungeon_id)
		if is_in_dungeon then
			fsm:done_current_task()		-- 已经在这个副本了
			return
		else
			fsm.manager:c_try_open_dungeon(fsm.dungeon_id)
		end
	end
	
	fsm:post_event(dungeon_open_dungeon_event)
end

-- 离开副本
local task_leave_dungeon = function(fsm)
	if fsm.currstate ~= dungeon_leave_state then
		local is_in_dungeon = fsm.manager:c_is_in_dungeon(fsm.dungeon_id)
		if not is_in_dungeon then
			fsm:done_current_task()		-- 已经不在这个副本了
			return
		else
			fsm.manager:c_leave_dungeon()
		end
	end
	
	fsm:post_event(dungeon_leave_dungeon_event)
end

-- 副本战斗
local task_fight_dungeon = function(fsm)
	-- 如果处于待机状态
	if fsm.currstate == idle_state then
		if fsm.manager:c_is_all_killed() then
			fsm:done_current_task()		-- 所有战斗结束
			return
		else
			local index = fsm.manager:c_get_fight_index()	-- 找一个怪
			if index ~= -1 then
				fsm.fight_index = index
				fsm.manager:c_dungeon_move_to(index)
				fsm:post_event(dungeon_fast_move_to_target)
				--print ('dungeon[', fsm.dungeon_id, '] target:', index)
			else
				print ('dungeon[', fsm.dungeon_id, '] have no target!')
			end
		end
	elseif fsm.currstate == dungeon_fight_move then			-- 移动到这个怪附件
		fsm:post_event(dungeon_fast_move_to_target)
	elseif fsm.currstate == dungeon_fight_arrive then		-- 到了之后战斗
		if fsm.manager:c_try_enter_fight() then
			fsm:post_event(dungeon_try_enter_fight)
		else
			print ('dungeon[', fsm.dungeon_id, '] enter fight failed!')
		end
	elseif fsm.currstate == dungeon_fight_try then
		fsm:post_event(dungeon_try_enter_fight)
	elseif fsm.currstate == dungeon_fight_start then
		fsm:post_event(dungeon_try_start_auto_fight)
	else
		print ('dungeon[', fsm.dungeon_id, '] failed state:', fsm.currstate)
	end
end

-- 任务完成执行函数 ---------------------------------------------------------------
local task_open_dungeon_done = function(fsm)
	fsm.publiccd = 1000
	--print ('dungeon[', fsm.dungeon_id, '] task open dungeon done!')
end

local task_leave_dungeon_done = function(fsm)
	fsm.publiccd = 500
	--print ('dungeon[', fsm.dungeon_id, '] task leave_dungeon done!')
	fsm.dungeon_id = nil
end

local task_fight_dungeon_done = function(fsm)
	fsm.publiccd = 1000
	--print ('dungeon[', fsm.dungeon_id, '] task fight_dungeon done!')
end

-- 状态迁移触发函数 --------------------------------------------------------------
local ai_action_dungeon = function(fsm)
	fsm:done_current_task()
end

local ai_action_enter_fight = function(fsm)
	--print ('dungeon[', fsm.dungeon_id, '] ai_action_enter_fight, state:', fsm.currstate)
	
	fsm.manager:c_try_auto_fight()
end

local ai_action_end_fight = function(fsm)
	--print ('dungeon[', fsm.dungeon_id, '] fight end')
	
	fsm.publiccd = 1000
end

-- 状态转移守卫 -------------------------------------------------------------------
local master_open_dungeon = function(fsm,from,evt,to)
	local is_in_dungeon = fsm.manager:c_is_in_dungeon(fsm.dungeon_id)
	
	if from == idle_state and evt == dungeon_open_dungeon_event then
		if not is_in_dungeon then 
			return true 
		else
			fsm:done_current_task()		-- 已经在这个场景
			return false
		end
	elseif from == dungeon_open_state and evt == dungeon_open_dungeon_event then
		return is_in_dungeon
	else
		print("master_open_dungeon state error", from, evt, to)
		return false
	end
end

local master_leave_dungeon = function(fsm,from,evt,to)
	local is_in_dungeon = fsm.manager:c_is_in_dungeon(fsm.dungeon_id)
	
	if from == idle_state and evt == dungeon_leave_dungeon_event then
		if is_in_dungeon then 
			return true 
		else
			fsm:done_current_task()		-- 已经不在这个副本了
			return false
		end
	elseif from == dungeon_leave_state and evt == dungeon_leave_dungeon_event then
		return not is_in_dungeon
	else
		print("master_leave_dungeon state error", from, evt, to)
		return false
	end
end

local master_dungeon_move = function(fsm,from,evt,to)
	if fsm.fight_index == nil then return false end
	
	if from == idle_state and evt == dungeon_fast_move_to_target then
		return true
	elseif from == dungeon_fight_move and evt == dungeon_fast_move_to_target then
		return fsm.manager:c_is_near_dungeon_target(fsm.fight_index)
	else
		print("master_dungeon_move state error", from, evt, to)
	end
end

local master_dungeon_start_fight = function(fsm,from,evt,to)
	if from == dungeon_fight_arrive and evt == dungeon_try_enter_fight then
		return true
	elseif from == dungeon_fight_try and evt == dungeon_try_enter_fight then
		return fsm.manager:c_is_fighting()
	else
		print("master_dungeon_start_fight state error", from, evt, to)
	end
end

local master_dungeon_start_end = function(fsm,from,evt,to)
	if from == dungeon_fight_start and evt == dungeon_try_start_auto_fight then
		return not fsm.manager:c_is_fighting()
	else
		print("master_dungeon_start_end state error", from, evt, to)
	end
end
-----------------------------------------------------------------------------------
-- 副本测试
local Robot_Open_Dungeon = function(fsm,dungeon_id)
	fsm:add_task(task_open_dungeon, task_open_dungeon_done,dungeon_id)				-- 尝试进入副本

	fsm:add_transition(idle_state,dungeon_open_dungeon_event,dungeon_open_state,master_open_dungeon,void_)
	fsm:add_transition(dungeon_open_state,dungeon_open_dungeon_event,idle_state,master_open_dungeon,ai_action_dungeon)
end

local Robot_Leave_Dungeon = function(fsm)		
	fsm:add_task(task_leave_dungeon, task_leave_dungeon_done)			-- 离开副本
	
	fsm:add_transition(idle_state,dungeon_leave_dungeon_event,dungeon_leave_state,master_leave_dungeon,void_)
	fsm:add_transition(dungeon_leave_state,dungeon_leave_dungeon_event,idle_state,master_leave_dungeon,ai_action_dungeon)
end

local Robot_Dungeon_Fight = function(fsm)
	fsm:add_task(task_fight_dungeon, task_fight_dungeon_done)			-- 副本战斗
	
	fsm:add_transition(idle_state,dungeon_fast_move_to_target,dungeon_fight_move,master_dungeon_move,void_)
	fsm:add_transition(dungeon_fight_move,dungeon_fast_move_to_target,dungeon_fight_arrive,master_dungeon_move,void_)
	fsm:add_transition(dungeon_fight_arrive,dungeon_try_enter_fight,dungeon_fight_try,master_dungeon_start_fight,void_)
	fsm:add_transition(dungeon_fight_try,dungeon_try_enter_fight,dungeon_fight_start,master_dungeon_start_fight,ai_action_enter_fight)
	fsm:add_transition(dungeon_fight_start,dungeon_try_start_auto_fight,idle_state,master_dungeon_start_end,ai_action_end_fight)
end

-- 副本测试 一个参数就是指定id 2个参数是2个数直接随机
Robot_Test_Dungeon = function(fsm, from, to)
	if from == nil and to == nil then 
		c_error('Robot_Test_Dungeon args from == nil and to == nil')
		return
	elseif from ~= nil and to == nil then
		Robot_Open_Dungeon(fsm, from)
	elseif from ~= nil and to ~= nil then
		Robot_Open_Dungeon(fsm, math.random(from, to))
	else
		c_error('Robot_Test_Dungeon args error')
		return
	end

	Robot_Dungeon_Fight(fsm)
	Robot_Leave_Dungeon(fsm)
end
