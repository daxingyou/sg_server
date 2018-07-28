
AUTOMAKE_OPTIONS = subdir-objects
INCLUDES= -I/usr/local/include/zdb \
          -I$(top_srcdir)/common \
          -I$(top_srcdir)/network \
          -I$(top_srcdir)/protocpp \
          -I$(top_srcdir)/center_server

LDFLAGS  = -L/usr/local/lib

CXXFLAGS = -std=c++11 -Wno-error=unused-function -Wno-error=implicit-fallthrough -Wno-error=int-in-bool-context -Wno-error=format-security -Wno-error=unused-variable -Wno-error=bool-compare
if IS_BUILD_DEBUG
    CXXFLAGS += -D_DEBUG -O0
    CXXFLAGS += -ggdb -gdwarf-2
    CXXFLAGS += -Werror
    CXXFLAGS += -Wall -Wextra -Wunreachable-code -Wsign-compare -Wfloat-equal -Wcast-align -Winline -Wcast-qual -Wundef -Wredundant-decls
    CXXFLAGS += -Wno-reorder -Wno-unused-parameter -Wno-ignored-qualifiers
    CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
endif
if IS_BUILD_VALGRIND
    CXXFLAGS += -D_DEBUG -O0
    CXXFLAGS += -ggdb -gdwarf-2
    CXXFLAGS += -Werror
    CXXFLAGS += -Wall -Wextra -Wunreachable-code -Wsign-compare -Wfloat-equal -Wcast-align -Winline -Wcast-qual -Wundef -Wredundant-decls
    CXXFLAGS += -Wno-reorder -Wno-unused-parameter -Wno-ignored-qualifiers
endif
if IS_BUILD_GPROF
    CXXFLAGS += -D_DEBUG -O0
    CXXFLAGS += -ggdb -gdwarf-2
    CXXFLAGS += -Werror
    CXXFLAGS += -Wall -Wextra -Wunreachable-code -Wsign-compare -Wfloat-equal -Wcast-align -Winline -Wcast-qual -Wundef -Wredundant-decls
    CXXFLAGS += -Wno-reorder -Wno-unused-parameter -Wno-ignored-qualifiers
    CXXFLAGS += -pg
endif
if IS_BUILD_RELEASE
    CXXFLAGS += -DNDEBUG -O2
    CXXFLAGS += -ggdb -gdwarf-2
endif
if IS_BUILD_ONLINE                  
    CXXFLAGS += -DNDEBUG -O2
    CXXFLAGS += -s
endif

bin_PROGRAMS = center_server

center_server_LDADD = $(top_srcdir)/common/libcommon.a \
                      $(top_srcdir)/network/libnetwork.a \
                      $(top_srcdir)/protocpp/libproto.a \
                      -lredis \
                      -lzdb \
                      -lprotobuf \
                      -lboost_system \
                      -lboost_thread \
                      -lboost_locale \
					  -lboost_regex \
					  -lz \
					  -lstdc++

