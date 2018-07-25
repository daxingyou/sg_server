#!/bin/sh

#crontab -e
#0    5    *    *    *    /var/www/html/sg_game_combat_logs/delete_combat_logs.sh

cd /var/www/html/sg_game_combat_logs/
find ./ -mtime +1 -name '*.fight' -exec rm -f {} 