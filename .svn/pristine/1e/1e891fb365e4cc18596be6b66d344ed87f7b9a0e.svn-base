#!/usr/bin/python
# -*- coding: utf8 -*-

import os,re,sys,time
import config

oldFile = "old.sql"
ingore=[]

help = "i:install [game, log, account, area, glog, bbs, up, ss]\nu:update [game, log, account, area, glog, bbs, up, ss]\nr:reset [game, log, account, area, glog, bbs, up, ss]\nd:drop [game, log, account, area, glog, bbs, up, ss]"
log = ""

#----------------------------------------------
def dropMark( str ):
	str = re.sub("#.*\n","",str)
	str = re.sub("--.*\n","",str)
	str = re.sub("/\*.*\*/","",str)
	return str
#----------------------------------------------
def doCmd( str ):
	str = dropMark( str )
	str = str.replace("\n","")
	str = str.replace("\t","")
	str = str.replace("`","\`")
	splitcmd = str.split(";")
	for dbconf in config.server:
		for index in range(len(dbconf)):
			if(index < 3):continue
			for sql in splitcmd:
				if(sql=="" or sql.isspace()):continue
				cmd='mysql -u ' + dbconf[1] + ' -p' + dbconf[2] + ' -h ' + dbconf[0] + ' -e ' + '\"' + 'SET NAMES utf8; use ' + dbconf[index] + ';' + sql + ';' + '\"'
				os.system(cmd)

#----------------------------------------------
def doCmdByConf( dbconf,str ):
	str = dropMark( str )
	str = str.rstrip()
	str = str.replace("\n","")
	str = str.replace("\t","")
	str = str.replace("`","\`")
	splitcmd = str.split(";")
	for sql in splitcmd:
		if(sql=="" or sql.isspace()):continue
		cmd='mysql -u ' + dbconf[1] + ' -p' + dbconf[2] + ' -h ' + dbconf[0] + ' -e ' + '\"' + 'SET NAMES utf8; use ' + dbconf[3] + '; ' + sql + ';' + '\"'
		os.system(cmd)
		saveLog(cmd)
#----------------------------------------------
def updateDatabase(dbconf):
	compare(dbconf)
#----------------------------------------------
def updateAllDatabase():
	for dbconf in config.server:
		updateDatabase(dbconf)
#----------------------------------------------
def installDatabase(dbconf):
	sql = "CREATE DATABASE IF NOT EXISTS " + dbconf[3] + " DEFAULT CHARSET utf8 COLLATE utf8_general_ci"
	cmd='mysql -u ' + dbconf[1] + ' -p' + dbconf[2] + ' -h ' + dbconf[0] + ' -e ' + '\" SET NAMES utf8; ' + sql + ';' + '\"'
	code = os.system(cmd)
	if(code != 0):
		return
	saveLog(cmd)
	
	sql="source " + dbconf[4]
	doCmdByConf(dbconf,sql)
		
#----------------------------------------------
def installAllDatabase():
	for dbconf in config.server:
		installDatabase(dbconf)
#----------------------------------------------
def dropDB(dbconf):
	sql = "DROP DATABASE IF EXISTS " + dbconf[3]
	doCmdByConf(dbconf,sql)
		
def dropAll():
	for dbconf in config.server:
		dropDB(dbconf)
		if dbconf == config.gamedb:
			resetGameRedis()
			resetAccount()
#----------------------------------------------
def reset(dbconf):
	reset = open(dbconf[4],"r")
	text = ""
	for line in reset:
		if(line.find("CREATE TABLE") != -1):
			table = re.compile("`.*`").search(line).group(0)
			table = table.replace("`","")	
			if(table in ingore):
				continue
			text = "TRUNCATE TABLE " + table + ";"
			doCmdByConf( dbconf,text )
			text = ""

#----------------------------------------------
def resetGameRedis():
	cmd='redis-cli -h ' + config.game_redis[0] + ' -p ' + config.game_redis[1] + ' -a ' + config.game_redis[2] + ' -n ' +  config.game_redis[3] + ' flushdb'
	print cmd
	os.system(cmd)

def resetGlobalRedis():
	cmd='redis-cli -h ' + config.global_redis[0] + ' -p ' + config.global_redis[1] + ' -a ' + config.global_redis[2] + ' -n ' +  config.global_redis[3] + ' flushdb'
	print cmd
	os.system(cmd)
		
def resetAccount():
	sql = "DELETE FROM role WHERE server_id=" + config.accountdb[5]
	doCmdByConf(config.con_accountdb,sql)
	
