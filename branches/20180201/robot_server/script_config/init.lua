--init.lua
--场景服务器 脚本入口

collectgarbage()

--根目录，全局变量
ROOT_DIR = c_get_root_dir()

--随机数种子
math.randomseed( os.time() )

--加载脚本文件函数------------------------------
function DoFile( file )
	local file = ROOT_DIR .. file
	if c_do_file( file ) == false then
		local chunk,err = assert( loadfile( file ) )
		if not chunk then
			local msg = "" .. err
			c_debug( "脚本日志:" + msg )
		end
	end
end

function string.split(str, delimiter)
	if str==nil or str=='' or delimiter==nil then
		return nil
	end
	
    local result = {}
    for match in (str..delimiter):gmatch("(.-)"..delimiter) do
		if match ~= "" then
			table.insert(result, match)
		end
    end
    return result
end

--加载文件夹下所有脚本文件
--preCallBack 加载脚本文件前的操作(参数为文件名)
--sufCallBack 加载脚本文件后的操作(参数为文件名)
function DoFileByFolder( path, preCallBack, sufCallBack )
	local listString = c_get_file_list_by_folder( path )
	if listString == nil then
		return
	end
	--c_info( "listString "..listString )
	
	local tb = string.split(listString, " ")
	if tb == nil then
		return
	end
	
	for k, v in pairs(tb) do
		if v ~= "" and v ~= " " then
			local file = v
			--c_info( "file "..file )
			local name = string.gsub( file, ".lua", "" )
			--c_info( "name "..name )
			if preCallBack ~= nil then
				preCallBack( name )
			end

			DoFile( path.."/"..file )
			if sufCallBack ~= nil then
				sufCallBack( name )
			end
		end
	end
end

--文件加载---------------------------------------------
DoFileByFolder( "global", nil, nil )

--文件状态机-------------------------------------------
DoFileByFolder( "robot", nil, nil )
