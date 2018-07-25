#!/bin/sh

array=""
index=0
i_count=1

function set_xml_value()
{   
	if [[ ! -n $1  || ! -n $2  || ! -n $3 ]]
	then
		echo -e "\033[1;35m参数错误：[$1] [$2] [$3]\033[0m"
		return
	fi


    local string=$1
	local value=$2
	local file_name=$3
    local split=(${string//./ })
    let count=0 
    for var in ${split[@]}
    do  
        array[$count]=$var
        ((count=count+1))
    done

	local file_len=$(wc -l $file_name | awk -F ' ' '{print $1}')

	count=1
	while read LINE
	do
		key=$(get_check_key)
		ret=$(echo $LINE | sed -n "/<$key>/p" | wc -l)
		if ((  $ret == '1' )); then
			if  (( $i_count == ${#array[@]} ))
			then
				set_check_key 
				break
			fi
			set_check_key 
		fi
		((count=count+1))	
	done<$file_name
	
	if (( $count >= $file_len ))
	then
		echo -e "\033[1;37;31m错误配置:$1 节点$(get_check_key)不存在 \033[0m"
		return
	fi
	
	sed -i "$count s/>.*</>$value</"  $file_name
	#echo "sed -i '$count s/>.*</>$value</'  $file_name"
}



function get_check_key()
{
	echo ${array[$index]}
}

function set_check_key()
{
	((index=index+1))
	((i_count=i_count+1))
}

function test_array()
{
    for element in ${array[@]}
    do
        echo $element
    done
}

set_xml_value $1 $2 $3
