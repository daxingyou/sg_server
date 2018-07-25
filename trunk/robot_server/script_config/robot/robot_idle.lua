-- robot_idle.lua 
-- robot idle event
-- version 

c_info( "load robot idle event" )

-- ע��idle״̬
idle_state="idle"

-- ע��idle�¼�
idle_event = 100

------------------------------------------------------------------------------

-- ����idle״̬Ǩ�ƴ�������
void_ = function(fsm)
	
end

ai_action_idle = function(fsm)
	fsm:done_current_task()
end

-- ����ִ�к���
local task_init_robot = function(fsm)
	fsm.manager:c_fast_move(fsm.task[fsm.taskfront].task_param1, fsm.task[fsm.taskfront].task_param2)
end

-- �������ִ�к���
local done_init_task = function(fsm)
	fsm.publiccd = 500
end

-----------------------------------------------------------------------------------
-- ��ɫ��ʼ����ĳ��λ��
Robot_init_position = function(fsm, x, y)
	fsm:add_once_task(task_init_robot, done_init_task, x, y)
end