center_server_SOURCES = main/main.cpp \
                        main/center_server.cpp \
                        gate/gate_manager.cpp \
                        gate/gate_listener.cpp \
                        gate/gate_msg_handler.cpp \
                        game/game_manager.cpp \
                        game/game_listener.cpp \
                        game/game_msg_handler.cpp \
                        city/city.cpp \
                        city/city_manager.cpp \
                        city/city_msg_handler.cpp \
                        db/db_connector.cpp \
                        db/db_msg_handler.cpp \
                        gm/gm_listener.cpp \
                        gm/gm_msg_handler.cpp \
                        gm/gm_func.cpp \
                        user/global_user.cpp \
                        user/global_user_manager.cpp \
                        user/global_user_msg_handle.cpp \
                        log/log_connector.cpp \
                        log/log_wrapper.cpp \
                        log/log_unify_save.cpp \
                        redis/global_user_data_mgr.cpp\
                        redis/rank_data_mgr.cpp \
                        redis/friend_data_mgr.cpp \
                        redis/mail_data_mgr.cpp \
                        redis/challenge_data_mgr.cpp \
                        redis/shop_data_mgr.cpp \
                        redis/family_data_mgr.cpp \
                        redis/global_data_mgr.cpp \
                        chat/chat_listener.cpp \
                        chat/chat_msg_handler.cpp \
                        config/config_manager.cpp \
                        rank/rank_sort.cpp \
						rank/rank_level.cpp \
						rank/rank_tower.cpp \
						rank/rank_criminal.cpp \
						rank/rank_achieve.cpp \
						rank/rank_family.cpp \
						rank/rank_manager.cpp \
						rank/rank_charm.cpp \
						rank/rank_recruit.cpp \
						rank/rank_copper.cpp \
						rank/rank_gold.cpp \
						rank/rank_power.cpp \
                        redis/country_data_mgr.cpp \
                        country/country.cpp \
                        country/country_mgr.cpp \
                        country/country_msg_handle.cpp \
                        family/family.cpp \
                        family/family_build.cpp \
                        family/family_manager.cpp \
                        family/family_member.cpp \
                        family/family_msg_handle.cpp \
						$(top_srcdir)/tblh/RandomShopConfig.cpp \
						family/family_shop.cpp \
                        field_boss/field_boss.cpp \
                        field_boss/field_boss_manager.cpp \
                        field_boss/field_boss_msg_handle.cpp \
                        mail/mail_manager.cpp \
                        mail/mail_msg_handle.cpp \
                        mail/mail_user_data.cpp \
                        mail/global_mail_manager.cpp \
                        friend/friend_mgr.cpp \
                        friend/friend_manager.cpp \
                        item/item_limit.cpp \
                        item/item_manager.cpp \
                        friend/friend_msg_handle.cpp \
                        item/item_msg_handler.cpp \
                        transfer/transfer_connector.cpp \
                        transfer/transfer_manager.cpp \
                        transfer/transfer_msg_handler.cpp \
                        offline_friend_msg_mgr/offline_friend_msg_mgr.cpp \
                        sys_notice/sys_notice_manager.cpp \
                        challenge/challenge_msg_handle.cpp \
                        challenge/challenge_manager.cpp \
                        king_war/king_war_manager.cpp \
                        king_war/king_war_msg_handle.cpp \
                        shop/shop_manager.cpp \
                        shop/shop_msg_handle.cpp \
                        shop/shop_user_data.cpp \
                        scene/global_scene_manager.cpp \
                        scene/global_scene.cpp \
                        scene/global_dungeon.cpp \
                        scene/family_dungeon.cpp \
                        scene/global_scene_msg_handle.cpp \
                        world_cup/world_cup_battle.cpp \
                        world_cup/world_cup_manager.cpp \
                        world_cup/world_cup_msg_handle.cpp \
                        user_troop/user_troop.cpp \
                        user_troop/user_troop_msg_handler.cpp \
                        user_troop/user_troop_mgr.cpp \
                        goods/goods.cpp \
                        goods/goods_msg_handle.cpp \
                        goods/goods_manager.cpp \
						general_event/general_info_manager.cpp \
						general_event/general_event_manager.cpp \
						general_event/general_event_msg_handler.cpp \
						offline_arena/offline_arena.cpp \
                        offline_arena/offline_arena_msg_handle.cpp \
                        offline_arena/offline_arena_manager.cpp \
						offline_arena/offline_arena_record.cpp \
						redbag/redbag.cpp \
                        redbag/redbag_msg_handle.cpp \
                        redbag/redbag_manager.cpp \
                        offline_fight/offline_fight_msg_handle.cpp \
                        offline_fight/offline_fight_manager.cpp \
						offline/offline_role.cpp \
                        offline/offline_role_manager.cpp \
                        red_point/red_point_manager.cpp \
                        achieve/achieve_common.cpp \
						sys_notice/sys_notice_msg_handle.cpp \
						sys_notice/sys_notice.cpp \
						look_role_info/simple_data_manager.cpp \
						look_role_info/simple_data_msg_handle.cpp \
						luckydraw/luckydraw_mgr.cpp \
						luckydraw/luckydraw_msg_handle.cpp \
						cdkey/cdkey_msg_handler.cpp \
						cdkey/cdkey_manager.cpp 

            
center_serverdir=$(prefix)/conf/
