<?php

function write_log($content)
{
	$path_file = "log/log_" . date("Ymd") .".txt";

	$fp = fopen($path_file,"a+");
	$f_content = date("Y-m-d H:i:s",time()) . " " . $content;
	fwrite($fp,$f_content."\r\n");
	fclose($fp);
}
?>