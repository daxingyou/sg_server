-- robot_arena.lua 
-- robot arena event
-- version

c_info( "load robot arena event" )

-- 注册状态
arena_join_state = "arena_join"
arena_match_state = "arena_match"
arena_fight_state = "arena_fight"
arena_auto_fight_state = "arena_auto_fight"

-- 注册事件
arena_join_event = 601
arena_match_event = 602
arena_wait_fight_event = 603
arena_end_event = 604

-- 循环匹配时间
local match_interval_time = 5	-- 秒

-- 匹配失败多少次输出一下log
local log_max_times = 5

local add_tickets = function(fsm)	
	fsm.manager:c_gm_command('/additem 50025 9999') --添加门票
end

-- 任务执行函数 -------------------------------------------------------------------
-- 初始化
local task_arena_init = function(fsm)
	if fsm.log_times == nil then fsm.log_times = 0 end
	if fsm.match_time == nil then fsm.match_time = 0 end
	
	fsm.log_times = 0
	fsm.match_time = 0
	add_tickets(fsm)	--给机器人添加门票
end

local task_arena = function(fsm)
	if fsm.currstate == idle_state then
		if fsm.manager:c_is_join_arena() == false then
			fsm.manager:c_join_arena()
		end
		fsm:post_event(arena_join_event)

	elseif fsm.currstate == arena_join_state then
		if os.time() > fsm.match_time + match_interval_time then
			if fsm.log_times >= log_max_times then 
				print ('arena match', log_max_times, 'times')
				fsm.log_times = 0
			end
		
			fsm.manager:c_arena_match()
		
			fsm.match_time = os.time() --上一次匹配的时间
			fsm.log_times = fsm.log_times + 1
			fsm.publiccd = 1000
		end
		fsm:post_event(arena_match_event)
		
	elseif fsm.currstate == arena_match_state then
		fsm:post_event(arena_wait_fight_event)

	elseif fsm.currstate == arena_fight_state then
		fsm:post_event(arena_end_event)

	else
		print ('field_boss failed state:', fsm.currstate)
	end
end


-- 任务完成执行函数 ---------------------------------------------------------------
-- 初始化结束
local task_arena_init_done = function(fsm)
	if fsm.task[fsm.taskfront] then fsm.task[fsm.taskfront].task_done = true end
	fsm.publiccd = 200
	print("task arena init done!")
end

local task_arena_done = function(fsm)
	print("done")
end

-- 状态迁移触发函数 --------------------------------------------------------------
local ai_action_auto_fight = function(fsm)
	fsm.manager:c_try_auto_fight()
end

local ai_action_arena_done = function(fsm)
	fsm:done_current_task()
end

-- 状态转移守卫 -------------------------------------------------------------------
local master_arena_guard = function(fsm, from, evt, to)
	if from == idle_state and evt == arena_join_event then
		local is_joined = fsm.manager:c_is_join_arena()
		if not is_joined then
			fsm.publiccd = 10000	--报名失败等10s继续报名
		end
		return is_joined
	elseif from == arena_join_state and evt == arena_match_event then
		return fsm.manager:c_is_arena_matched()
	elseif from == arena_match_state and evt == arena_wait_fight_event then
		return fsm.manager:c_is_fighting()
	elseif from == arena_fight_state and evt == arena_end_event then
		return not fsm.manager:c_is_fighting()
	end
end

-----------------------------------------------------------------------------------
-- 竞技场测试
Robot_Arena = function(fsm)
	fsm:add_once_task(task_arena_init, task_arena_init_done)		-- 初始化一下
	fsm:add_task(task_arena, task_arena_done)				-- 先加入竞技场

	fsm:add_transition(idle_state,			arena_join_event,		arena_join_state,	master_arena_guard,	void_)	--报名
	fsm:add_transition(arena_join_state,	arena_match_event,		arena_match_state,	master_arena_guard,	void_)	--匹配
	fsm:add_transition(arena_match_state,	arena_wait_fight_event,	arena_fight_state,	master_arena_guard,	ai_action_auto_fight)	--等待服务器loading信息
	fsm:add_transition(arena_fight_state,	arena_end_event,		idle_state,			master_arena_guard,	ai_action_arena_done)
end
