<?php

function write_log($content)
{
	$path_file = "log/log_" . date("Ymd") .".txt";

	$fp = fopen($path_file,"a+");
	fwrite($fp,$content."\r\n");
	fclose($fp);
}
?>