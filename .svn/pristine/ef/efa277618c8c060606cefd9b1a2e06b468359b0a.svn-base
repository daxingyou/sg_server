<?php
/**
 * Example:
 * $client = new TP_NetClient_TCP; //TP_NetClient_TCP or TP_NetClient_UDP
 * if($client->connect('127.0.0.1', 80, 0.5)) //Host,Port,Timeout
 * {
 *     $client->send("GET / HTTP/1.1\r\n\r\n");
 *     echo $client->recv();
 * }
 * else
 * {
 *     echo $client->code;
 *     echo $client->msg;
 * }
 */

/**
 * 网络客户端封装基类
 * @author tianfenghan
 * @package TPLib.Net
 * @subpackage Client
 */
abstract class TP_NetClient
{
	protected $sock;
	protected $host;     //Server Host
	protected $port;     //Server Port

	protected $timeout_send;
	protected $timeout_recv;
	public $sendbuf_size = 65535;
	public $recvbuf_size = 65535;

	public $code = 0;
	public $msg = '';

	/**
	 * 错误信息赋值
	 */
	protected function set_error()
	{
		$this->code = socket_last_error($this->sock);
		$this->msg = socket_strerror($this->code);
		socket_clear_error($this->sock);
	}
	/**
	 * 设置超时
	 * @param float $recv_timeout 接收超时
	 * @param float $send_timeout 发送超时
	 */
	function set_timeout($timeout_recv,$timeout_send)
	{
		$_timeout_recv_sec = (int)$timeout_recv;
		$_timeout_send_sec = (int)$timeout_send;

		$this->timeout_recv = $timeout_recv;
		$this->timeout_send = $timeout_send;

		$_timeout_recv = array('sec'=>$_timeout_recv_sec,'usec'=>(int)(($timeout_recv - $_timeout_recv_sec)*1000*1000));
		$_timeout_send = array('sec'=>$_timeout_send_sec,'usec'=>(int)(($timeout_send - $_timeout_send_sec)*1000*1000));

		$this->setopt(SO_RCVTIMEO, $_timeout_recv);
		$this->setopt(SO_SNDTIMEO, $_timeout_send);
	}
	/**
	 * 设置socket参数
	 */
	function setopt($opt,$set)
	{
		socket_set_option($this->sock, SOL_SOCKET, $opt, $set);
	}
	/**
	 * 获取socket参数
	 */
	function getopt($opt)
	{
		return socket_get_option($this->sock, SOL_SOCKET, $opt);
	}

	function get_socket()
	{
		return $this->sock;
	}

	function set_bufsize($sendbuf_size,$recvbuf_size)
	{
		$this->setopt(SO_SNDBUF, $sendbuf_size);
		$this->setopt(SO_RCVBUF, $recvbuf_size);
	}
	/**
	 * 析构函数
	 */
	function __destruct()
	{
		$this->close();
	}
}

/**
 * UDP客户端
 * @author tianfenghan
 */
class TP_NetClient_UDP extends TP_NetClient
{
	public $remote_host;
	public $remote_port;

	/**
	 * 连接到服务器
	 * 接受一个浮点型数字作为超时，整数部分作为sec，小数部分*100万作为usec
	 *
	 * @param string $host 服务器地址
	 * @param int    $port 服务器地址
	 * @param float  $timeout 超时默认值，连接，发送，接收都使用此设置
	 * @param bool   $udp_connect 是否启用connect方式
	 */
	function connect($host,$port,$timeout = 0.1,$udp_connect = true)
	{
		//判断超时为0或负数
		if(empty($host) or empty($port) or $timeout <=0)
		{
			$this->code = -10001;
			$this->msg = "param error";
			return false;
		}
		$this->host = $host;
		$this->port = $port;
		$this->sock = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);
		$this->set_timeout($timeout,$timeout);
		$this->set_bufsize($this->sendbuf_size, $this->recvbuf_size);

		//是否用UDP Connect
		if($udp_connect !==true)
		{
			return true;
		}
		if(socket_connect($this->sock, $host, $port))
		{
			//清理connect前的buffer数据遗留
			while(@socket_recv($this->sock, $buf, 65535 , MSG_DONTWAIT));
			return true;
		}
		else
		{
			$this->set_error();
			return false;
		}
	}
	/**
	 * 发送数据
	 * @param string $data
	 * @return $n or false
	 */
	function send($data)
	{
		$len = strlen($data);
		$n = socket_sendto($this->sock, $data, $len , 0, $this->host, $this->port);

		if($n === false or $n < $len)
		{
			$this->set_error();
			return false;
		}
		else
		{
			return $n;
		}
	}
	
	/**
	 * 接收数据，UD包不能分2次读，recv后会清除数据包，所以必须要一次性读完
	 *
	 * @param int $length 接收数据的长度
	 * @param bool $waitall 等待接收到全部数据后再返回，注意waitall=true,超过包长度会阻塞住
	 */
	function recv($length = 65535,$waitall = 0)
	{
		if($waitall) $waitall = MSG_WAITALL;
		while(true)
		{
			$ret = socket_recvfrom($this->sock, $data, $length, $waitall, $this->remote_host, $this->remote_port);
			if($ret === false)
			{
				$this->set_error();
				//EINT，中断
				if($this->code == 4)
				{
					continue;
				}
				//出错了，超时或者其他
				else
				{
					return false;
				}
			}
			//正常
			else
			{
				return $data;
			}
		}
	}
	/**
	 * 关闭socket连接
	 */
	function close()
	{
		if($this->sock) socket_close($this->sock);
		$this->sock = null;
	}
}

