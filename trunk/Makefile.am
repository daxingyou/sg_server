AUTOMAKE_OPTIONS = subdir-objects
SUBDIRS= . common network protocpp game_server center_server db_server gate_server log_server login_server chat_server gm_server robot_server account_server area_server transfer_server 

ignore : .svnignore
    find -type d |sed -e '/\.svn/d' | xargs svn propset svn:ignore --file .svnignore
