<?php
require_once('./lib/tp_netclient.php');
require_once('./lib/probuff/message/pb_message.php');	
require_once('./lib/log_debug.php');

class tcp_client {
	public $nettcp;

	public function connect( $ip , $port ){
		$this->nettcp = new TP_NetClient_TCP;

		$result=array();
		if($this->nettcp->connect($ip,$port,60) == false) {
			return -1;
		}
		return 0;
	}


	public function send( $cmd, $data){
		$result = $this->nettcp->probuffsend( $cmd, $data);	
		if( $result == false ){
			return -1;	
		}
		log_debug("receiving");
		$recv_buff = $this->nettcp->recv();
		log_debug("receivingend");
		return $recv_buff;
	}

	public function close(){
		$this->nettcp->close();		
	}
};

?>
