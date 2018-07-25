<?php

function log_debug($content){
	return file_put_contents($_SERVER['DOCUMENT_ROOT'] . '/sg_gmtool/tmp/log.tmp', $content . PHP_EOL, FILE_APPEND);
}

?>