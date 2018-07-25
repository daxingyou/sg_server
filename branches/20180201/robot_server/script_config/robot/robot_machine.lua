-- robot.lua 
-- robot files entry
-- version 

c_info( "load robot machine" )

-----------------------------------------------------------
--------------------AI状态机_begin-------------------------
-- 声明为全局变量
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
	
	fsm.task={}  -- 任务表
	fsm.taskfront = 0 --任务计数
	fsm.taskback = -1 --任务计数
	
	fsm.transitions={}  --状态迁移表
	fsm.currstate = initstate   --当前状态
   
	fsm.events={}	   	--事件集
    fsm.front = 0		--事件计数
    fsm.back = -1		--事件计数
	
	fsm.signal = MachineState.NORMAL
	
    return fsm
end

-- 重置
function state_machine:reset(initstate)
	for i = 0, self.taskback do
		self.task[i].task_done = false
	end
	
	self.taskfront = 0 --任务计数
	
	self.currstate = initstate   --当前状态
   
	self.events={}	   	--事件集
    self.front = 0		--事件计数
    self.back = -1		--事件计数
	
end

--from 源状态
--event 引起迁移的事件
--to 迁移的目标状态，如果是自身迁移则from和to相同
--guard 迁移可以成立的守卫条件
--entry 新状态的进入动作
--action 执行迁移的动作
--leave 转移到其他状态时的离开动作

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

--inaction 任务类型
--doneaction 完成事件
--param1 param2 附加参数
function state_machine:add_task(inaction, doneaction, param1, param2)
	local t = {task_done=false,task_action=inaction,done_action=doneaction,task_param1=param1,task_param2=param2,no_state=false} 
	self.taskback = self.taskback + 1
	self.task[self.taskback] = t
end

-- 此类任务，执行完就置完成，不会发生状态转移
function state_machine:add_once_task(inaction, doneaction, param1, param2)
	self:add_task(inaction, doneaction, param1, param2)
	self.task[self.taskback].no_state = true
end

-- 设置当前任务完成，执行完成函数
function state_machine:done_current_task()
	if self.task[self.taskfront] then 
		self.task[self.taskfront].task_done = true
		self.task[self.taskfront].done_action(self)
	end
end

-- 意外终止
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
--------------------AI状态机_over-------------------------

