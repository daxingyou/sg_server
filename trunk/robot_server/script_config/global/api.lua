-------------------全局函数定义----------------------------

c_info( "load Global api" )
--[[
--脚本日志
function syslog( msg )
	c_debug( "脚本日志:"..msg )
end


--打印当前堆栈
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