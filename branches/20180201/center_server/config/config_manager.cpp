#include "config_manager.hpp"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "tblh/RankReward.tbls.h"
#include "tblh/RankType.tbls.h"
#include "tblh/City.tbls.h"
#include "tblh/Monster.tbls.h"
#include "tblh/FightAgainst.tbls.h"
#include "tblh/MonsterHero.tbls.h"
#include "tblh/Comprehensive.tbls.h"

#include "tblh/CountryOfficial.tbls.h"
#include "tblh/FamilyOfficial.tbls.h"
#include "tblh/Family.tbls.h"
#include "tblh/FamilyPray.tbls.h"
#include "tblh/Country.tbls.h"
#include "tblh/CountryFlag.tbls.h"
#include "tblh/Activities.tbls.h"
#include "tblh/BossBornTable.tbls.h"
#include "tblh/DropWorldTable.tbls.h"
#include "tblh/TimeTable.tbls.h"
#include "tblh/MailTable.tbls.h"
#include "tblh/SysNoticeTable.tbls.h"
#include "tblh/ShopTable.tbls.h"
#include "tblh/Item.tbls.h"
#include "tblh/TroopType.tbls.h"
#include "user_troop/user_troop_mgr.hpp"
#include "tblh/MapTable.tbls.h"
#include "tblh/DungeonTable.tbls.h"
#include "tblh/ForbidWordTable.tbls.h"
#include "tblh/Goods.tbls.h"
#include "goods/goods_manager.hpp"
#include "tblh/TradeProsperity.tbls.h"
#include "tblh/Match.tbls.h"
#include "tblh/Expedition.tbls.h"
#include "offline_fight/offline_fight_manager.hpp"
#include "tblh/LanguageTable.tbls.h"
#include "tblh/SysChannel.tbls.h"
#include "tblh/TowerTrigger.tbls.h"
#include "tblh/OfflineArenaClass.tbls.h"
#include "tblh/OfflineArenaMatch.tbls.h"
#include "tblh/OfflineArenaRobot.tbls.h"
#include "tblh/OfflineArenaAward.tbls.h"
#include "tblh/OfflineRobotInfo.tbls.h"
#include "tblh/BossFamilyTable.tbls.h"
#include "tblh/BossPersonalTable.tbls.h"
#include "tblh/RandomShopTable.tbls.h"
#include "tblh/ShopItemTable.tbls.h"
#include "tblh/LuckydrawWish.tbls.h"
#include "tblh/CDKeyTable.tbls.h"

// 中心服务器不加载
void addBossPersonalReward(uint32_t, uint32_t) {
	
}

// 中心服务器不加载
void addBossFamilyReward(uint32_t, uint32_t) {
	
}

