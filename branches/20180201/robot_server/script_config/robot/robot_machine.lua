-- robot.lua 
-- robot files entry
-- version 

c_info( "load robot machine" )

-----------------------------------------------------------
--------------------AI״̬��_begin-------------------------
-- ����Ϊȫ�ֱ���
state_machine={}

MachineState = 
{ 
    "NORMAL", 
	"ERROR",
}

MachineState = CreatEnumTable(MachineState) 

state_machine.new=function(initstate)  
    local fsm = {}
    setmetatable(fsm,{__index = state_machine} )
   --for k, v in pairs(state_machine) do fsm[k] = v end
	
	fsm.task={}  -- �����
	fsm.taskfront = 0 --�������
	fsm.taskback = -1 --�������
	
	fsm.transitions={}  --״̬Ǩ�Ʊ�
	fsm.currstate = initstate   --��ǰ״̬
   
	fsm.events={}	   	--�¼���
    fsm.front = 0		--�¼�����
    fsm.back = -1		--�¼�����
	
	fsm.signal = MachineState.NORMAL
	
    return fsm
end

-- ����
function state_machine:reset(initstate)
	for i = 0, self.taskback do
		self.task[i].task_done = false
	end
	
	self.taskfront = 0 --�������
	
	self.currstate = initstate   --��ǰ״̬
   
	self.events={}	   	--�¼���
    self.front = 0		--�¼�����
    self.back = -1		--�¼�����
	
end

--from Դ״̬
--event ����Ǩ�Ƶ��¼�
--to Ǩ�Ƶ�Ŀ��״̬�����������Ǩ����from��to��ͬ
--guard Ǩ�ƿ��Գ�������������
--entry ��״̬�Ľ��붯��
--action ִ��Ǩ�ƵĶ���
--leave ת�Ƶ�����״̬ʱ���뿪����

function state_machine:add_transition(infrom,inevt,into,inguard,inaction)
    local transition = self.transitions[infrom]

	local t = {to=into,guard=inguard,action=inaction} 
	if transition then
		--transition[inevt]=t
		if transition[inevt] == nil then transition[inevt]=t end
	else
		transition={}
		transition[inevt] = t
		self.transitions[infrom]=transition
    end

end

function state_machine:do_transition(evt)
	--print("do_transition")
	--print("self.currstate=", self.currstate, "enent=", evt)
	--print("evt=", evt)
	if self.transitions[self.currstate] == nil then return end	
	
	local  row = assert(self.transitions[self.currstate] )    
	--print("transition=", row[evt])
	local  transition = row[evt]
	if not transition then return end
	assert(transition.to) 
	if transition.guard(self,self.currstate,evt,transition.to) then
		transition.action(self)
		self.currstate = transition.to
	end
end

function state_machine:post_event(evt)

	while self.front <= self.back do
		local event = self.events[self.front]
		if event == evt then return end
		self.front = self.front + 1
	end
	 
	self.front = 0
	self.back = self.back + 1
	self.events[self.back] = evt
			
end

function state_machine:process_event() 
	--print("front=",  self.front,"back=",  self.back)
    if  self.front then 
        while self.front <= self.back do
            local event = self.events[self.front]
                self.events[self.front] = nil        
                self.front = self.front + 1
                self:do_transition(event)
				--print("event"..event)
		end
		self.front = 0
		self.back = -1
    end 
end

--inaction ��������
--doneaction ����¼�
--param1 param2 ���Ӳ���
function state_machine:add_task(inaction, doneaction, param1, param2)
	local t = {task_done=false,task_action=inaction,done_action=doneaction,task_param1=param1,task_param2=param2,no_state=false} 
	self.taskback = self.taskback + 1
	self.task[self.taskback] = t
end

-- ��������ִ���������ɣ����ᷢ��״̬ת��
function state_machine:add_once_task(inaction, doneaction, param1, param2)
	self:add_task(inaction, doneaction, param1, param2)
	self.task[self.taskback].no_state = true
end

-- ���õ�ǰ������ɣ�ִ����ɺ���
function state_machine:done_current_task()
	if self.task[self.taskfront] then 
		self.task[self.taskfront].task_done = true
		self.task[self.taskfront].done_action(self)
	end
end

-- ������ֹ
function state_machine:terminated(logerr)
	c_error(logerr)
	
	self.signal = MachineState.ERROR
end

function state_machine:do_task()
	while self.taskfront <= self.taskback do
		
		if self.task[self.taskfront].task_done == true then self.taskfront = self.taskfront + 1 end
		if self.taskfront > self.taskback then return end
		
		self.task[self.taskfront].task_action(self)
		if self.task[self.taskfront].no_state == true then self:done_current_task() end
		return
	end
end
--------------------AI״̬��_over-------------------------