/**
 * TCP客户端
 * @author tianfenghan
 */
class TP_NetClient_TCP extends TP_NetClient
{
	// 主机序转换
	function net_ntohl($str)
	{
		$net_pack = unpack('NLEN', pack('N', $str));
		return $net_pack['LEN'];
	}
	function net_htonl($str)
	{
		$net_pack = pack('V', $str);
		return $net_pack;
	}
	
	function net_htonc($str)
	{
		$net_pack = pack('C', $str);
		return $net_pack;
	}
	
	function net_htonn($str)
	{
		$net_pack = pack('v', $str);
		return $net_pack;
	}
	
	/**
	 * 发送数据
	 * @param string $data
	 */
	function send($data)
	{
		$length = strlen($data);
		$written = 0;
		$t1 = microtime(true);
		//总超时，for循环中计时
		while($written < $length)
		{
			$n = socket_write($this->sock, substr($data, $written));
			//超过总时间
			if(microtime(true) > $this->timeout_send + $t1)
			{
				echo "time out microtime:" . microtime(true) . " timeout_send:" . $this->timeout_send . " t1:" . $t1;
				return false;
			}
			if($n===false) //反过来
			{
				$errno = socket_last_error($this->sock);
				//判断错误信息，EAGAIN EINTR，重写一次
				if($errno == 11 or $errno==4)
				{
					echo "timeout tcp send error:".$errno;
					return false;
				}
				else
				{
					echo "tcp send error:".$errno;
					return false;
				}
			}
			$written += $n;
		}
		return $written;
	}
	// 带probuff的send
	function probuffsend($msg_cmd,$g_admin)
	{
		#$g_sz_admin = $g_admin->SerializeToString();
		
		$g_admin_len = strlen($g_admin);
		$msglen = 4+2+2+2+2+$g_admin_len;

		$g_admin_len_ntohl=$this->net_htonl($g_admin_len);
		$g_msg_cmd_ntohl=$this->net_htonn($msg_cmd);
		$g_msg_cmd_ex_ntohl=$this->net_htonn(0);
		$g_flags_ntohl = $this->net_htonn(0);
		$g_unused_ntohl = $this->net_htonn(0);

		$g_sz_total = $g_admin_len_ntohl . $g_msg_cmd_ntohl . $g_msg_cmd_ex_ntohl . $g_flags_ntohl . $g_unused_ntohl. $g_admin;
		
		return $this->send($g_sz_total, $msglen);
	}
	/**
	 * 接收数据
	 * @param int $length 接收数据的长度
	 * @param bool $waitall 等待接收到全部数据后再返回，注意这里超过包长度会阻塞住
	 */
	function recv($length = 65535 ,$waitall = 0)
	{
		if($waitall) $waitall = MSG_WAITALL;

		while(true)
		{
			$ret = socket_recv($this->sock, $data, $length, $waitall);
			if($ret === false)
			{
				$this->set_error();
				//EINT，被中断
				if($this->code==4)
				{
					continue;
				}
				//出错了
				else
				{
					return false;
				}
			}
			//正常
			else
			{
				//$final_data = substr($data,12);
				return $data;
			}
		}
	}
	/**
	 * 连接到服务器
	 * 接受一个浮点型数字作为超时，整数部分作为sec，小数部分*100万作为usec
	 *
	 * @param string $host 服务器地址
	 * @param int    $port 服务器地址
	 * @param float  $timeout 超时默认值，连接，发送，接收都使用此设置
	 */
	function connect($host,$port,$timeout = 0.1)
	{
		//判断超时为0或负数
		if(empty($host) or empty($port) or $timeout <=0)
		{
			$this->code = -10001;
			$this->msg = "param error";
			return false;
		}
		$this->host = $host;
		$this->port = $port;
		//创建socket
		$this->sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
		if($this->sock === false)
		{
			$this->set_error();
			return false;
		}
		//设置connect超时
		$this->set_timeout($timeout,$timeout);
		//连接服务器
		if(socket_connect($this->sock, $this->host, $this->port))
		{
			$this->setopt(SO_REUSEADDR, 1); //重用
			return true;
		}
		else
		{
			$this->set_error();
			return false;
		}
	}
	/**
	 * 关闭socket连接
	 */
	function close()
	{
		if($this->sock) socket_close($this->sock);
		$this->sock = null;
	}
}
