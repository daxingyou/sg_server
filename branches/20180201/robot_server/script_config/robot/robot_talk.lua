-- robot_talk.lua 
-- robot talk event
-- version 

c_info( "load robot talk event" )

------------------------------------------------------------------------------

-- 任务执行函数
local task_action_talk=function(fsm)
	fsm.manager:c_talk_about(fsm.task[fsm.taskfront].task_param1, fsm.task[fsm.taskfront].task_param2)
end

-- 任务完成执行函数
local done_talk_task = function(fsm)
	fsm.publiccd = 500
end

local task_gm_commond=function(fsm)
	fsm.manager:c_gm_command(fsm.task[fsm.taskfront].task_param1)
end

local task_toggle_mount=function(fsm)
	fsm.manager:c_toggle_mount(fsm.task[fsm.taskfront].task_param1)
end

-----------------------------------------------------------------------------------
-- 循环在某个频道输出
Robot_TalkAbout = function(fsm, channel, str, times)
	if times < 1 then return end
	
	for i=1, times do
		fsm:add_once_task(task_action_talk, done_talk_task, channel, str)
	end
end

-- 执行gm命令
Robot_do_gm = function(fsm, str)
	fsm:add_once_task(task_gm_commond, done_talk_task, str)
end

Robot_do_mount = function(fsm, state)
	fsm:add_once_task(task_toggle_mount, done_talk_task, state)
end
