--User API Define------------------------------------

c_info( "load player lua" )

--[[
function script_role_t:debug( msg )
	self:c_debug( msg )
end

function script_role_t:info( msg )
	self:c_info( msg )
end

function script_role_t:error( msg )
	self:c_error( msg )
end
--]]