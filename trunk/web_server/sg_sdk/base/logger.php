<?php

function log_debug($content, $filename){
	return file_put_contents($GLOBALS["log_path"] . '/' . $filename, $content . PHP_EOL, FILE_APPEND);
}

class OutPutLogger
{
	private $m_filename;
	private $m_recording;
	public function __construct($filename){
		$this->m_filename = $filename;
		$this->m_recording = false;
	}
	public function record(){
		if($this->m_recording === true)
			return false;
		$this->m_recording = true;
		return ob_start();
	}
	public function gen_log(){
		if($this->m_recording === false)
			return false;
		$str = ob_get_contents();
		log_debug($str, $this->m_filename);
		$this->m_recording = false;
		return ob_end_clean();
	}
}

?>