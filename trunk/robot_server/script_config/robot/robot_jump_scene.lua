-- robot_jump_scene.lua 
-- robot jump_scene event
-- version 

c_info( "load robot jump scene event" )

-- 注册状态
changescene_state="changescene"

-- 注册事件
changescene_event = 400

------------------------------------------------------------------------------
-- 可用条件判断
-- 是否在场景
local ai_condition_scene = function(fsm)
	if fsm.manager:c_get_scene() == fsm.task[fsm.taskfront].task_param1 then return true end
	return false
end

-- 状态迁移触发函数
local ai_action_changescene =function(fsm)
	fsm:done_current_task()
end

-- 状态转移守卫
local master_changescene=function(fsm,from,evt,to)
	local isInScene = ai_condition_scene(fsm)
	
	if from == idle_state and evt == changescene_event then
		if not isInScene then
			return true
		else
			fsm:done_current_task()		-- 已经在这个场景
			return false
		end
	elseif from == changescene_state and evt == changescene_event then
		return isInScene
	else
		print("master_changescene state error", from, evt, to)
		return false
	end
end

-- 任务执行函数
local task_action_changescene=function(fsm)
	if fsm.currstate ~= changescene_state then
		if fsm.manager:c_get_scene() == fsm.task[fsm.taskfront].task_param1 then
			fsm:done_current_task()		-- 已经在这个场景
			return
		else
			fsm.manager:c_jump_scene(fsm.task[fsm.taskfront].task_param1)
		end
	end
	fsm:post_event(changescene_event)
end

-- 任务完成执行函数
local done_changescene_task = function(fsm)
	fsm.publiccd = 500
end

-----------------------------------------------------------------------------------
-- 跳场景
Robot_JumpScene = function(fsm, sceneid)
	fsm:add_task(task_action_changescene, done_changescene_task, sceneid)
	
	fsm:add_transition(idle_state,changescene_event,changescene_state,master_changescene,void_)
	fsm:add_transition(changescene_state,changescene_event,idle_state,master_changescene,ai_action_changescene)
end
