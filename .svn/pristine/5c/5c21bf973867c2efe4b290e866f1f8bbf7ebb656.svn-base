#!/bin/sh
#./proto2php.sh

cpp_path=../protocpp
#lua_path=../protolua

client_to_cpp_rlt=""
#client_to_lua_rlt=""
common_to_cpp_rlt=""
common_to_lua_rlt=""
server_to_cpp_rlt=""
log_to_cpp_rlt=""
gm_to_cpp_rlt=""
gm_common_to_cpp_rlt=""
gen_cmd_rlt=""

function op_proto()
{    
    #if [ -d $lua_path ]
    #then
    #    \rm -rf $lua_path
    #fi
    #mkdir $lua_path

	proto_name="client.proto"
	echo -e "\033[33m==========================================\033[0m"
	echo -e "\033[36mparse "$proto_name" to cpp......\033[0m" 
	client_to_cpp_rlt=$proto_name" to cpp failed!"
	#client_to_lua_rlt=$proto_name" to lua failed!"
	protoc --cpp_out=$cpp_path $proto_name
	if [ $? -eq 0 ];then
		client_to_cpp_rlt=$proto_name" to cpp success!"
		
		#echo -e "\033[33m==========================================\033[0m"
		#echo -e "\033[36mparse "$proto_name" to lua......\033[0m" 
		#luaprotoc $proto_name $lua_path
		#if [ $? -eq 0 ];then
		#	client_to_lua_rlt=$proto_name" to lua success!"		
		#fi	
    fi

	proto_name="common.proto"
	echo -e "\033[33m==========================================\033[0m"
	echo -e "\033[36mparse "$proto_name" to cpp......\033[0m" 
	common_to_cpp_rlt=$proto_name" to cpp failed!"
	common_to_lua_rlt=$proto_name" to lua failed!"
	protoc --cpp_out=$cpp_path $proto_name
	if [ $? -eq 0 ];then
		common_to_cpp_rlt=$proto_name" to cpp success!"
		
		#echo -e "\033[33m==========================================\033[0m"
		#echo -e "\033[36mparse "$proto_name" to lua......\033[0m" 
		#luaprotoc $proto_name $lua_path
		if [ $? -eq 0 ];then
			common_to_lua_rlt=$proto_name" to lua success!"	
		fi		
    fi
	
	proto_name="server.proto"
	echo -e "\033[33m==========================================\033[0m"
	echo -e "\033[36mparse "$proto_name" to cpp......\033[0m" 
	server_to_cpp_rlt=$proto_name" to cpp failed!"
	protoc --cpp_out=$cpp_path $proto_name
	if [ $? -eq 0 ];then
		server_to_cpp_rlt=$proto_name" to cpp success!"	
    fi
	
	proto_name="log.proto"
	echo -e "\033[33m==========================================\033[0m"
	echo -e "\033[36mparse "$proto_name" to cpp......\033[0m" 
	log_to_cpp_rlt=$proto_name" to cpp failed!"
	protoc --cpp_out=$cpp_path $proto_name
	if [ $? -eq 0 ];then
		log_to_cpp_rlt=$proto_name" to cpp success!"
    fi
	
	proto_name="gm.proto"
	echo -e "\033[33m==========================================\033[0m"
	echo -e "\033[36mparse "$proto_name" to cpp......\033[0m" 
	gm_to_cpp_rlt=$proto_name" to cpp failed!"
	protoc --cpp_out=$cpp_path $proto_name
	if [ $? -eq 0 ];then
		gm_to_cpp_rlt=$proto_name" to cpp success!"
    fi
	
	proto_name="gm_common.proto"
	echo -e "\033[33m==========================================\033[0m"
	echo -e "\033[36mparse "$proto_name" to cpp......\033[0m" 
	gm_common_to_cpp_rlt=$proto_name" to cpp failed!"
	protoc --cpp_out=$cpp_path $proto_name
	if [ $? -eq 0 ];then
		gm_common_to_cpp_rlt=$proto_name" to cpp success!"
    fi
	
	#op_cmd
	#proto_name="msg_cmd.proto"
	#echo -e "\033[33m==========================================\033[0m"
	#echo -e "\033[36mparse "$proto_name" to cpp......\033[0m" 
	#server_to_cpp_rlt=$proto_name" to cpp failed!"
	#protoc --cpp_out=$cpp_path $proto_name
	#if [ $? -eq 0 ];then
	#	server_to_cpp_rlt=$proto_name" to cpp success!"	
    #fi
	
	gen_cmd_rlt="gen cmd failed!"
    python ./op_cmd/op_cmd.py
	if [ $? -eq 0 ];then
		\cp -f ./op_cmd/op_cmd.hpp $cpp_path
		rm -f ./op_cmd/op_cmd.hpp
		\cp -f ./op_cmd/msg_cmd.cs ./
		rm -f ./op_cmd/msg_cmd.cs
		\cp -f ./op_cmd/cmd_lua.lua ./
		rm -f ./op_cmd/cmd_lua.lua
		\cp -f ./op_cmd/cmd_desc.cpp $cpp_path
		rm -f ./op_cmd/cmd_desc.cpp
		#\cp -f ./op_cmd/op_cmd.lua $lua_path
		#rm -f ./op_cmd/op_cmd.lua
		gen_cmd_rlt="gen cmd success!"		
	fi
}

op_proto
echo -e "\033[33m==========================================\033[0m"
echo "Result:"
grep "failed" <<< $client_to_cpp_rlt 	>/dev/null && echo -e "\033[31m"$client_to_cpp_rlt"\033[0m" 	|| echo $client_to_cpp_rlt
#grep "failed" <<< $client_to_lua_rlt 	>/dev/null && echo -e "\033[31m"$client_to_lua_rlt"\033[0m" 	|| echo $client_to_lua_rlt
grep "failed" <<< $common_to_cpp_rlt 	>/dev/null && echo -e "\033[31m"$common_to_cpp_rlt"\033[0m" 	|| echo $common_to_cpp_rlt
#grep "failed" <<< $common_to_lua_rlt 	>/dev/null && echo -e "\033[31m"$common_to_lua_rlt"\033[0m" 	|| echo $common_to_lua_rlt
grep "failed" <<< $server_to_cpp_rlt 	>/dev/null && echo -e "\033[31m"$server_to_cpp_rlt"\033[0m" 	|| echo $server_to_cpp_rlt
grep "failed" <<< $log_to_cpp_rlt	 	>/dev/null && echo -e "\033[31m"$log_to_cpp_rlt"\033[0m" 		|| echo $log_to_cpp_rlt
grep "failed" <<< $gm_to_cpp_rlt     	>/dev/null && echo -e "\033[31m"$gm_to_cpp_rlt"\033[0m" 		|| echo $gm_to_cpp_rlt
grep "failed" <<< $gm_common_to_cpp_rlt >/dev/null && echo -e "\033[31m"$gm_common_to_cpp_rlt"\033[0m" 	|| echo $gm_common_to_cpp_rlt
grep "failed" <<< $gen_cmd_rlt 			>/dev/null && echo -e "\033[31m"$gen_cmd_rlt"\033[0m" 			|| echo $gen_cmd_rlt
echo -e "\033[33m==========================================\033[0m"
	
