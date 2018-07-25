<?php 
	function strcomp($str1,$str2){ 
	  if($str1 == $str2){ 
	    return TRUE; 
	  }else{ 
	    return FALSE; 
	  } 
	}

	
	$data_login = $_POST['login'];
	$data_pwd = $_POST['pwd'];
	$data_code = $_POST['code'];
	$data_CodeVal = $_POST['CodeVal'];
	$login = "123456";
	$pass = "123456";

	//&& strcomp($data_code , $data_CodeVal)

	if (strcomp($login, $data_login) && strcomp($pass,$data_pwd) ) {
		//$data = responseText: {"Status":"ok","Text":"登录成功<br /><br />欢迎回来","login": "admin"};
		$data["Status"] = "ok";
		$data["Text"] = "登录成功<br /><br />欢迎回来";
		$data["login"] = "admin";
		echo json_encode($data);
	}else {
		$data["Status"] = "Erro";
		$data["Text"] = "账号名或密码有误";
		$data["login"] = "admin";
		//$data = responseText: {"Status":"Erro","Text":"账号名或密码或验证码有误","login": "admin"};
		echo json_encode($data);
	}


?>