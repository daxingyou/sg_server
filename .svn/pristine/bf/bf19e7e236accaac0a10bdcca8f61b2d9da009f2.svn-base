-- robot_task_end.lua 
-- robot task_end event
-- version 

c_info( "load robot task end event" )

-- 注册状态
end_state="end"

-- 注册事件
task_end = 10000

------------------------------------------------------------------------------

-- 状态迁移触发函数
local ai_action_end =function(fsm)
	fsm:done_current_task()
end

-- 状态转移守卫
local master_taskend=function(fsm,from,evt,to)
	
	if from == idle_state and evt == task_end then
		return true
	else
		return false
	end
end

-- 任务执行函数
local task_action_end=function(fsm)
	fsm:post_event(task_end)
end

-- 任务完成执行函数
local done_all_task = function(fsm)
	fsm.manager:c_robot_done()
	
	fsm.signal = MachineState.STOP
end

-----------------------------------------------------------------------------------
-- 任务结束
Robot_End = function(fsm)
	fsm:add_task(task_action_end, done_all_task)
	
	--初始化相对应的状态转移表
	fsm:add_transition(idle_state,task_end,end_state,master_taskend,ai_action_end)
end
