-- robot.lua 
-- robot files entry
-- version 

c_info( "load robot event" )

local MapTable = 
{
{id = 1, pos = {{{153, 90}, {140, 77}}, {{148, 157}, {146, 128}}, {{91, 34}, {114, 55}}}},
{id = 2, pos = {{{214, 101}, {213, 146}}, {{182, 120}, {164, 138}}, {{214, 209}, {214, 244}}, {{232, 290}, {256, 290}}}},
{id = 3, pos = {{{256, 152}, {280, 152}}, {{198, 152}, {173, 152}}, {{205, 222}, {198, 251}}}},
{id = 4, pos = {{{104, 79}, {87, 97}}, {{43, 90}, {61, 73}}, {{51, 147}, {54, 173}}}},
{id = 5, pos = {{{256, 152}, {280, 152}}, {{198, 152}, {173, 152}}, {{205, 222}, {198, 251}}}},
{id = 6, pos = {{{104, 79}, {87, 97}}, {{43, 90}, {61, 73}}, {{51, 147}, {54, 173}}}},
{id = 7, pos = {{{256, 152}, {280, 152}}, {{198, 152}, {173, 152}}, {{205, 222}, {198, 251}}}},
{id = 8, pos = {{{104, 79}, {87, 97}}, {{43, 90}, {61, 73}}, {{51, 147}, {54, 173}}}},
{id = 9, pos = {{{256, 152}, {280, 152}}, {{198, 152}, {173, 152}}, {{205, 222}, {198, 251}}}},
{id = 10, pos = {{{104, 79}, {87, 97}}, {{43, 90}, {61, 73}}, {{51, 147}, {54, 173}}}},
{id = 11, pos = {{{256, 152}, {280, 152}}, {{198, 152}, {173, 152}}, {{205, 222}, {198, 251}}}},
{id = 12, pos = {{{104, 79}, {87, 97}}, {{43, 90}, {61, 73}}, {{51, 147}, {54, 173}}}},
{id = 13, pos = {{{200, 200}, {180, 200}}, {{237, 125}, {213, 125}}, {{255, 200}, {280, 200}}}},
{id = 14, pos = {{{200, 200}, {180, 200}}, {{237, 125}, {213, 125}}, {{255, 200}, {280, 200}}}},
{id = 15, pos = {{{200, 200}, {180, 200}}, {{237, 125}, {213, 125}}, {{255, 200}, {280, 200}}}},
--{id = 16, pos = {{{200, 200}, {180, 200}}, {{237, 125}, {213, 125}}, {{255, 200}, {280, 200}}}},
--{id = 17, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
--{id = 18, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
--{id = 19, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
--{id = 20, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
--{id = 21, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
--{id = 22, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
--{id = 23, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
--{id = 24, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
--{id = 25, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
--{id = 26, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
--{id = 27, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
--{id = 28, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
--{id = 29, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
--{id = 30, pos = {{{108, 229}, {108, 250}}, {{186, 225}, {200, 246}}, {{185, 146}, {188, 123}}}},
}

--	Robot_Move(fsm, 308, 186)
--	Robot_JumpScene(fsm, 2)
--	Robot_JumpScene(fsm, 1)
--	Robot_Test_Dungeon(fsm, 3)
--	Robot_TalkAbout(fsm, 1, "I am done", 1)

--初始化函数
function InitRobotAi(_Self)
	local fsm
	fsm = state_machine.new(idle_state)
	fsm.manager = _Self	--Robot指针
	fsm.publiccd = 1000	--上线后等一会再执行lua
	
	local map_index = math.random(#MapTable)
	local pos_index = math.random(table.getn(MapTable[map_index].pos))
	
	--print (map_index)
	
	Robot_JumpScene(fsm, MapTable[map_index].id)
	Robot_init_position(fsm, MapTable[map_index].pos[pos_index][1][1], MapTable[map_index].pos[pos_index][1][2])
	
	for var = 1, 100 do  
		Robot_Move(fsm, MapTable[map_index].pos[pos_index][1][1], MapTable[map_index].pos[pos_index][1][2])
		Robot_Move(fsm, MapTable[map_index].pos[pos_index][2][1], MapTable[map_index].pos[pos_index][2][2])
	end 

	Robot_End(fsm)
	
	return fsm
end