bool load_config()
{
	if (NULL != MonsterManager::getInstancePtr() &&
		!MonsterManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("MonsterManager");
	}

	if (NULL != FightAgainstManager::getInstancePtr() &&
		!FightAgainstManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("FightAgainstManager");
	}

	if (NULL != MonsterHeroManager::getInstancePtr() &&
		!MonsterHeroManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("MonsterHeroManager");
	}

	if (NULL != RankRewardManager::getInstancePtr() &&
		!RankRewardManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("RankRewardManager");
	}

	if (NULL != RankTypeManager::getInstancePtr() &&
		!RankTypeManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("RankTypeManager");
	}

	if (NULL != CityManager::getInstancePtr() &&
		!CityManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("CityManager");
	}

	if (NULL != ComprehensiveManager::getInstancePtr() &&
		!ComprehensiveManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ComprehensiveManager");
	}

	if (NULL != CountryManager::getInstancePtr() &&
		!CountryManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
	else
	{
		PUSH_TO_CONFIG_LIST("CountryManager");
	}

	if (NULL != FamilyOfficialManager::getInstancePtr() &&
		!FamilyOfficialManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("FamilyOfficialManager");
	}

	if (NULL != CountryOfficialManager::getInstancePtr() &&
		!CountryOfficialManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("CountryOfficialManager");
	}

	if (NULL != FamilyManager::getInstancePtr() &&
		!FamilyManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("FamilyManager");
	}

	if (NULL != FamilyPrayManager::getInstancePtr() &&
		!FamilyPrayManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("FamilyPrayManager");
	}

	if (NULL != ActivitiesManager::getInstancePtr() &&
		!ActivitiesManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ActivitiesManager");
	}

	if (NULL != BossBornTableManager::getInstancePtr() &&
		!BossBornTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("BossBornTableManager");
	}

	if (NULL != DropWorldTableManager::getInstancePtr() &&
		!DropWorldTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("DropWorldTableManager");
	}

	if (NULL != TimeTableManager::getInstancePtr() &&
		!TimeTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("TimeTableManager");
	}

	if (NULL != MailTableManager::getInstancePtr() &&
		!MailTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("MailTableManager");
	}

	if (NULL != SysNoticeTableManager::getInstancePtr() &&
		!SysNoticeTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("SysNoticeTableManager");
	}

	if (NULL != ShopTableManager::getInstancePtr() &&
		!ShopTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ShopTableManager");
	}

	if (NULL != ItemManager::getInstancePtr() &&
		!ItemManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ItemManager");
	}

	if (NULL != TroopTypeManager::getInstancePtr() &&
		!TroopTypeManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("TroopTypeManager");
	}

	if (NULL != MapTableManager::getInstancePtr() &&
		!MapTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("MapTableManager");
	}

	if (NULL != DungeonTableManager::getInstancePtr() &&
		!DungeonTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("DungeonTableManager");
	}

	if (NULL != ForbidWordTableManager::getInstancePtr() &&
		!ForbidWordTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("ForbidWordTableManager");
    }

	if (NULL != GoodsManager::getInstancePtr() &&
		!GoodsManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("GoodsManager");
		goods_manager_t::make_conf();
	}

	if (NULL != TradeProsperityManager::getInstancePtr() &&
		!TradeProsperityManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("TradeProsperityManager");
	}

	if (NULL != ExpeditionManager::getInstancePtr() &&
		!ExpeditionManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ExpeditionManager");
	}
	
	if (NULL != MatchManager::getInstancePtr() &&
		!MatchManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("MatchManager");
		offline_fight_manager_t::make_conf();
	}

	if (NULL != LanguageTableManager::getInstancePtr() &&
		!LanguageTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("LanguageTableManager");
	}

	if (NULL != SysChannelManager::getInstancePtr() &&
		!SysChannelManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("SysChannelManager");
	}

	if (NULL != TowerTriggerManager::getInstancePtr() &&
		!TowerTriggerManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("TowerTriggerManager");
	}

	if (NULL != OfflineArenaClassManager::getInstancePtr() &&
		!OfflineArenaClassManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("OfflineArenaClassManager");
	}

	if (NULL != OfflineArenaMatchManager::getInstancePtr() &&
		!OfflineArenaMatchManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("OfflineArenaMatchManager");
	}

	if (NULL != OfflineArenaRobotManager::getInstancePtr() &&
		!OfflineArenaRobotManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("OfflineArenaRobotManager");
	}
	
	if (NULL != OfflineArenaAwardManager::getInstancePtr() &&
		!OfflineArenaAwardManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("OfflineArenaAwardManager");
	}

	if (NULL != BossPersonalTableManager::getInstancePtr() &&
		!BossPersonalTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("BossPersonalTableManager");
	}

	if (NULL != BossFamilyTableManager::getInstancePtr() &&
		!BossFamilyTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("BossFamilyTableManager");
	}

	if (NULL != OfflineRobotInfoManager::getInstancePtr() &&
		!OfflineRobotInfoManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("OfflineRobotInfoManager");
	}

	if (NULL != RandomShopTableManager::getInstancePtr() &&
		!RandomShopTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("RandomShopTableManager");
	}

	if (NULL != ShopItemTableManager::getInstancePtr() &&
		!ShopItemTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("ShopItemTableManager");
	}

	if (NULL != CountryFlagManager::getInstancePtr() &&
		!CountryFlagManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("CountryFlagManager");
	}

	if (NULL != LuckydrawWishManager::getInstancePtr() &&
		!LuckydrawWishManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("LuckydrawWishManager");
	}
	
	if (NULL != CDKeyTableManager::getInstancePtr() && 
		!CDKeyTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
			return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("CDKeyTableManager");
	}

	log_info("load config success!");
	return true;
}

void release_config()
{
	MonsterManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MonsterManager");

	FightAgainstManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FightAgainstManager");

	MonsterHeroManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MonsterHeroManager");

	RankRewardManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("RankRewardManager");

	RankTypeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("RankTypeManager");

	CityManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("CityManager");

	ComprehensiveManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ComprehensiveManager");

	CountryManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("CountryManager");

	FamilyOfficialManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FamilyOfficialManager");

	CountryOfficialManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("CountryOfficialManager");

	FamilyManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FamilyManager");

	FamilyPrayManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FamilyPrayManager");

	ActivitiesManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ActivitiesManager");

	BossBornTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("BossBornTableManager");

	DropWorldTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("DropWorldTableManager");

	TimeTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TimeTableManager");

	MailTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MailTableManager");

	SysNoticeTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SysNoticeTableManager");

	ShopTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ShopTableManager");

	ItemManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ItemManager");

	TroopTypeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TroopTypeManager");

	MapTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MapTableManager");

	DungeonTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("DungeonTableManager");

    ForbidWordTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ForbidWordTableManager");

	GoodsManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("GoodsManager");
	goods_manager_t::clear_conf();

	TradeProsperityManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TradeProsperityManager");

	ExpeditionManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ExpeditionManager");

	MatchManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MatchManager");
	offline_fight_manager_t::clear_conf();

	LanguageTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LanguageTableManager");

	SysChannelManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SysChannelManager");

	TowerTriggerManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TowerTriggerManager"); 

	OfflineArenaClassManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("OfflineArenaClassManager");

	OfflineArenaMatchManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("OfflineArenaMatchManager");

	OfflineArenaRobotManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("OfflineArenaRobotManager");
	
	OfflineArenaAwardManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("OfflineArenaAwardManager");

	BossPersonalTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("BossPersonalTableManager");

	BossFamilyTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("BossFamilyTableManager");	

	OfflineRobotInfoManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("OfflineRobotInfoManager");

	RandomShopTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("RandomShopTableManager");

	ShopItemTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ShopItemTableManager");

	CountryFlagManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("CountryFlagManager");

	LuckydrawWishManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LuckydrawWishManager");

	CDKeyTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("CDKeyTableManager");

	//检查表的释放
	CHECK_CONFIG_LIST_SIZE();
	
    log_info("release config success!");
}

void print_config_list()
{
	for (auto& it : config_list)
	{
		log_error("[%s]table release fail!", it.c_str());
	}
}
