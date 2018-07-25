#gm页面传参信息
#[IP:USER:PASSWD:DBNAME;IP:USER:PASSWD:DBNAME]

#拆解合服参数

function split_str()
{
	local ret=""
	local split=(${1//$2/ })
	for var in ${split[@]}
	do
		ret=${ret}" "${var}
	done
	echo $ret
}

function package_ignore()
{
	local dbname=$1
	local ret=""
	
	for var in $ignorelist
	do
		ret="$ret --ignore-table=${dbname}.${var}"

	done
	echo $ret
}

#####################################################
#主库
#str_src="192.168.1.178:root:Sanguo1!:huangyang_trunk_sg_game"
str_src=$1
#从库
#str_tag="192.168.1.6:root:Sanguo1!:design_trunk_sg_game;192.168.1.5:root:Sanguo1!:nei_trunk_sg_game"
str_tag=$2

NOW_TIME=$(date +%Y%m%d_%H%M%S)

DB_EXPORT_DIR="./db_export/bak"

DB_NEW_DIR="./db_export/new"

mysqldump=/usr/bin/mysqldump

ignorelist="global_server_data city country country_office family_war global_mail shop_goods king_war_apply ranklist goods_care rarity_price offline_role_data redbag  general_info general_status general_target_first_name"

#####################################################
function init_dir()
{
	if [ ! -d $DB_EXPROT_DIR ]
	then
		mkdir -p  $DB_EXPROT_DIR
	fi


	local dir_name=${1}
	local tag_dir=${DB_EXPORT_DIR}/${dir_name}

	if [ ! -d $tag_dir ]
	then
		mkdir -p $tag_dir
	fi
	echo $tag_dir
}

function do_export()
{
	local db_ip=$1
	local db_user=$2
	local db_passwd=$3
	local db_name=$4
	local IGNORE_LIST=""
	local NO_CREATE_FLAG=""

	if [[ $5 == "no_create" ]]
	then
		NO_CREATE_FLAG="--no-create-info"
	fi

	if [[ $6 == "igon" ]]
	then
		IGNORE_LIST=$(package_ignore $db_name)  
	fi

	local tag_dir=$(init_dir $db_name)
	local tag_file=${tag_dir}/${NOW_TIME}_${db_name}.sql

	if [ -f $tag_file ]
	then
		ERR "[数据库文件已经存在，可能已经合服，请人工确认!]\n"
		return 50
	fi

    $mysqldump $NO_CREATE_FLAG --compact -u ${db_user} -p${db_passwd} -h${db_ip} ${db_name} $IGNORE_LIST --default-character-set=utf8 > $tag_file
	if [[ $? != 0 ]]
	then
		ERR	"[导出数据库${db_name}失败!]\n"
		return 51
	fi
	return 0
}


#导出合服的数据库
function export_db()
{
#导出主服数据
	local ret=$(split_str $str_src ';')
	for var in $ret 
	do
		local db_info=""
		local index=0
		local db_str=$(split_str $var ':')
		for data in $db_str
		do
			db_info[index]=$data
			((index=index+1))
		done	
		do_export ${db_info[0]} ${db_info[1]} ${db_info[2]} ${db_info[3]}
		local ret=$?
		if [[ $ret !=  0 ]]
		then	
			return $ret
		fi
		break;
	done
#导出目标服数据
	local ret=$(split_str $str_tag ';')
	for var in $ret 
	do
		local db_info=""
		local index=0
		local db_str=$(split_str $var ':')
		for data in $db_str
		do
			db_info[index]=$data
			((index=index+1))
		done	
		do_export ${db_info[0]} ${db_info[1]} ${db_info[2]} ${db_info[3]} "no_create" "igon"
		local ret=$?
		if [[ $ret !=  0 ]]
		then	
			return $ret
		fi
	done
	return 0
}

function do_import()
{
	local db_name=$1
	local tag_dir=$(init_dir $db_name)
	local tag_file=${tag_dir}/${NOW_TIME}_${db_name}.sql

	DEBUG "开始导入数据库${db_name}"
	if [ ! -f $tag_file ]
	then
		ERR "找不到导出的sql文件[$tag_file]"
		return 52
	fi

	local ret=$(split_str $str_src ';')
	for var in $ret 
	do
		local db_info=""
		local index=0
		local db_str=$(split_str $var ':')
		for data in $db_str
		do
			db_info[index]=$data
			((index=index+1))
		done	
		mysql -h${db_info[0]} -u${db_info[1]} -p${db_info[2]} new_${db_info[3]}_$NOW_TIME -f --default-character-set=utf8 < $tag_file  
		local ret=$?
		if [[ $ret !=  0 ]]
		then	
			ERR "导入数据库失败"
			return $ret
		fi
		break
	done
	return 0
}

#导入合服数据库
function import_db()
{
	#导入带建表语句的主库
	local ret=$(split_str $str_src ';')
	for var in $ret 
	do
		local db_info=""
		local index=0
		local db_str=$(split_str $var ':')
		for data in $db_str
		do
			db_info[index]=$data
			((index=index+1))
		done	
		#创建临时库
		local sql_create_tmp_db="CREATE DATABASE IF NOT EXISTS  new_${db_info[3]}_$NOW_TIME  DEFAULT CHARSET utf8 COLLATE utf8_general_ci"
    	mysql -h${db_info[0]}  -u${db_info[1]} -p${db_info[2]} -e "$sql_create_tmp_db"
		local ret=$?
		if [[ $ret !=  0 ]]
		then	
			ERR "创建临时库失败"
			return $ret
		fi

		do_import ${db_info[3]}
		local ret=$?
		if [[ $ret !=  0 ]]
		then	
			ERR "导入数据库失败"
			return $ret
		fi
		break
	done


	#从库导入	
	local ret=$(split_str $str_tag ';')
	for var in $ret 
	do
		local db_info=""
		local index=0
		local db_str=$(split_str $var ':')
		for data in $db_str
		do
			db_info[index]=$data
			((index=index+1))
		done	
		do_import ${db_info[3]}
		local ret=$?
		if [[ $ret !=  0 ]]
		then	
			return $ret
		fi
	done
	return 0
}

#处理全局重命名
function do_del_dumpname()
{
    local db_ip=$1
    local db_user=$2
    local db_passwd=$3
    local db_name=$4

	DEBUG "角色表开始去除重命名"
    local sql_rename_role="update role set name=concat(name,rk) where name in (select name from (select name from role group by name having count(*) >1) as b)"
    mysql -h$db_ip -u$db_user -p$db_passwd $db_name -e "$sql_rename_role"
	local ret=$?
	if [[ $ret !=  0 ]]
	then	
		ERR "表role,去除重命名失败"
		return $ret
	fi

	DEBUG "角色表开始去除重命名"
    local sql_rename_global_role="update global_user set name=concat(name,rk) where name in (select name from (select name from global_user group by name having count(*) >1) as b)"
    mysql -h$db_ip -u$db_user -p$db_passwd $db_name -e "$sql_rename_global_role"
	local ret=$?
	if [[ $ret !=  0 ]]
	then
		ERR "表global_user,去除重命名失败"
		return $ret
	fi

	DEBUG "角色表开始去除重命名"
	local sql_family_name="update family set name=concat(name,rk) where name in (select name from (select name from family group by name having count(*) >1) as b)"
    mysql -h$db_ip -u$db_user -p$db_passwd $db_name -e "$sql_rename_global_role"
	local ret=$?
	if [[ $ret !=  0 ]]
	then
		ERR "表family,去除重名失败"		
		return $ret
	fi
	return 0
}


function del_dumpname()
{
    local ret=$(split_str $str_src ';')
    for var in $ret
    do  
        local db_info=""
        local index=0
        local db_str=$(split_str $var ':')
        for data in $db_str
        do  
            db_info[index]=$data
            ((index=index+1))
        done    

		#删除重命名
		do_del_dumpname ${db_info[0]} ${db_info[1]} ${db_info[2]} new_${db_info[3]}_$NOW_TIME
		local ret=$?
        if [[ $ret !=  0 ]]
		then
            ERR "删除重命名失败"
            return $ret
        fi

		if [ ! -d $DB_NEW_DIR ]
		then
			mkdir -p $DB_NEW_DIR
		fi

		#导出临时库	
		$mysqldump --compact -h${db_info[0]}  -u${db_info[1]} -p${db_info[2]} new_${db_info[3]}_$NOW_TIME --default-character-set=utf8 > ${DB_NEW_DIR}/${NOW_TIME}_sg_game.sql
		local ret=$?
        if [[ $ret !=  0 ]]
		then
			ERR "[导出数据库$new_${db_info[3]}_$NOW_TIME 失败!]\n"
        	return 51
    	fi

		#删除临时库
		local sql_drop_tmp="drop database new_${db_info[3]}_${NOW_TIME}"		
    	mysql -h${db_info[0]}  -u${db_info[1]} -p${db_info[2]} -e "$sql_drop_tmp"
		local ret=$?
        if [[ $ret !=  0 ]]
        then    
            ERR "删除临时库失败"
            return $ret
        fi
        break
    done
	GEN_LOG ${DB_NEW_DIR}/${NOW_TIME}_sg_game.sql	
}

function GEN_LOG()
{
	local target_file=$1
	local merage_list=""
	#导出主服数据
    local ret=$(split_str $str_src ';')
    for var in $ret
    do  
        local db_info=""
        local index=0
        local db_str=$(split_str $var ':')
        for data in $db_str
        do  
            db_info[index]=$data
            ((index=index+1))
        done    
		merage_list="${merage_list} ${db_info[3]}"
        break;
    done

#导出目标服数据
    local ret=$(split_str $str_tag ';')
    for var in $ret
    do  
        local db_info=""
        local index=0
        local db_str=$(split_str $var ':')
        for data in $db_str
        do  
            db_info[index]=$data
            ((index=index+1))
        done    
		merage_list="${merage_list} ${db_info[3]}"
    done

	if [ ! -f $target_file ]	
	then
		ERR "合服[$merage_list] 合并数据库生成文件[$target_file] 失败"
		return 55
	fi
	ECHO "合服[$merage_list] 合并数据库生成文件[$target_file] 成功"
}


function ECHO()
{
	local NOW_TIME=$(date +%Y%m%d-%H:%M:%S)
	echo [${NOW_TIME}][INFO]: $1
}

function ERR()
{
	local NOW_TIME=$(date +%Y%m%d-%H:%M:%S)
	echo [${NOW_TIME}][ERROR]: $1
}

function DEBUG()
{
	local NOW_TIME=$(date +%Y%m%d-%H:%M:%S)
	echo [${NOW_TIME}][DEBUG]: $1
}


function main()
{
	ECHO "合服开始-------------------"
	export_db
	local ret=$?
    if [[ $ret !=  0 ]]
    then    
		ERR "导出数据库失败\n"
		exit $ret
	fi

	import_db
	local ret=$?
    if [[ $ret !=  0 ]]
    then    
		ERR "导入数据库失败\n"
		exit $ret
	fi

	del_dumpname
	local ret=$?
    if [[ $ret !=  0 ]]
    then    
		ERR "去除重名失败"
		exit $ret
	fi
	ECHO "合服结束--------------------"
}

main


