-- robot_move.lua 
-- robot move event
-- version 

c_info( "load robot move event" )

-- 注册move状态
move_state = "move"

-- 注册move事件
arrive_event = 200
randmove_event = 201

------------------------------------------------------------------------------

-- 可用条件判断
local ai_condition_arrive = function(fsm)
	if fsm.manager:c_move_arrive(fsm.task[fsm.taskfront].task_param1, fsm.task[fsm.taskfront].task_param2) then return true end
	return false
end

-- 可用条件判断
local ai_randmove_arrive = function(fsm)
	if fsm.manager:c_move_arrive(fsm.randmove_x, fsm.randmove_y) then return true end
	return false
end

-- 状态转移守卫
local master_Arrive1=function(fsm,from,evt,to)
	local isArrive = ai_condition_arrive(fsm)
	
	if from == idle_state and evt == arrive_event then
		if not isArrive then
			return true
		else
			fsm:done_current_task()
			return false	
		end
	end
end

local master_Arrive2=function(fsm,from,evt,to)
	local isArrive = ai_condition_arrive(fsm)
	
	if from == move_state and evt == arrive_event then
		return isArrive
	else
		return isArrive
	end
end

local master_randArrive1=function(fsm,from,evt,to)
	local isArrive = ai_randmove_arrive(fsm)
	
	if from == idle_state and evt == randmove_event then
		if not isArrive then
			return true
		else
			fsm:done_current_task()
			return false	
		end
	end
end

local master_randArrive2=function(fsm,from,evt,to)
	local isArrive = ai_randmove_arrive(fsm)
	
	if from == move_state and evt == randmove_event then
		return isArrive
	else
		return isArrive
	end
end

-- 任务执行函数
local task_action_move=function(fsm)
	fsm.manager:c_move_to(fsm.task[fsm.taskfront].task_param1, fsm.task[fsm.taskfront].task_param2)
	
	fsm:post_event(arrive_event)
end

local task_action_randmove=function(fsm)
	if fsm.randmove_x == nil and fsm.randmove_y == nil then
		local moverand = {-3, 0, 3}
		fsm.randmove_x = fsm.manager:c_get_currX() + moverand[math.random(3)]
		fsm.randmove_y = fsm.manager:c_get_currY() + moverand[math.random(3)]
	end
	
	fsm.manager:c_move_to(fsm.randmove_x, fsm.randmove_y)
	fsm:post_event(randmove_event)
end

-- 任务完成执行函数
local done_move_task = function(fsm)
	fsm.publiccd = 300
end

local done_randmove_task = function(fsm)
	if fsm.task[fsm.taskfront] then
		fsm.randmove_x = nil
		fsm.randmove_y = nil
	end
	
	fsm.publiccd = 300
end

------------------------------------------------------------------------------
-- 移动到某个位置
Robot_Move = function(fsm, x, y)
	fsm:add_task(task_action_move, done_move_task, x, y)
	
	--初始化相对应的状态转移表
	fsm:add_transition(idle_state,arrive_event,move_state,master_Arrive1,void_)
	fsm:add_transition(move_state,arrive_event,idle_state,master_Arrive2,ai_action_idle)
end

-- 随机移动
Robot_RandMove = function(fsm, times)
	if times < 1 then return end
	
	for i=1, times do
		
		fsm:add_task(task_action_randmove, done_randmove_task)
		
		--print("----------------------------"..cur_x.."-------"..cur_y)
	end
	
	--初始化相对应的状态转移表
	fsm:add_transition(idle_state,randmove_event,move_state,master_randArrive1,void_)
	fsm:add_transition(move_state,randmove_event,idle_state,master_randArrive2,ai_action_idle)
end

