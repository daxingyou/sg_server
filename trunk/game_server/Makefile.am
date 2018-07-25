
AUTOMAKE_OPTIONS = subdir-objects
INCLUDES =  -I/usr/local/include/lua \
            -I/usr/local/include/tolua++ \
            -I$(top_srcdir)/common \
            -I$(top_srcdir)/network \
            -I$(top_srcdir)/protocpp \
            -I$(top_srcdir)/tblh \
            -I$(top_srcdir)/game_server 
             
LDFLAGS =   -L/usr/local/lib -ldl

CXXFLAGS = -std=c++11
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

#RM := rm -rf
#all:clean_file

bin_PROGRAMS = game_server

game_server_LDADD =    $(top_srcdir)/common/libcommon.a \
                        $(top_srcdir)/network/libnetwork.a \
                        $(top_srcdir)/protocpp/libproto.a \
                        -llua \
                        -ltolua++ \
                        -lluabit \
                        -lprotobuf \
                        -lboost_system  \
                        -lboost_thread \
						-lboost_regex \
                        -lboost_locale
                        
                        
game_server_SOURCES =   $(top_srcdir)/tblh/DropTableConfig.cpp \
						$(top_srcdir)/tblh/RandomShopConfig.cpp \
                        hero/hero_manager.cpp \
                        hero/hero.cpp \
                        hero/talent.cpp \
                        hero/cultivate_manager.cpp \
                        center/center_msg_handler.cpp \
                        center/center_connector.cpp \
                        gate/gate_msg_handler.cpp \
                        gate/gate_listener.cpp \
                        gate/gate_connector.cpp \
                        gate/gate_manager.cpp \
                        log/log_wrapper.cpp \
                        log/log_connector.cpp \
                        log/log_unify_save.cpp \
                        main/game_server.cpp \
                        main/main.cpp \
                        db/db_msg_handler.cpp \
                        db/db_connector.cpp \
                        gm_cmd/gm_cmd.cpp \
                        gm_cmd/gm_cmd_opt.cpp \
                        item/item_manager.cpp \
                        item/item.cpp \
                        item/item_msg_handle.cpp \
                        task/task_msg_handle.cpp \
                        task/task.cpp \
                        task/task_manager.cpp \
                        task/bounty.cpp \
                        task/bounty_manager.cpp \
                        country/country_msg_handle.cpp \
						country/country_manager.cpp \
                        family/family_msg_handle.cpp \
                        family/family_manager.cpp \
                        family/family_war_scene.cpp \
                        family/family_war_long.cpp \
                        role/role.cpp \
                        role/role_scene_info.cpp \
                        role/role_manager.cpp \
                        role/role_unify_save.cpp \
                        role/role_msg_handle.cpp \
                        role/money.cpp \
                        role/money_manager.cpp \
                        object/object_attr.cpp \
                        object/object_manager.cpp \
                        object/object_position.cpp \
                        monster/monster_manager.cpp \
                        monster/monster.cpp \
                        monster/monster_ai.cpp \
                        mail/mail_manager.cpp \
                        mail/mail_msg_handle.cpp \
                        field_boss/field_boss.cpp \
                        field_boss/field_boss_manager.cpp \
                        field_boss/field_boss_msg_handle.cpp \
                        scene/map.cpp \
                        scene/map_area.cpp \
						scene/map_patrol.cpp \
                        scene/scene_manager.cpp \
                        scene/scene.cpp \
                        scene/scene_msg_handle.cpp \
						scene/scene_state.cpp \
                        chat/chat_msg_handler.cpp \
                        chat/chat_connector.cpp \
						chat/chat_msg_manager.cpp \
                        formation/formation.cpp \
                        formation/formation_msg_handle.cpp \
                        formation/formation_manager.cpp \
                        formation/tactic.cpp \
                        formation/tactic_manager.cpp \
                        city/city.cpp \
                        city/city_manager.cpp  \
                        city/city_msg_handler.cpp  \
                        trade/trade.cpp \
						trade/role_trade.cpp \
						trade/role_trade_manager.cpp \
						trade/trade_item.cpp \
                        trade/trade_manager.cpp \
                        trade/trade_msg_handler.cpp \
                        script/ToScript.cpp \
                        script/script_role.cpp \
                        script/game_script_func.cpp \
                        script/script_fight_hero.cpp \
                        script/script_dungeon.cpp \
                        user_troop/user_troop.cpp \
                        user_troop/user_troop_msg_handler.cpp \
                        user_troop/user_troop_mgr.cpp \
                        fight/fight_manager.cpp \
                        fight/fight_msg_handle.cpp \
                        fight/buff.cpp \
                        fight/buff_element.cpp \
                        fight/buff_element_manager.cpp \
                        fight/buff_manager.cpp \
                        fight/combat.cpp \
                        fight/fight_hero.cpp \
                        fight/fight_obj.cpp \
                        fight/role_combat_result.cpp \
                        fight/combat_log.cpp 	\
						fight/skill_data.cpp	\
						fight/skill_manager.cpp	\
                        item/package.cpp \
                        item/package_manager.cpp \
                        config/config_manager.cpp \
						rank/rank_manager.cpp \
                        mount/mount.cpp \
                        mount/mount_manager.cpp \
                        mount/mount_msg_handler.cpp \
                        item/drop_manager.cpp \
                        item/item_limit.cpp \
                        fight_study/fight_study_manager.cpp \
                        fight_study/fight_study_msg_handle.cpp \
                        collect/collect_point.cpp \
						collect/collect_platform.cpp \
                        collect/collect_manager.cpp \
                        collect/collect_msg_handler.cpp \
                        activity/activity.cpp \
                        activity/activity_manager.cpp \
                        activity/activity_msg_handle.cpp \
						activity/holiday_manager.cpp \
                        area/area_connector.cpp \
                        area/area_msg_handler.cpp \
                        arena/arena.cpp \
                        arena/arena_manager.cpp \
                        arena/arena_msg_handler.cpp \
                        treasure/treasure_msg_handle.cpp \
                        treasure/treasure_manager.cpp \
                        sys_notice/sys_notice_manager.cpp \
                        pk/pk_manager.cpp \
                        pk/pk_msg_handle.cpp \
                        king_war/king_war_manager.cpp \
                        king_war/king_war_msg_handle.cpp \
                        king_war/king_war_scene.cpp \
                        king_war/long_pulse.cpp \
                        shop/shop_manager.cpp \
                        shop/shop_msg_handle.cpp \
						shop/role_random_shop.cpp \
                        equip/equip_manager.cpp \
                        equip/equip_msg_handle.cpp \
						equip/equip_plan.cpp \
						equip/equip_plan_manager.cpp \
                        achieve/achieve.cpp \
                        achieve/achieve_manager.cpp \
                        achieve/achieve_method.cpp \
                        achieve/achieve_msg_handle.cpp \
                        tower/tower.cpp \
						tower/tower_trigger.cpp \
                        tower/tower_manager.cpp \
                        tower/tower_msg_handle.cpp \
						expedition/expedition.cpp \
                        expedition/expedition_msg_handle.cpp \
						expedition/mirror_fight_data.cpp \
						expedition/hero_save_data.cpp \
						recharge/recharge.cpp \
                        recharge/recharge_msg_handle.cpp \
						office/office.cpp \
                        office/office_msg_handle.cpp \
                        luckydraw/luckydraw.cpp \
                        luckydraw/luckydraw_manager.cpp \
                        luckydraw/luckydraw_msg_handle.cpp \
                        fsmai/fsm_xml_manager.cpp \
                        fsmai/ai_action.cpp \
                        fsmai/ai_event.cpp \
                        fsmai/ai_mode.cpp \
                        fsmai/ai_state.cpp \
                        dungeon/dungeon_scene.cpp \
                        dungeon/dungeon_msg_handle.cpp \
                        dungeon/dungeon_manager.cpp \
                        dungeon/role_dungeon_data.cpp \
                        dungeon/dungeon_state.cpp \
                        red_point/red_point.cpp \
                        red_point/red_point_manager.cpp \
                        red_point/red_point_msg_handle.cpp \
                        role_buff/role_buff.cpp \
                        role_buff/role_buff_manager.cpp \
                        virtual_notice/name_manager.cpp \
                        virtual_notice/virtual_notice_manager.cpp \
                        lifeforce/lifeforce_manager.cpp \
                        lifeforce/lifeforce_attr_manager.cpp \
                        lifeforce/lifeforce_msg_handle.cpp	\
						liandan/liandan.cpp \
                        liandan/liandan_manager.cpp \
                        liandan/liandan_msg_handle.cpp	\
						family_skill/family_skill.cpp \
                        family_skill/family_skill_manager.cpp \
                        family_skill/family_skill_msg_handle.cpp	\
						page/page_manager.cpp	\
						page/page_msg_handle.cpp \
						level_reward/role_level_reward.cpp \
						achieve/achieve_common.cpp \
						achieve/active_reward.cpp \
						achieve/active_reward_manager.cpp \
						sys_notice/sys_notice_msg_handle.cpp \
						sys_notice/sys_notice.cpp \
						multi_time/multi_time_manager.cpp \
						multi_time/multi_time_msg_handler.cpp\
						level_reward/role_level_reward_mail.cpp	\
						business_activity/business_activity_manager.cpp \
						business_activity/business_activity_msg_handler.cpp \
						business_activity/shake_money_tree.cpp \
						business_activity/sevenday_login.cpp \
						business_activity/month_card.cpp \
						business_activity/level_fund.cpp \
						business_activity/exchange_item.cpp \
						business_activity/limit_buy.cpp \
						business_activity/sevenday_target.cpp \
						business_activity/recharge_total.cpp \
						business_activity/activity_shop.cpp \
						business_activity/gold_luckydraw.cpp \
						business_activity/time_rank.cpp \
						partner/partner.cpp \
						partner/partner_tutor.cpp \
						partner/partner_shadow.cpp \
						partner/partner_manager.cpp \
						patrol_hire/patrol_hire.cpp \
						patrol_hire/patrol_hire_manager.cpp \
						patrol_hire/patrol_hire_msg_handle.cpp  \
						general_event/general_event_manager.cpp \
						general_event/general_info_manager.cpp 	\
						general_event/general_event_msg_handler.cpp	\
						friend/friend_manager.cpp \
						friend/friend_msg_handler.cpp \
                        friend/friendliness.cpp \
						video_client/video_client.cpp \
						video_client/video_connector.cpp
						

game_serverdir=$(prefix)/conf/

TOLUA = /usr/local/bin/tolua++

script/ToScript.cpp:script/ToScript.pkg @GAME_TOLUA_DEPEND@
	$(TOLUA) -o script/ToScript.cpp script/ToScript.pkg
    
#clean_file:
#   $(RM) script/ToScript.cpp
