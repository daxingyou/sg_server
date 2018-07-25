SUBDIRS= . common network protocpp db_save #game_server center_server db_server gate_server log_server login_server chat_server account_server area_server transfer_server robot_server gm_server db_save

ignore : .svnignore
    find -type d |sed -e '/\.svn/d' | xargs svn propset svn:ignore --file .svnignore
