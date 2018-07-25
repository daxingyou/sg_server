#!/usr/bin/python
# -*- coding: utf-8 -*-
# centos 6.5
import os,re,sys,time
import signal
import datetime
import csv

#中断
IS_SIGNAL_UP = False

#要监视的进程
#SERVER = ['game_server']
SERVER = ['game_server', 'center_server', 'db_server', 'gate_server', 'login_server', 'log_server', 'chat_server']
#SERVER = ['account_server', 'area_server', 'transfer_server', 'cross_server']
#SERVER = ['mysql', 'redis']
#要监视的进程信息（内存、CPU）
SERVER_INFO = {}

#采样间隔
INTEVEL_TIME = 3
#网卡
NIC = 'eth0'

#采样Shell
SHELL_COM1 = 'ps -e | grep {0}'	#
SHELL_COM2 = 'top -bn 2 -d 1 -p' # 

_RED_ = '\033[1;31;40m'
_NORMAL_ = '\033[0m'

#换算
KB = 1024
MB = 1024*1024

#一些全局变量
PREV_NET_INFO = {"rev":0,"send":0}

#系统网络、内存、CPU统计
GLOBAL_COUNT = 0
PER_TIME = 0
#整体信息
INFO_DATA = []

#--------------------------------------NET---------------------------------
def get_net_stat(eth):
	with open('/proc/net/dev') as netfp:
		data = netfp.read()
		#print data
		patten=eth + '.*'
		netin = 0
		netout = 0
		if not re.search(patten,data):
			print "The ETHname not have"
		else:
			netin = int(re.search(patten,data).group().replace(':',' ').split()[1])
			netout = int(re.search(patten,data).group().replace(':',' ').split()[9])
	#print netin, netout
	return netin, netout
	
def net_info(eth, intevel):
	global PREV_NET_INFO
	#print intevel
	NET_STATS = []
	net_stat = get_net_stat(eth)
	
	if PREV_NET_INFO['rev'] == 0 and PREV_NET_INFO['send'] == 0:
		PREV_NET_INFO['rev'] = net_stat[0]
		PREV_NET_INFO['send'] = net_stat[1]
		
	NET_STATS.append(float(net_stat[0] - PREV_NET_INFO['rev']) / KB / intevel )
	NET_STATS.append(float(net_stat[1] - PREV_NET_INFO['send']) / KB / intevel )
	
	PREV_NET_INFO['rev'] = net_stat[0]
	PREV_NET_INFO['send'] = net_stat[1]
	
	print 'NET:', '%.2fKbps receive | %.2fKbps transmit' % (NET_STATS[0], NET_STATS[1])
	return '%.2f'%(NET_STATS[0]), '%.2f'%(NET_STATS[1])

#--------------------------------------------------------------------------

#----------------------------MEM-------------------------------------------
def mem_stat():
	meminfo = {}
	with open('/proc/meminfo') as f:
		for line in f:
			#print line
			meminfo[line.split(':')[0]] = int(line.split(':')[1].split()[0])
	meminfo['MemUsed'] = meminfo['MemTotal'] - meminfo['MemFree'] - meminfo['Buffers']
	pre = round(float(meminfo['MemUsed'])/float(meminfo['MemTotal']),5)
	print 'MEM:', '%dkb used'%(meminfo['MemUsed']),'| PER:%.2f%%'%(pre*100)
	return pre*100
#--------------------------------------------------------------------------
#--------------------------------------------------------------------------
#获得PID
def Init():
	for i in SERVER:
		shell = SHELL_COM1.format(i)
		line = os.popen(shell).read().split();
		print line
		if len(line) > 0:
			SERVER_INFO[int(line[0])] = {}
			SERVER_INFO[int(line[0])]['NAME'] = i
			SERVER_INFO[int(line[0])]['CPU'] = []
			SERVER_INFO[int(line[0])]['RES'] = []
			SERVER_INFO[int(line[0])]['MEM'] = []
			SERVER_INFO[int(line[0])]['TIME'] = []
			