def resetAll():
	#reset(config.gamedb)
	#reset(config.logdb)
	for dbconf in config.server:
		reset(dbconf)
		if dbconf == config.gamedb:
			resetGameRedis()
			resetAccount()
		elif dbconf == config.areadb:
			resetGlobalRedis()

#----------------------------------------------
def readSqlToDict(sqlfile,dc):
	file = open(sqlfile,"r")
	table = "";
	for line in file:
		if(line.find("CREATE TABLE") != -1):
			table = re.compile("`.*`").search(line).group(0)
			table = table.replace("`","")
			dc[table] = {}
			dc[table]["createsql"] = ""
		elif(line.strip().find("`") == 0):
			if(table != ""):
				col = re.compile("`.*`").search(line).group(0)
				col = col.replace("`","")
				dc[table][col] = line
		if(table != ""):
			dc[table]["createsql"] += line
			dc[table]["createsql"] += "\n"
		if(line.find(";") != -1):
			table = ""

def compare(dbconf):
	cmd='mysqldump --opt -d -h' + dbconf[0] + ' -u' + dbconf[1] + ' -p' + dbconf[2] + ' ' + dbconf[3] + '>' + oldFile
	#print cmd
	code = os.system(cmd)
	if(code != 0):
		print "Can't connect to MySQL config.server database " + dbconf[3] + " on " + dbconf[0]
		return

	tb1 = {}
	tb2 = {}
	readSqlToDict(dbconf[4], tb1)
	readSqlToDict(oldFile, tb2)

	for (k, v) in tb1.items():
		if(k not in tb2.keys()):
			print "table[" + k + "] not found "
			doCmdByConf( dbconf,v["createsql"] )
		else:
			for (k1, v1) in v.items():
				if(k1 == "createsql"):
					continue
				elif(k1 not in tb2[k].keys()):
					print "table[" + k + "] col[" + k1 + "] not found "
					altersql = "ALTER TABLE `" + k + "` ADD COLUMN " + tb1[k][k1]
					altersql = altersql.replace(",","")
					doCmdByConf( dbconf,altersql )
				else:
					index1 = tb1[k][k1].find("COMMENT")
					index2 = tb2[k][k1].find("COMMENT")
					col1 = tb1[k][k1][0:index1]
					col2 = tb2[k][k1][0:index2]
					
					col1 = col1.replace("\n","")
					col1 = col1.replace("\t","")
					col1 = col1.replace(",","")
					col1 = col1.strip()
					
					
					col2 = col2.replace("\n","")
					col2 = col2.replace("\t","")
					col2 = col2.replace(",","")
					col2 = col2.strip()
					if( col1 != col2):
						print "table[" + k + "] col[" + k1 + "] changed "
						altersql = "ALTER TABLE `" + k + "` MODIFY COLUMN " + col1
						doCmdByConf( dbconf,altersql )
	
	for (k, v) in tb2.items():
		if(k not in tb1.keys()):
			print "table " + k + " need drop"
			dropsql = "DROP TABLE `" + k + "`"
			doCmdByConf( dbconf,dropsql )
		else:
			for (k1, v1) in v.items():
				if(k1 == "createsql"):
					continue
				elif(k1 not in tb1[k].keys()):
					print "table " + k + " col " + k1 + " need drop"
					altersql = "ALTER TABLE `" + k + "` DROP COLUMN " + k1
					doCmdByConf( dbconf,altersql )

#----------------------------------------------
def saveLog(content):
	global log;
	if(log != ""):
		log += "\n"
	
	log += content
#----------------------------------------------
if __name__ == "__main__":
	if len( sys.argv ) <= 2:
		print help
		exit(0)
		
	for i in range(2, len( sys.argv )):
		if config.serverdb.has_key(sys.argv[i]):
			config.server.append(config.serverdb[sys.argv[i]])
		else:
			print 'has no db:', sys.argv[i]
			exit(0)

	if sys.argv[1] == "i":
		installAllDatabase()
	elif sys.argv[1] == "r":
		resetAll()
	elif sys.argv[1] == "u":
		updateAllDatabase()
	elif sys.argv[1] == "d":
		dropAll()
	else:
		print help

	if(log != ""):
		print log
		logfile = "logs/" + time.strftime("%Y%m%d%H%M%S",time.localtime()) + ".txt"
		loghandle = open(logfile, 'w+')
		loghandle.write(log)
		loghandle.close()
		log = ""
		print "log to:" + logfile
		print "Done!"
	else:
		print "Nothing to do!"
