<?php
require_once('./lib/tp_netclient.php');
require_once('./lib/probuff/message/pb_message.php');	

class tcp_client {
	public $nettcp;

	public function connect( $ip , $port ){
		$this->nettcp = new TP_NetClient_TCP;

		$result=array();
		if($this->nettcp->connect($ip,$port,60) == false) {
			$result['code']="-1";
			$result['msg']=$this->nettcp->msg;
		}else{
			$result['code']="0";
		}
    	return $result;
	}


	public function send( $cmd, $data){
		$result = $this->nettcp->probuffsend( $cmd, $data);	
		if( $result != false ){
			$recv_buff = $this->nettcp->recv();
			return $recv_buff;
		}else{
			return "";	
		}
		return 0;
	}

	public function close(){
		$this->nettcp->close();		
	}
};

?>
