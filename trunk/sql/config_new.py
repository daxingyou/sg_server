#!/usr/bin/python
#        ["ip","user","password","dbname","sqlfile","server_id"]
con_accountdb = ["127.0.0.1","root","Sanguo1!","sg_account","sg_account.sql","5"]
con_areadb = ["127.0.0.1","root","Sanguo1!","sg_area","sg_area.sql","5"]
gamedb = ["127.0.0.1","root","Sanguo1!","sg_game","sg_game.sql"]
logdb = ["127.0.0.1","root","Sanguo1!","sg_log","sg_log.sql"]
accountdb = ["127.0.0.1","root","Sanguo1!","sg_account","sg_account.sql","5"]
areadb = ["127.0.0.1","root","Sanguo1!","sg_area","sg_area.sql"]
glogdb = ["127.0.0.1","root","Sanguo1!","sg_glog","sg_glog.sql"]
bbsdb = ["127.0.0.1","root","Sanguo1!","sg_bbs","sg_bbs.sql"]
updb = ["127.0.0.1","root","Sanguo1!","sg_up","sg_up.sql"]
ssdb = ["127.0.0.1","root","Sanguo1!","sg_statistics","sg_statistics.sql"]
gmdb = ["127.0.0.1","root","Sanguo1!","sg_gm","sg_gm.sql"]
videodb = ["127.0.0.1","root","Sanguo1!","sg_video","sg_video.sql"]

server = []
serverdb = {'game':gamedb, 'log':logdb, 'account':accountdb, 'area':areadb, 'glog':glogdb, 'bbs':bbsdb, 'up':updb, 'ss':ssdb, 'gm':gmdb, 'video':videodb}

#       ["ip","port","password","db"]
global_redis = ["127.0.0.1","6379","Sanguo1!","0"]    
game_redis = ["127.0.0.1","6379","Sanguo1!","1"]


