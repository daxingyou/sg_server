<?php
class service{
	//获取sdk响应
    public static function get_sdk_server_response($url, $data, $loop_num, $time_out) {
		return service::loop_post($url, $data, $loop_num, $time_out);	
    }

    // 获取毫秒级的时间参数
     public static  function getMillisecond() {
        $time = explode ( " ", microtime () );
        $time = $time [1] . ($time [0] * 1000);
        $time2 = explode ( ".", $time );
        $time = $time2 [0];
        return $time;
    }

    //loop post 
    public static function loop_post($url, $datas, $loop_num, $time_out){
		$coutent=false;
        $begin = time();
        for ($i = 0; $i < $loop_num; $i++){
            $content = self::curl_post($url, $datas, $time_out);
            if( time() - $begin >= 20 ){
                break;
            }
        }
        return $content;
    }

    //post url
    public static function curl_post($url, $data, $timeout) {
        $curl = curl_init();

        //设置抓取的url
        curl_setopt($curl, CURLOPT_URL, $url);
        curl_setopt($curl, CURLOPT_CONNECTTIMEOUT, $timeout);//设置超时时间
        curl_setopt($curl, CURLOPT_TIMEOUT, $timeout);//设置超时时间
        curl_setopt($curl, CURLOPT_USERAGENT, 'Mozilla/5.0 (compatible; MSIE 5.01; Windows NT 5.0)');

        //设置header
        curl_setopt($curl, CURLOPT_HEADER, 0);

        //设置cURL 参数，要求结果保存到字符串中还是输出到屏幕上。
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($curl, CURLOPT_POST, 1);
        curl_setopt($curl, CURLOPT_POSTFIELDS, $data);

        // 运行cURL，请求网页
        $document = curl_exec($curl);
        $errno=curl_errno($curl);
        if ($errno) {
			$document["state"]->code=$errno;
			$document["state"]->msg="curl unkonw error";
        }
        curl_close($curl);
        return $document;
    }
}
?>