#采样
def Sampling():
	keys = SERVER_INFO.keys()
	shell_comd = SHELL_COM2
	for i in range(0, len(keys)):
		if i != 0:
			shell_comd += ','
		str = ' %d'%(keys[i])
		shell_comd += str
	#print shell_comd
	#因为os.popen(shell_comd)执行需要时间，所以有时在中断时会捕获IO异常
	#所以用try..except 保证能够dump到结果
	try:
		print shell_comd
		top = os.popen(shell_comd).read().split('\n\n\n')
		if len(top) > 1:
			return top[1]
		else:
			return []
	except:
		print 'os.popen error'
		IS_SIGNAL_UP = True
	
	return []

#统计分析	
def Analysis():
	global GLOBAL_COUNT
	global PER_TIME
	GLOBAL_COUNT += 1
	
	top = Sampling()
	if len(top) == 0:
		return False
	
	txt = top.split('\n\n')
	print txt[0]
	now = datetime.datetime.now().strftime('%Y%m%d-%H:%M:%S')
	timenow = int(time.time()) 
	
	cpu = re.split(':|%|,',txt[0].split('\n')[2])
	info = [GLOBAL_COUNT, cpu[7]]
	
	net_stat = net_info(NIC, timenow - PER_TIME)
	info.append(net_stat[0])
	info.append(net_stat[1])
	PER_TIME = timenow
	
	mem = mem_stat()
	info.append(mem)
	
	info.append(now)
	INFO_DATA.append(info)
	
	print txt[1]
	lines = txt[1].split('\n')
	for i in range(1, len(lines)):
		line = lines[i].split()
		res = 0
		if line[5].find('m') != -1:
			res = float(line[5].split('m')[0])
		else:
			res = float(line[5])/KB
			
		SERVER_INFO[int(line[0])]['RES'].append(res)
		SERVER_INFO[int(line[0])]['CPU'].append(line[8])
		SERVER_INFO[int(line[0])]['MEM'].append(line[9])
		SERVER_INFO[int(line[0])]['TIME'].append(now)

	print '-------------------------------------------------------------------------------------'
	return True
	
#输出csv
def Output_CSV():
	localtime = time.strftime('%Y%m%d%H%M%S',time.localtime(time.time()))
	log_dir = os.path.join('./pm_log/', localtime)
	if not os.path.exists(log_dir):
		os.makedirs(log_dir) 
	with open(os.path.join(log_dir, 'info.csv'), 'wb') as csvfile:
		spamwriter = csv.writer(csvfile,dialect='excel')
		spamwriter.writerow(['编号'.decode('utf-8').encode('gbk'),'CPU空闲%'.decode('utf-8').encode('gbk'),'物理内存使用%'.decode('utf-8').encode('gbk'),'接收(KB)'.decode('utf-8').encode('gbk'),'发送(KB)'.decode('utf-8').encode('gbk'),'时间'.decode('utf-8').encode('gbk')])
		for line in INFO_DATA:
			spamwriter.writerow([line[0],line[1],line[4],line[2],line[3],line[5]])
	
	for key,info in SERVER_INFO.items():
		with open(os.path.join(log_dir, '%s.csv'%(info['NAME'])), 'wb') as csvfile:
			spamwriter = csv.writer(csvfile,dialect='excel')
			spamwriter.writerow(['编号'.decode('utf-8').encode('gbk'),'RES(Mb)','CPU%','MEM%','时间'.decode('utf-8').encode('gbk')])
			for i in range(0, len(info['RES'])):
				spamwriter.writerow([i+1, info['RES'][i], info['CPU'][i], info['MEM'][i], info['TIME'][i]])
#--------------------------------------------------------------------------
#--------------------------------------------------------------------------
def sigint_handler(signum, frame):
	global IS_SIGNAL_UP
	IS_SIGNAL_UP = True
	print 'catched interrupt signal!'
	
def main():
	signal.signal(signal.SIGINT, sigint_handler)
	signal.signal(signal.SIGHUP, sigint_handler)
	signal.signal(signal.SIGTERM, sigint_handler)
	Init()
	#print SERVER_INFO
	print SERVER_INFO.keys()
	while True:
		if Analysis() == False:
			break
		
		if IS_SIGNAL_UP:
			print 'Exit!'
			break
		time.sleep(INTEVEL_TIME)
	
	if IS_SIGNAL_UP:
		Output_CSV()

if __name__ == '__main__':
	main()