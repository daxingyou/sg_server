-- robot_task_end.lua 
-- robot task_end event
-- version 

c_info( "load robot task end event" )

-- ע��״̬
end_state="end"

-- ע���¼�
task_end = 10000

------------------------------------------------------------------------------

-- ״̬Ǩ�ƴ�������
local ai_action_end =function(fsm)
	fsm:done_current_task()
end

-- ״̬ת������
local master_taskend=function(fsm,from,evt,to)
	
	if from == idle_state and evt == task_end then
		return true
	else
		return false
	end
end

-- ����ִ�к���
local task_action_end=function(fsm)
	fsm:post_event(task_end)
end

-- �������ִ�к���
local done_all_task = function(fsm)
	fsm.manager:c_robot_done()
	
	fsm.signal = MachineState.STOP
end

-----------------------------------------------------------------------------------
-- �������
Robot_End = function(fsm)
	fsm:add_task(task_action_end, done_all_task)
	
	--��ʼ�����Ӧ��״̬ת�Ʊ�
	fsm:add_transition(idle_state,task_end,end_state,master_taskend,ai_action_end)
end
