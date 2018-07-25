<?php

class SSH2Obj
{
    private $m_ip;

    private $m_port;

    private $m_user;

    private $m_passwd;

    private $m_conn;

    public function __construct($ip, $port, $user, $passwd){
        $this->m_ip = $ip;
        $this->m_port = $port;
        $this->m_user = $user;
        $this->m_passwd = $passwd;
        $conn = ssh2_connect($ip,$port);
        if($conn == false){
            throw new Exception("Can't connect to {$ip}:{$port}");
        }
        if(ssh2_auth_password($conn, $user, $passwd) == false){
            throw new Exception("Wrong user:{$user} or password");
        }
        $this->m_conn = $conn;
    }

    public function sync_operation($cmd){
        $stream = ssh2_exec($this->m_conn, $cmd);
        stream_set_blocking($stream, true);
        return stream_get_contents($stream);
    }
}

?>