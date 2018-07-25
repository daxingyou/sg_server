-- robot_idle.lua 
-- robot idle event
-- version 

c_info( "load robot idle event" )

-- 注册idle状态
idle_state="idle"

-- 注册idle事件
idle_event = 100

------------------------------------------------------------------------------

-- 进入idle状态迁移触发函数
void_ = function(fsm)
	
end

ai_action_idle = function(fsm)
	fsm:done_current_task()
end

-- 任务执行函数
local task_init_robot = function(fsm)
	fsm.manager:c_fast_move(fsm.task[fsm.taskfront].task_param1, fsm.task[fsm.taskfront].task_param2)
end

-- 任务完成执行函数
local done_init_task = function(fsm)
	fsm.publiccd = 500
end

-----------------------------------------------------------------------------------
-- 角色初始化到某个位置
Robot_init_position = function(fsm, x, y)
	fsm:add_once_task(task_init_robot, done_init_task, x, y)
end
