-- robot_arena.lua 
-- robot arena event
-- version 

c_info( "load robot arena event" )

-- 注册状态
arena_join_state="arena_join"
arena_fight_state="arena_fight"

-- 注册事件
arena_join_event=601
arena_match_event=602
arena_end_event=603

-- 循环匹配时间
local match_interval_time = 10	-- 秒

-- 匹配失败多少次输出一下log
local log_max_times = 5

-- 任务执行函数 -------------------------------------------------------------------
-- 初始化
local task_arena_init = function(fsm)
	if fsm.log_times == nil then fsm.log_times = 0 end
	if fsm.match_time == nil then fsm.match_time = 0 end
	
	fsm.log_times = 0
	fsm.match_time = 0
end

-- 加入竞技场
local task_arena_join = function(fsm)
	if fsm.manager:c_is_join_arena() == false then
		fsm.manager:c_join_arena()
		fsm.publiccd = 1000 	-- 等待一段时间，保证消息返回
	end
	
	fsm:post_event(arena_join_event)
end

-- 开始匹配
local task_arena_match = function(fsm)
	if os.time() > fsm.match_time + match_interval_time then
		if fsm.log_times >= log_max_times then 
			print ('arena match', log_max_times, 'times')
			fsm.log_times = 0
		end
		
		fsm.manager:c_arena_match()
		
		fsm.match_time = os.time()
		fsm.log_times = fsm.log_times + 1
	end
	
	fsm:post_event(arena_match_event)
end

-- 战斗是否结束
local task_arena_fight_end = function(fsm)
	fsm:post_event(arena_end_event)
end

-- 任务完成执行函数 ---------------------------------------------------------------
-- 初始化结束
local task_arena_init_done = function(fsm)
	if fsm.task[fsm.taskfront] then fsm.task[fsm.taskfront].task_done = true end
	fsm.publiccd = 200
	print("task arena init done!")
end

local task_arena_join_done = function(fsm)
	if fsm.task[fsm.taskfront] then fsm.task[fsm.taskfront].task_done = true end
	fsm.publiccd = 500
	print ('task arena join done!')
end

local task_arena_match_done = function(fsm)
	if fsm.task[fsm.taskfront] then fsm.task[fsm.taskfront].task_done = true end
	fsm.publiccd = 500
	print ('task arena match done!')
end

local task_arena_fight_end_done = function(fsm)
	if fsm.task[fsm.taskfront] then fsm.task[fsm.taskfront].task_done = true end
	fsm.publiccd = 500
	print ('task arena fight_end done!')
end

-- 状态迁移触发函数 --------------------------------------------------------------
local ai_action_arena = function(fsm)
	if fsm.task[fsm.taskfront] then fsm.task[fsm.taskfront].done_action(fsm) end
end

-- 状态转移守卫 -------------------------------------------------------------------
local master_join_arena = function(fsm,from,evt,to)
	if from == idle_state and evt == arena_join_event then
		return fsm.manager:c_is_join_arena()
	else
		print("master_join_arena state error", from, evt, to)
		return false
	end
end

local master_match_arena = function(fsm,from,evt,to)
	if from == arena_join_state and evt == arena_match_event then
		return fsm.manager:c_is_arena_fighting()
	else
		print("master_match_arena state error", from, evt, to)
		return false
	end
end

local master_fight_end = function(fsm,from,evt,to)
	if from == arena_fight_state and evt == arena_end_event then
		return not fsm.manager:c_is_arena_fighting()
	else
		print("master_fight_end state error", from, evt, to)
		return false
	end
end
-----------------------------------------------------------------------------------
-- 竞技场测试
Robot_Arena = function(fsm)
	fsm:add_once_task(task_arena_init, task_arena_init_done)		-- 初始化一下
	fsm:add_task(task_arena_join, task_arena_join_done)				-- 先加入竞技场
	fsm:add_task(task_arena_match, task_arena_match_done)			-- 竞技场匹配
	fsm:add_task(task_arena_fight_end, task_arena_fight_end_done)	-- 匹配成功，等待战斗结果
	
	fsm:add_transition(idle_state,arena_join_event,arena_join_state,master_join_arena,ai_action_arena)
	fsm:add_transition(arena_join_state,arena_match_event,arena_fight_state,master_match_arena,ai_action_arena)
	fsm:add_transition(arena_fight_state,arena_end_event,idle_state,master_fight_end,ai_action_arena)
end
