-- robot.lua 
-- robot files entry
-- version 

c_info( "load robot event" )

--��ʼ������
function InitRobotAi(_Self)
	local fsm
	fsm = state_machine.new(idle_state)
	fsm.manager = _Self	--Robotָ��
	fsm.publiccd = 1000	--���ߺ��һ����ִ��lua

	-- һЩgm����
	--Robot_do_gm(fsm, '/funcopenall')
	
	-- ���Ե�½
	-- Robot_Login_test(fsm)
	
	-- ����9���ӿ�
	Robot_Nine_test(fsm)
	
	-- talk
    -- Robot_TalkAbout(fsm, 3, '[EM06]��hi����Ŷ���ý𸻺��ͷ��ٰ����ݰ�����ȻIP�ҽ�������غ�Ȼŷ�ʷ��ź������[EM07]', 100)
    -- Robot_TalkAbout(fsm, 2, '[EM06]��[EM07]�˿���λ����Ƭǰ������', 100)
	
	-- �����Զ�ս��
    -- ��������ս��������-1��������
	-- Robot_Random_fight(fsm, -1)
	
	-- �ٻ�����boss
	-- ��������ȡֵ��Χ0-25, ����Ϊ0�����������IDΪ1-25������һ��boss������Ϊ1-25����ֻ����IDΪ��ֵ��boss
	-- Robot_SummonBoss(fsm, 0)

	-- ���Բ���bossս��
	-- Robot_do_gm(fsm, '/setlevel 40')
	-- Robot_FightBoss(fsm)
	
	-- ���Ը���ս��
	-- Robot_Test_Dungeon(fsm, 3)

	-- ���Կ��ս��
	-- Robot_do_gm(fsm, '/chgtime 20190601200000')
	-- Robot_Arena(fsm)
	
	-- �������
	Robot_End(fsm)
	
	return fsm
end

