<?php
require_once dirname(dirname(__FILE__)).'/base/service.class.php';
require_once dirname(dirname(__FILE__)).'/jiuyou/config.php';

class jiuyou_sdk_service extends service{
	public static function verifySession($sid) {	
		global $sdk_conf;	#引用全局变量

		$params=array();
		$params["sid"]=$sid;
		
		$url=$sdk_conf["JIUYOU"]->veritfy_session_url;
		$data=self::assemblyParameters($params);
		$result=parent::get_sdk_server_response($url, json_encode($data), $sdk_conf["JIUYOU"]->loop_num, $sdk_conf["JIUYOU"]->time_out  );

       	$result_de=json_decode($result);
       	$code=$result_de->state->code;
       	$msg=$result_de->state->msg;
       	$uid=$result_de->data->accountId;

		if ($code == 1)
		{
			$rtl_data["status"]="success";	
			$rtl_data["uid"]="{$uid}";	
		}else{
			$rtl_data["status"]="failed";	
			$rtl_data["uid"]="0";	
		}

		$rtl_data["message"]->code=$code;	
		$rtl_data["message"]->msg=$msg;	

		return $rtl_data;
	}


	//生成参数串
    public static function get_params_str( array $params ){
        ksort($params);
        $enData='';
        foreach( $params as $key=>$val ){
            $enData = $enData.$key.'='.$val;
        }
		return $enData;
    }

	//生成post_data
	private static function assemblyParameters( array $params ){
		global $sdk_conf;	#引用全局变量

        $game_param = array();
        $game_param["gameId"]=$sdk_conf["JIUYOU"]->gameId;

        $sign_source=self::get_params_str($params).$sdk_conf["JIUYOU"]->key;
        $sign=md5($sign_source);

        $request_params["id"]=parent::getMillisecond();
        $request_param["game"]=$game_param;
        $request_param["data"]=$params;
        $request_param["sign"]=$sign;

        return $request_param;
    }
}
?>
