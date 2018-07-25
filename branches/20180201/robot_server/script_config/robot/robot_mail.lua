-- robot_mail.lua 
-- robot mail event
-- version 

c_info( "load robot mail event" )

-- 注册状态
mail_del_state="del_mail"
mail_open_state="opem_mail"
mail_get_state="get_addenda"

-- 注册事件
mail_del_event = 500
mail_open_event = 501
mail_get_event = 502

------------------------------------------------------------------------------
-- 状态迁移触发函数
local ai_action_mail = function(fsm)
	fsm:done_current_task()
end

-- 状态转移守卫
local master_mail_del=function(fsm,from,evt,to)
	local mail_num = fsm.manager:c_get_mail_num()
	
	if from == idle_state and evt == mail_del_event then
		if mail_num > 0 then
			return true
		else
			ai_action_mail(fsm)
			return false
		end
	elseif from == mail_del_state and evt == mail_del_event then
		return mail_num == 0
	else
		print("master_mail_del state error", from, evt, to)
		return false
	end
end

local master_mail_open=function(fsm,from,evt,to)
	local have_new = fsm.manager:c_have_new_mail()
	
	if from == idle_state and evt == mail_open_event then
		if have_new then
			return true
		else
			ai_action_mail(fsm)
			return false
		end
	elseif from == mail_open_state and evt == mail_open_event then
		return not have_new
	else
		print("master_mail_open state error", from, evt, to)
		return false
	end
end

local master_mail_get=function(fsm,from,evt,to)
	local index = fsm.manager:c_get_addenda_mail_index()
	
	if from == idle_state and evt == mail_get_event then
		if index >= 0 then
			return true
		else
			ai_action_mail(fsm)
			return false
		end
	elseif from == mail_get_state and evt == mail_get_event then
		return index < 0
	else
		print("master_mail_get state error", from, evt, to)
		return false
	end
end

-- 任务执行函数
-- 得到邮件列表
local task_action_mail_get = function(fsm)
	fsm.manager:c_get_mail_list()
end

-- 一份一份删除所有邮件
local task_action_mail_del = function(fsm)
	if fsm.manager:c_get_mail_num() > 0 then
		fsm.manager:c_del_one_mail()
	end
	
	fsm:post_event(mail_del_event)
	--print("task_action_mail_del", fsm.manager:c_get_mail_num())
end

-- 通过GM给自己发若干份邮件
local task_action_gm_send_mail = function(fsm)
	fsm.manager:c_random_mail(fsm.task[fsm.taskfront].task_param1)
end

-- 一键删除所有邮件
local task_action_quick_del_all = function(fsm)
	fsm.manager:c_one_key_del_all()
end

-- 一份一份的打开所有邮件
local task_action_mail_open = function(fsm)
	if fsm.manager:c_have_new_mail() then
		fsm.manager:c_open_one_mail()
	end
	
	fsm:post_event(mail_open_event)
end

-- 一份一份的去拿所有附件
local task_action_addenda_get = function(fsm)
	if fsm.manager:c_get_addenda_mail_index() >= 0 then
		fsm.manager:c_get_mail_addenda(fsm.manager:c_get_addenda_mail_index())
	end
	
	fsm:post_event(mail_get_event)
end

-- 任务完成执行函数
local done_get_list = function(fsm)
	fsm.publiccd = 1000	-- 给予服务器足够时间回复消息
	--print("send get mail list!")
end

local done_del_all_mails = function(fsm)
	fsm.publiccd = 500
	--print("del all mail done!")
end

local done_gm_send_mails = function(fsm)
	fsm.publiccd = 1000	-- 给予服务器足够时间回复消息
	--print("gm send mails done!num:", fsm.task[fsm.taskfront].task_param1)
end

local done_quick_del_all_mails = function(fsm)
	fsm.publiccd = 1000	-- 给予服务器足够时间回复消息
	--print("done quick del all mails end!")
end

local done_open_all_mails = function(fsm)
	fsm.publiccd = 500
	--print("opened all mails done!")
end

local done_get_all_mails = function(fsm)
	fsm.publiccd = 500
	--print("all mails addenda has got done!")
end

-----------------------------------------------------------------------------------
-- 邮件测试流程
Robot_Mail = function(fsm)
	fsm:add_once_task(task_action_gm_send_mail, done_gm_send_mails, math.random(1))	-- 给自己随机发邮件
	fsm:add_once_task(task_action_mail_get, done_get_list)		-- 得到邮件列表
	fsm:add_task(task_action_mail_open, done_open_all_mails)	-- 一份一份打开所有新邮件
	fsm:add_task(task_action_addenda_get, done_get_all_mails)	-- 一份一份拿去所有附件
	fsm:add_task(task_action_mail_del, done_del_all_mails)		-- 删除所有邮件
	
	fsm:add_transition(idle_state,mail_del_event,mail_del_state,master_mail_del,void_)
	fsm:add_transition(mail_del_state,mail_del_event,idle_state,master_mail_del,ai_action_mail)
	
	fsm:add_transition(idle_state,mail_open_event,mail_open_state,master_mail_open,void_)
	fsm:add_transition(mail_open_state,mail_open_event,idle_state,master_mail_open,ai_action_mail)
	
	fsm:add_transition(idle_state,mail_get_event,mail_get_state,master_mail_get,void_)
	fsm:add_transition(mail_get_state,mail_get_event,idle_state,master_mail_get,ai_action_mail)
end
