-- robot.lua 
-- robot files entry
-- version 

c_info( "load robot files entry" )

-- 是否设置开启循环
ROBOT_LOOP = false

function script_robot_t:onInit(id)
	--print("onInit".."FSM"..id)
	local data = "FSM"..id
	local tself = rawget(_G,data)
	if tself == nil then 
		tself = {}
		rawset(_G,data,tself)
	end
	
	tself.fsm = InitRobotAi(self)	-- 函数在robot_event
end

function script_robot_t:onReset(id)
	local tself = assert(rawget(_G,"FSM"..id))
	
	tself.fsm = nil
end

function script_robot_t:onFinalize(id)
	local tself = assert(rawget(_G,"FSM"..id))
	tself.fsm = nil
	
	tself = nil
end

function script_robot_t:onError(id)
	local tself = assert(rawget(_G,"FSM"..id))
	
	if tself.fsm then tself.fsm:terminated("robot error stop") end
end

function script_robot_t:onThink(id, frametime)
	local tself = assert(rawget(_G,"FSM"..id))
	if tself.fsm then
		if tself.fsm.signal == MachineState.ERROR then return end
		
		if tself.fsm.publiccd > 0 then
			if tself.fsm.publiccd > frametime then 
				tself.fsm.publiccd = tself.fsm.publiccd - frametime
				return
			else
			    tself.fsm.publiccd = 0
			end
		end
		
		if ROBOT_LOOP and tself.fsm.currstate == end_state then
			tself.fsm:reset(idle_state)
			--print("restart!")
		end
		
		tself.fsm:do_task()
		
		tself.fsm:process_event()
	end
end



