<?php 
	function strcomp($str1,$str2){ 
	  if($str1 == $str2){ 
	    return TRUE; 
	  }else{ 
	    return FALSE; 
	  } 
	}

	
	$data_login = $_GET['login'];
	$data_pwd = $_GET['pwd'];
	$login = "test";
	$pass = "test";
	$login1 = "admin";
	$pass1 = "admin";

	//&& strcomp($data_code , $data_CodeVal)

	if ((strcomp($login, $data_login) && strcomp($pass,$data_pwd)) || (strcomp($login1, $data_login) && strcomp($pass1,$data_pwd))) {
		//$data = responseText: {"Status":"ok","Text":"登录成功<br /><br />欢迎回来","login": "admin"};
		$data["Status"] = "ok";
		$data["Text"] = "登录成功<br /><br />欢迎回来";
		$data["login"] = $data_login;
		echo json_encode($data);
	}else {
		$data["Status"] = "Erro";
		$data["Text"] = "账号名或密码有误";
		$data["login"] = $data_login;
		//$data = responseText: {"Status":"Erro","Text":"账号名或密码或验证码有误","login": "admin"};
		echo json_encode($data);
	}


?>