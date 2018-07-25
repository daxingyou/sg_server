-- robot_jump_scene.lua 
-- robot jump_scene event
-- version 

c_info( "load robot jump scene event" )

-- ע��״̬
changescene_state="changescene"

-- ע���¼�
changescene_event = 400

------------------------------------------------------------------------------
-- ���������ж�
-- �Ƿ��ڳ���
local ai_condition_scene = function(fsm)
	if fsm.manager:c_get_scene() == fsm.task[fsm.taskfront].task_param1 then return true end
	return false
end

-- ״̬Ǩ�ƴ�������
local ai_action_changescene =function(fsm)
	fsm:done_current_task()
end

-- ״̬ת������
local master_changescene=function(fsm,from,evt,to)
	local isInScene = ai_condition_scene(fsm)
	
	if from == idle_state and evt == changescene_event then
		if not isInScene then
			return true
		else
			fsm:done_current_task()		-- �Ѿ����������
			return false
		end
	elseif from == changescene_state and evt == changescene_event then
		return isInScene
	else
		print("master_changescene state error", from, evt, to)
		return false
	end
end

-- ����ִ�к���
local task_action_changescene=function(fsm)
	if fsm.currstate ~= changescene_state then
		if fsm.manager:c_get_scene() == fsm.task[fsm.taskfront].task_param1 then
			fsm:done_current_task()		-- �Ѿ����������
			return
		else
			fsm.manager:c_jump_scene(fsm.task[fsm.taskfront].task_param1)
		end
	end
	fsm:post_event(changescene_event)
end

-- �������ִ�к���
local done_changescene_task = function(fsm)
	fsm.publiccd = 500
end

-----------------------------------------------------------------------------------
-- ������
Robot_JumpScene = function(fsm, sceneid)
	fsm:add_task(task_action_changescene, done_changescene_task, sceneid)
	
	fsm:add_transition(idle_state,changescene_event,changescene_state,master_changescene,void_)
	fsm:add_transition(changescene_state,changescene_event,idle_state,master_changescene,ai_action_changescene)
end
