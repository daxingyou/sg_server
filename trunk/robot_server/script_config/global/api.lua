-------------------ȫ�ֺ�������----------------------------

c_info( "load Global api" )
--[[
--�ű���־
function syslog( msg )
	c_debug( "�ű���־:"..msg )
end


--��ӡ��ǰ��ջ
function trace()
	c_debug( debug.traceback() )
end
]]

function CreatEnumTable(tbl, index) 
    assert(tbl) 
    local enumtbl = {} 
    local enumindex = index or 0 
    for i, v in ipairs(tbl) do 
        enumtbl[v] = enumindex + i 
    end 
    return enumtbl 
end 