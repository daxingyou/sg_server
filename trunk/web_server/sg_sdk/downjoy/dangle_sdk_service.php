<?php

require_once dirname(__FILE__) . '/config.php';
require_once dirname(__FILE__) . '/../base/logger.php';

define("SUCCESS_CODE", 2000); // 成功状态码
define("MASTER_HOST", "https://ctmaster.d.cn/api/cp/checkToken"); // 主路线地址
define("SLAVE_HOST", "https://ctslave.d.cn/api/cp/checkToken"); // 灾备线路地址
define("THIRD_CHECK_HOST", "http://ctslave.downjoy.com/api/cp/checkToken"); // 第三条灾备路线（没发生超时异常时不要调用）

class dangle_sdk_service
{
	/**
	 * 获取验证登陆token的响应内容
	 * @return:自定义关联数组
	格式
	{
		valid: "1",
		is_adult: 1,
		is_certified: 1,
		roll: true,
		interval: 60,
		times: 1,
		msg_code: 2000,
		msg_desc: "成功"
	}
	 */
	public static function getCheckTokenData($token, $umid){
		$reqParam = array();
	    $reqParam["appid"] = $GLOBALS['dj_appId'];
	    $reqParam["umid"] = $umid;
	    $reqParam["token"] = $token;
	    $needSignStr = sprintf("%s|%s|%s|%s", $GLOBALS['dj_appId'], $GLOBALS['dj_appKey'], $token, $umid);
	    $sign = md5($needSignStr);
	    $reqParam["sig"] = $sign;
	    $host = MASTER_HOST;
	    //检查token是否指定灾备url
	    if(strpos($token, "ZB_") === 0){
            $host = SLAVE_HOST;
        }
        $requestBody = service::connectRequestParam($reqParam);
        $responseResult;
		try {
            $responseResult = dangle_sdk_service::post($host, $requestBody);
        } catch (Exception $e) {
            print_r($e->getMessage());
            // 有异常则请求第三条线路
            $host = THIRD_CHECK_HOST;
            $responseResult = dangle_sdk_service::post($host, $requestBody);
        }
        return json_decode($responseResult, true);
	}
	/**
	 * 发送post请求
	 * @param post请求地址
	 * @param 请求参数，格式：自定义关联数组
	 */
	public static function post($url, $reqParam){
		if($GLOBALS['dj_isDebug']){
			echo "\npost_url：" . $url;
			echo "\n请求参数: " . print_r($reqParam, true);
		}
		return service::get_sdk_server_response($url, $reqParam, 1, 5000);
	}
	/*
	生成与客户端协定格式的消息
	返回值:约定格式的关联数组
	格式:
	{
		status:'success', //string;成功'success',失败填其他
		uid:'32412321', //string;就是客户端请求时提供的umid
		message:'abcdef' //string;一些解释说明
	}
	*/
	public static function verifySession($token, $umid){
		$logger = new OutPutLogger($GLOBALS['dj_logName'] . date("Y-m-d"));
		if($GLOBALS['dj_isLog'])
			$logger->record();
		$checkTokenData = dangle_sdk_service::getCheckTokenData($token, $umid);
		if($GLOBALS['dj_isDebug']){
			echo "\n当乐返回消息：" . print_r($checkTokenData, true);
		}
		$toclient = array('uid' => "{$umid}", 'message' => "nothing");
		if($checkTokenData['msg_code'] == SUCCESS_CODE && $checkTokenData['valid'] == 1){
			$toclient['status'] = 'success';
		}
		else{
			$toclient['status'] = 'fail';
		}
		if($GLOBALS['dj_isDebug']){
			echo "\n返回客户端消息：" . print_r($toclient, true);
		}
		if($GLOBALS['dj_isLog'])
			$logger->gen_log();
		return $toclient;
	}
}


?>


