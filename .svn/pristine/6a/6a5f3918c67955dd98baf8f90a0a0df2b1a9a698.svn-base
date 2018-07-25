#include "config_manager.hpp"

#include "tblh/MapTable.tbls.h"
#include "tblh/JumpTable.tbls.h"
#include "tblh/Tactic.tbls.h"
#include "tblh/Task.tbls.h"
#include "tblh/TaskCircle.tbls.h"
#include "tblh/TaskShilian.tbls.h"
#include "tblh/TaskEvent.tbls.h"
#include "tblh/BuffTable.tbls.h"
#include "tblh/BuffBigType.tbls.h"
#include "tblh/BuffSmallType.tbls.h"
//#include "tblh/SkillTable.tbls.h"
#include "tblh/SkillEffectTable.tbls.h"
#include "tblh/SkillAI.tbls.h"
#include "tblh/SkillTargetAI.tbls.h"
#include "tblh/MountTable.tbls.h"
#include "tblh/Item.tbls.h"
#include "tblh/Trade.tbls.h"
#include "tblh/TradeFloat.tbls.h"
#include "tblh/TradeGoodsType.tbls.h"
#include "tblh/City.tbls.h"
#include "tblh/HeroGrade.tbls.h"
#include "tblh/HeroStar.tbls.h"
#include "tblh/Hero.tbls.h"
#include "tblh/HeroPlugin.tbls.h"
#include "tblh/HeroJobTransfer.tbls.h"
#include "tblh/HeroGet.tbls.h"
#include "tblh/Prestige.tbls.h"
#include "tblh/LevelDemad.tbls.h"
#include "tblh/HeroTalentPos.tbls.h"
#include "tblh/HeroTalentLevel.tbls.h"
#include "tblh/HeroTalentAdd.tbls.h"
#include "tblh/HeroTalentPlan.tbls.h"
#include "tblh/HeroFamous.tbls.h"
#include "tblh/Famous.tbls.h"
#include "tblh/Monster.tbls.h"
#include "tblh/FightAgainst.tbls.h"
#include "tblh/MonsterHero.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/Bounty.tbls.h"
#include "tblh/DropBagTable.tbls.h"
#include "tblh/DropTable.tbls.h"
#include "tblh/LevelupTable.tbls.h"
#include "tblh/DropWorldTable.tbls.h"
#include "tblh/TimeTable.tbls.h"
#include "tblh/CollectTable.tbls.h"
#include "tblh/Activities.tbls.h"
#include "tblh/Liveness.tbls.h"
#include "tblh/MailTable.tbls.h"
#include "tblh/MonsterBaseAttrTable.tbls.h"
#include "tblh/ArenaBattleAwardTable.tbls.h"
#include "tblh/ArenaClassTable.tbls.h"
#include "tblh/ArenaWeekAwardTable.tbls.h"
#include "tblh/Treasure.tbls.h"
#include "tblh/TreasureMake.tbls.h"
#include "tblh/TreasureEvent.tbls.h"
#include "tblh/SysNoticeTable.tbls.h"

#include "tblh/Pk.tbls.h"
#include "tblh/Country.tbls.h"
#include "tblh/Equip.tbls.h"
#include "tblh/EquipExattr.tbls.h"
#include "tblh/EquipExattrMake.tbls.h"
#include "tblh/EquipExchange.tbls.h"
#include "tblh/EquipExchangePool.tbls.h"
#include "tblh/EquipExattrRemake.tbls.h"
#include "tblh/EquipSmelt.tbls.h"
#include "tblh/EquipSuit.tbls.h"
#include "tblh/EquipAttrScore.tbls.h"
#include "tblh/EquipName.tbls.h"
#include "tblh/EquipSpecial.tbls.h"
#include "tblh/EquipStar.tbls.h"
#include "tblh/EquipStrengthen.tbls.h"
#include "tblh/EquipStrengthenMaster.tbls.h"
#include "tblh/EquipSoul.tbls.h"
#include "tblh/SkillScore.tbls.h"
#include "tblh/TalentScore.tbls.h"
#include "equip/equip_manager.hpp"
#include "task/task_manager.hpp"
#include "tblh/GeneralEventTargetTable.tbls.h"
#include "general_event/general_info_manager.hpp"
#include "protos_fwd.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "tblh/ShopTable.tbls.h"
#include "tblh/Achievelist.tbls.h"
#include "tblh/Achievelevel.tbls.h"
#include "tblh/Achieverecord.tbls.h"
#include "tblh/Tower.tbls.h"
#include "tblh/TowerAchieve.tbls.h"
#include "tblh/TowerTrigger.tbls.h"
#include "tblh/Luckydraw.tbls.h"
#include "luckydraw/luckydraw_manager.hpp"
#include "tblh/DungeonTable.tbls.h"
#include "tblh/DungeonType.tbls.h"
#include "tblh/LifeState.tbls.h"
#include "tblh/LifeStar.tbls.h"
#include "tblh/RoleBuff.tbls.h"
#include "tblh/RoleBuffRule.tbls.h"
#include "tblh/PrefixNameTable.tbls.h"
#include "tblh/SurNameTable.tbls.h"
#include "tblh/FemaleNameTable.tbls.h"
#include "tblh/MaleNameTable.tbls.h"
#include "tblh/VirtualNoticeTable.tbls.h"
#include "tblh/RestraintTable.tbls.h"
#include "tblh/Expedition.tbls.h"
#include "tblh/ExpeditionBox.tbls.h"
#include "tblh/ExpeditionShop.tbls.h"
#include "tblh/Section.tbls.h"
#include "tblh/SectionReward.tbls.h"
#include "tblh/ActiveReward.tbls.h"
#include "tblh/VIP.tbls.h"
#include "tblh/Recharge.tbls.h"
#include "tblh/RandomShopTable.tbls.h"
#include "tblh/ShopItemTable.tbls.h"
#include "tblh/Patrol.tbls.h"
#include "tblh/PatrolExp.tbls.h"
#include "tblh/LanguageTable.tbls.h"
#include "tblh/SysChannel.tbls.h"
#include "tblh/LevelRewardTable.tbls.h"
#include "tblh/GuideFightTable.tbls.h"
#include "tblh/OfflineArenaClass.tbls.h"
#include "tblh/SectionElite.tbls.h"
#include "field_boss/field_boss_manager.hpp"
#include "tblh/BossFamilyTable.tbls.h"
#include "tblh/BossPersonalTable.tbls.h"
#include "tblh/LevelRewardMailTable.tbls.h"
#include "tblh/ItemMerageTable.tbls.h"
#include "tblh/LuckydrawSeq.tbls.h"
#include "tblh/LuckydrawWish.tbls.h"
#include "tblh/LuckydrawType.tbls.h"
#include "tblh/LabelTable.tbls.h"

#include "tblh/MoneyTreeReward.tbls.h"
#include "tblh/MoneyTreeShake.tbls.h"
#include "tblh/MoneyTreeTable.tbls.h"
#include "tblh/BuyPermission.tbls.h"
#include "tblh/SevendayLogin.tbls.h"
#include "tblh/TutorTable.tbls.h"
#include "tblh/HeadTable.tbls.h"
#include "tblh/FrameTable.tbls.h"
#include "tblh/MonthCard.tbls.h"
#include "tblh/LiandanTable.tbls.h"
#include "tblh/LiandanluTable.tbls.h"
#include "tblh/FightTypeTable.tbls.h"
#include "tblh/PatrolHire.tbls.h"
#include "tblh/PatrolLevel.tbls.h"
#include "tblh/Patrolstar.tbls.h"
#include "tblh/WsScoreTreasuryTable.tbls.h"
#include "tblh/LevelFundTable.tbls.h"
#include "tblh/BusinessActiveTable.tbls.h"
#include "tblh/WsTreasuryTable.tbls.h"
#include "tblh/BAExchangeItem.tbls.h"
#include "tblh/LimitBuyTable.tbls.h"
#include "tblh/SDTargetCountTable.tbls.h"
#include "tblh/SDTargetTable.tbls.h"
#include "tblh/FamilySkill.tbls.h"

#include "tblh/KingWarPersonal.tbls.h"
#include "tblh/KingWarRank.tbls.h"
#include "tblh/GeneralEventTable.tbls.h"
#include "tblh/GeneralEventTargetTable.tbls.h"
#include "tblh/LevelLimitTable.tbls.h"
#include "tblh/LevelAddExpRateTable.tbls.h"
#include "tblh/Holiday.tbls.h"
#include "tblh/Career.tbls.h"
#include "tblh/RechargeTotal.tbls.h"
#include "tblh/RechargeCheap.tbls.h"
#include "tblh/GoldLuckydraw.tbls.h"
#include "tblh/Friendliness.tbls.h"
#include "tblh/FashionMerageTable.tbls.h"
#include "tblh/FamilyWarPersonal.tbls.h"
#include "tblh/FamilyWarRank.tbls.h"
#include "tblh/Office.tbls.h"
#include "tblh/HandBook.tbls.h"
#include "tblh/TimeRank.tbls.h"

USING_NS_COMMON;

void addBossPersonalReward(uint32_t monster_tid, uint32_t id) {
	field_boss_manager_t::add_persion_reward(monster_tid, id);
}

void addBossFamilyReward(uint32_t monster_tid, uint32_t id) {
	field_boss_manager_t::add_family_reward(monster_tid, id);
}

bool load_config()
{
    if (!load_common_config())
        return false;

    if (NULL != TaskManager::getInstancePtr() &&
		!TaskManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("TaskManager");
    }

    if (NULL != TaskCircleManager::getInstancePtr() &&
		!TaskCircleManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("TaskCircleManager");
    }

	if (NULL != TaskShilianManager::getInstancePtr() &&
		!TaskShilianManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("TaskShilianManager");
	}

	if (NULL != TaskEventManager::getInstancePtr() &&
		!TaskEventManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("TaskEventManager");
	}

    if (NULL != BountyManager::getInstancePtr() &&
		!BountyManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("BountyManager");
    }
    
    if (NULL != PrestigeManager::getInstancePtr() &&
		!PrestigeManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("PrestigeManager");
    }

    if (NULL != LevelDemadManager::getInstancePtr() &&
		!LevelDemadManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("LevelDemadManager");
    }

    if (NULL != MountTableManager::getInstancePtr() &&
		!MountTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("MountTableManager");
    }

    if (NULL != TradeManager::getInstancePtr() &&
		!TradeManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("TradeManager");
    }

    if (NULL != TradeGoodsTypeManager::getInstancePtr() &&
		!TradeGoodsTypeManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("TradeGoodsTypeManager");
    }

    if (NULL != TradeFloatManager::getInstancePtr() &&
		!TradeFloatManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("TradeFloatManager");
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

    if (NULL != MapTableManager::getInstancePtr() &&
		!MapTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("MapTableManager");
    }

    if (NULL != JumpTableManager::getInstancePtr() &&
		!JumpTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("JumpTableManager");
    }

    if (NULL != DropBagTableManager::getInstancePtr() &&
		!DropBagTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("DropBagTableManager");
    }

    if (NULL != DropTableManager::getInstancePtr() &&
		!DropTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("DropTableManager");
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

    if (NULL != LevelupTableManager::getInstancePtr() &&
		!LevelupTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("LevelupTableManager");
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

    if (NULL != ActivitiesManager::getInstancePtr() &&
		!ActivitiesManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("ActivitiesManager");
    }

    if (NULL != LivenessManager::getInstancePtr() &&
		!LivenessManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("LivenessManager");
    }
// 	if (!CollectTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
// 	{
// 		return false;
// 	}

    if (NULL != MailTableManager::getInstancePtr() &&
		!MailTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("MailTableManager");
    }

    if (NULL != ArenaClassTableManager::getInstancePtr() &&
		!ArenaClassTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("ArenaClassTableManager");
    }

    if (NULL != ArenaBattleAwardTableManager::getInstancePtr() &&
		!ArenaBattleAwardTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("ArenaBattleAwardTableManager");
    }

    if (NULL != ArenaWeekAwardTableManager::getInstancePtr() &&
		!ArenaWeekAwardTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("ArenaWeekAwardTableManager");
    }

    if (NULL != TreasureManager::getInstancePtr() &&
		!TreasureManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("TreasureManager");
    }
    
    if (NULL != TreasureMakeManager::getInstancePtr() &&
		!TreasureMakeManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("TreasureMakeManager");
    }

    if (NULL != TreasureEventManager::getInstancePtr() &&
		!TreasureEventManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("TreasureEventManager");
    }

    if (NULL != CollectTableManager::getInstancePtr() &&
		!CollectTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("CollectTableManager");
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

    if (NULL != PkManager::getInstancePtr() &&
		!PkManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("PkManager");
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

    if (NULL != EquipExattrMakeManager::getInstancePtr() &&
		!EquipExattrMakeManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("EquipExattrMakeManager");
    }

    if (NULL != EquipExchangeManager::getInstancePtr() &&
		!EquipExchangeManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("EquipExchangeManager");
    }

    if (NULL != EquipExchangePoolManager::getInstancePtr() &&
		!EquipExchangePoolManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("EquipExchangePoolManager");
    }

    if (NULL != EquipExattrRemakeManager::getInstancePtr() &&
		!EquipExattrRemakeManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("EquipExattrRemakeManager");
    }

    if (NULL != EquipSmeltManager::getInstancePtr() &&
		!EquipSmeltManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("EquipSmeltManager");
	}

    if (NULL != EquipAttrScoreManager::getInstancePtr() &&
		!EquipAttrScoreManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("EquipAttrScoreManager");
	}

	if (NULL != EquipStarManager::getInstancePtr() &&
		!EquipStarManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("EquipStarManager");
	}

	if (NULL != EquipStrengthenMasterManager::getInstancePtr() &&
		!EquipStrengthenMasterManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("EquipStrengthenMasterManager");
	}

	if (NULL != EquipSoulManager::getInstancePtr() &&
		!EquipSoulManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("EquipSoulManager");
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

    if (NULL != AchievelistManager::getInstancePtr() &&
		!AchievelistManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("AchievelistManager");
    }

    if (NULL != AchievelevelManager::getInstancePtr() &&
		!AchievelevelManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("AchievelevelManager");
    }

    if (NULL != AchieverecordManager::getInstancePtr() &&
		!AchieverecordManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("AchieverecordManager");
    }

    if (!equip_manager_t::make_equip_info())
    {
        return false;
    }

    if (NULL != TowerManager::getInstancePtr() &&
		!TowerManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("TowerManager");
    }

    if (NULL != TowerAchieveManager::getInstancePtr() &&
		!TowerAchieveManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("TowerAchieveManager");
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
	
    if (NULL != LuckydrawManager::getInstancePtr() &&
		!LuckydrawManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("LuckydrawManager");
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

	if (NULL != DungeonTypeManager::getInstancePtr() &&
		!DungeonTypeManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("DungeonTypeManager");
	}

    if (NULL != LifeStateManager::getInstancePtr() &&
		!LifeStateManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("LifeStateManager");
    }

    if (NULL != RoleBuffManager::getInstancePtr() &&
		!RoleBuffManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("RoleBuffManager");
    }

    if (NULL != RoleBuffRuleManager::getInstancePtr() &&
		!RoleBuffRuleManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("RoleBuffRuleManager");
    }

    if (NULL != PrefixNameTableManager::getInstancePtr() &&
		!PrefixNameTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("PrefixNameTableManager");
    }

    if (NULL != SurNameTableManager::getInstancePtr() &&
		!SurNameTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("SurNameTableManager");
    }

    if (NULL != FemaleNameTableManager::getInstancePtr() && 
		!FemaleNameTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("FemaleNameTableManager");
    }

    if (NULL != MaleNameTableManager::getInstancePtr() &&
		!MaleNameTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("MaleNameTableManager");
    }

    if (NULL != VirtualNoticeTableManager::getInstancePtr() && 
		!VirtualNoticeTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("VirtualNoticeTableManager");
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

	if (NULL != ExpeditionBoxManager::getInstancePtr() &&
		!ExpeditionBoxManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ExpeditionBoxManager");
	}

	if (NULL != ExpeditionShopManager::getInstancePtr() &&
		!ExpeditionShopManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ExpeditionShopManager");
	}

	if (NULL != SectionManager::getInstancePtr() &&
		!SectionManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("SectionManager");
	}

	if (NULL != SectionRewardManager::getInstancePtr() &&
		!SectionRewardManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("SectionRewardManager");
	}

	if (NULL != ActiveRewardManager::getInstancePtr() &&
		!ActiveRewardManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ActiveRewardManager");
	}

	if (NULL != RechargeManager::getInstancePtr() &&
		!RechargeManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("RechargeManager");
	}

	if (NULL != VIPManager::getInstancePtr() &&
		!VIPManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("VIPManager");
	}

	if (NULL != RandomShopTableManager::getInstancePtr() &&
		!RandomShopTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("RandomShopTableManager");
	}

	if (NULL != ShopItemTableManager::getInstancePtr() &&
		!ShopItemTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ShopItemTableManager");
	}

	if (NULL != PatrolManager::getInstancePtr() &&
		!PatrolManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("PatrolManager");
	}

	if (NULL != PatrolExpManager::getInstancePtr() &&
		!PatrolExpManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("PatrolExpManager");
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

	if (NULL != LevelRewardTableManager::getInstancePtr() &&
		!LevelRewardTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	} 
	else 
	{
		PUSH_TO_CONFIG_LIST("LevelRewardTableManager");
	}

	if (NULL != GuideFightTableManager::getInstancePtr() &&
		!GuideFightTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("GuideFightTableManager");
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
	
	if (NULL != SectionEliteManager::getInstancePtr() &&
		!SectionEliteManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("SectionEliteManager");
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

	if (NULL != LevelRewardMailTableManager::getInstancePtr() &&
		!LevelRewardMailTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("LevelRewardMailTableManager");
	}

	if (NULL != ItemMerageTableManager::getInstancePtr() &&
		!ItemMerageTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("ItemMerageTableManager");
	}

	if (NULL != LuckydrawSeqManager::getInstancePtr() &&
		!LuckydrawSeqManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("LuckydrawSeqManager");
	}

	if (NULL != LuckydrawWishManager::getInstancePtr() &&
		!LuckydrawWishManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("LuckydrawWishManager");
	}

	if (NULL != LuckydrawTypeManager::getInstancePtr() &&
		!LuckydrawTypeManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("LuckydrawTypeManager");
	}

	if ( NULL != MoneyTreeTableManager::getInstancePtr() &&
		!MoneyTreeTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("MoneyTreeTableManager");
	}

	if ( NULL != MoneyTreeShakeManager::getInstancePtr() &&
		!MoneyTreeShakeManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("MoneyTreeShakeManager");
	}

	if ( NULL != MoneyTreeRewardManager::getInstancePtr() &&
		!MoneyTreeRewardManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("MoneyTreeRewardManager");
	}

	if (NULL != BuyPermissionManager::getInstancePtr() &&
		!BuyPermissionManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("BuyPermissionManager");
	}

	if (NULL != SevendayLoginManager::getInstancePtr() &&
		!SevendayLoginManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("SevendayLoginManager");
	}

	if (NULL != TutorTableManager::getInstancePtr() &&
		!TutorTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("TutorTableManager");
	}


	if (!luckydraw_manager_t::make_luckydraw_pool())
	{
		return false;
	}

	if (NULL != LabelTableManager::getInstancePtr() &&
		!LabelTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("LabelTableManager");
	}

	if (NULL != HeadTableManager::getInstancePtr() &&
		!HeadTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("HeadTableManager");
	}

	if (NULL != FrameTableManager::getInstancePtr() &&
		!FrameTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("FrameTableManager");
	}

	if (NULL != MonthCardManager::getInstancePtr() &&
		!MonthCardManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("MonthCardManager");
	}

	if (NULL != PatrolHireManager::getInstancePtr() &&
		!PatrolHireManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("PatrolHireManager");
	}

	if (NULL != PatrolLevelManager::getInstancePtr() &&
		!PatrolLevelManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("PatrolLevelManager");
	}

	if (NULL != PatrolstarManager::getInstancePtr() &&
		!PatrolstarManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("PatrolstarManager");
	}

	if (NULL != WsTreasuryTableManager::getInstancePtr() &&
		!WsTreasuryTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("WsTreasuryTableManager");
	}
	if (NULL != LevelFundTableManager::getInstancePtr() &&
		!LevelFundTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("LevelFundTableManager");
	}

	if (NULL != WsScoreTreasuryTableManager::getInstancePtr() &&
		!WsScoreTreasuryTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("WsScoreTreasuryTableManager");
	}


	if (NULL != BusinessActiveTableManager::getInstancePtr() &&
		!BusinessActiveTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("BusinessActiveTableManager");
	}

	if (NULL != LimitBuyTableManager::getInstancePtr() &&
		!LimitBuyTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("LimitBuyTableManager");
	}

	if (NULL != BAExchangeItemManager::getInstancePtr() &&
		!BAExchangeItemManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("BAExchangeItemManager");
	}

	if (NULL != SDTargetTableManager::getInstancePtr() &&
		!SDTargetTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("SDTargetTableManager");
	}

	if (NULL != FamilySkillManager::getInstancePtr() &&
		!FamilySkillManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("FamilySkillManager");
	}

	if (NULL != KingWarRankManager::getInstancePtr() &&
		!KingWarRankManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("KingWarRankManager");
	}

	if (NULL != KingWarPersonalManager::getInstancePtr() &&
		!KingWarPersonalManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("KingWarPersonalManager");
	}

	if (NULL != GeneralEventTargetTableManager::getInstancePtr() &&
		!GeneralEventTargetTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("GeneralEventTargetTableManager");
	}

	if (NULL != SDTargetCountTableManager::getInstancePtr() &&
		!SDTargetCountTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("SDTargetCountTableManager");
	}

	if (NULL != GeneralEventTableManager::getInstancePtr() &&
		!GeneralEventTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("GeneralEventTableManager");
	}

	if( NULL != LevelLimitTableManager::getInstancePtr() &&
		!LevelLimitTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("LevelLimitTableManager");
	}

	if (NULL != LevelAddExpRateTableManager::getInstancePtr() &&
		!LevelAddExpRateTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("LevelAddExpRateTableManager");
	}

	if (NULL != HolidayManager::getInstancePtr() &&
		!HolidayManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("HolidayManager");
	}
	
	if (NULL != CareerManager::getInstancePtr() &&
		!CareerManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("CareerManager");
	}

	if (NULL != RechargeTotalManager::getInstancePtr() &&
		!RechargeTotalManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("RechargeTotalManager");
	}

	if (NULL != RechargeCheapManager::getInstancePtr() &&
		!RechargeCheapManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("RechargeCheapManager");
	}

	if (NULL != GoldLuckydrawManager::getInstancePtr() &&
		!GoldLuckydrawManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("GoldLuckydrawManager");
	}

	if (NULL != FriendlinessManager::getInstancePtr() &&
		!FriendlinessManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("FriendlinessManager");
	}

	if (NULL != FashionMerageTableManager::getInstancePtr() &&
		!FashionMerageTableManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("FashionMerageTableManager");
	}

	if (NULL != FamilyWarRankManager::getInstancePtr() &&
		!FamilyWarRankManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("FamilyWarRankManager");
	}

	if (NULL != FamilyWarPersonalManager::getInstancePtr() &&
		!FamilyWarPersonalManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	} else {
		PUSH_TO_CONFIG_LIST("FamilyWarPersonalManager");
	}

	if (NULL != OfficeManager::getInstancePtr() &&
		!OfficeManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("OfficeManager");
	}

	if (NULL != HandBookManager::getInstancePtr() &&
		!HandBookManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("HandBookManager");
	}

	if (NULL != TimeRankManager::getInstancePtr() &&
		!TimeRankManager::getInstancePtr()->loadBinary(env::xml->config_path)) {
		return false;
	}
	else {
		PUSH_TO_CONFIG_LIST("TimeRankManager");
	}

    if (!task_manager_t::make_circle_info())
    {
        return false;
    }
	general_info_manager_t::make_conf();
    //////////////////////////////////////////////////////////////////////////////////////
    //以下是旧表读取，请用新表后，把下面的旧表删除！！！
    //////////////////////////////////////////////////////////////////////////////////////
    if (!ConfMgr<Hades::Data::Garrison, uint32_t>::GetSingleton().Init(env::xml->config_path))
    {
        log_error("fail to load Garrison config file");
        return false;
    }
    if (!ConfMgr<Hades::Data::GarrisonCitylist, uint32_t>::GetSingleton().Init(env::xml->config_path))
    {
        log_error("fail to load Garrison config file");
        return false;
    }

    log_info("load config success!");
    return true;
}

void release_config()
{
    TaskManager::delInstance();
    REMOVE_FROM_CONFIG_LIST("TaskManager");

    TaskCircleManager::delInstance();
    REMOVE_FROM_CONFIG_LIST("TaskCircleManager");

	TaskShilianManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TaskShilianManager");

	TaskEventManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TaskEventManager");

    task_manager_t::clear_circle_info();


    BountyManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("BountyManager");

    PrestigeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("PrestigeManager");

	LevelDemadManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LevelDemadManager");

    MountTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MountTableManager");

    TradeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TradeManager");

    TradeGoodsTypeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TradeGoodsTypeManager");

    TradeFloatManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TradeFloatManager");

    CityManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("CityManager");

    MapTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MapTableManager");

    JumpTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("JumpTableManager");

    DropBagTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("DropBagTableManager");

    DropTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("DropTableManager");

    DropWorldTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("DropWorldTableManager");

    LevelupTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LevelupTableManager");


    TimeTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TimeTableManager");


    MailTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MailTableManager");


    SysNoticeTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SysNoticeTableManager");


    ActivitiesManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ActivitiesManager");


    LivenessManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LivenessManager");


    PkManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("PkManager");


    CountryManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("CountryManager");


    EquipExattrMakeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipExattrMakeManager");


    EquipExchangeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipExchangeManager");


    EquipExchangePoolManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipExchangePoolManager");


    EquipExattrRemakeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipExattrRemakeManager");


    EquipSmeltManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipSmeltManager");
	
    EquipAttrScoreManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipAttrScoreManager");

	EquipStrengthenMasterManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipStrengthenMasterManager");

	EquipSoulManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipSoulManager");

	EquipStarManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipStarManager");

    equip_manager_t::clear_equip_info();


    ShopTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ShopTableManager");

    AchievelistManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("AchievelistManager");

    AchievelevelManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("AchievelevelManager");


    AchieverecordManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("AchieverecordManager");


    TowerManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TowerManager");


    TowerAchieveManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TowerAchieveManager");

	TowerTriggerManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TowerTriggerManager"); 
	
    LuckydrawManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LuckydrawManager");

    luckydraw_manager_t::clear_luckydraw_pool();

    DungeonTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("DungeonTableManager");

	DungeonTypeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("DungeonTypeManager");

    LifeStateManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LifeStateManager");


    RoleBuffManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("RoleBuffManager");

    RoleBuffRuleManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("RoleBuffRuleManager");

    CollectTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("CollectTableManager");

    ArenaClassTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ArenaClassTableManager");

    ArenaBattleAwardTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ArenaBattleAwardTableManager");

	ArenaWeekAwardTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ArenaWeekAwardTableManager");

    TreasureEventManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TreasureEventManager");

    TreasureMakeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TreasureMakeManager");

    TreasureManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TreasureManager");

    PrefixNameTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("PrefixNameTableManager");

    SurNameTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SurNameTableManager");

    FemaleNameTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FemaleNameTableManager");

    MaleNameTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MaleNameTableManager");

    VirtualNoticeTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("VirtualNoticeTableManager");

	ExpeditionManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ExpeditionManager"); 

	ExpeditionBoxManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ExpeditionBoxManager");

	ExpeditionShopManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ExpeditionShopManager");

	SectionManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SectionManager");

	SectionRewardManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SectionRewardManager");

	ActiveRewardManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ActiveRewardManager");

	RechargeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("RechargeManager");

	VIPManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("VIPManager");

	RandomShopTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("RandomShopTableManager");

	ShopItemTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ShopItemTableManager");

	PatrolManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("PatrolManager");

	PatrolExpManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("PatrolExpManager");


	LanguageTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LanguageTableManager");

	SysChannelManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SysChannelManager");

	LevelRewardTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LevelRewardTableManager");

	GuideFightTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("GuideFightTableManager");
	
	OfflineArenaClassManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("OfflineArenaClassManager");
	
	SectionEliteManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SectionEliteManager");

	BossPersonalTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("BossPersonalTableManager");

	BossFamilyTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("BossFamilyTableManager");

	LevelRewardMailTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LevelRewardMailTableManager");

	ItemMerageTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ItemMerageTableManager");

	LuckydrawSeqManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LuckydrawSeqManager");

	BuyPermissionManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("BuyPermissionManager");

	LabelTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LabelTableManager");

	MoneyTreeTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MoneyTreeTableManager");

	MoneyTreeShakeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MoneyTreeShakeManager");

	MoneyTreeRewardManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MoneyTreeRewardManager");

	SevendayLoginManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SevendayLoginManager");

	LuckydrawWishManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LuckydrawWishManager");

	LuckydrawTypeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LuckydrawTypeManager");

	TutorTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TutorTableManager");

	HeadTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("HeadTableManager");

	FrameTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FrameTableManager");

	MonthCardManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MonthCardManager");

	LevelFundTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LevelFundTableManager");

	BusinessActiveTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("BusinessActiveTableManager");

	PatrolHireManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("PatrolHireManager");

	PatrolLevelManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("PatrolLevelManager");

	PatrolstarManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("PatrolstarManager");

	WsTreasuryTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("WsTreasuryTableManager");

	WsScoreTreasuryTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("WsScoreTreasuryTableManager");

	BAExchangeItemManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("BAExchangeItemManager");

	LimitBuyTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LimitBuyTableManager");

	SDTargetCountTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SDTargetCountTableManager");

	SDTargetTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SDTargetTableManager");
	
	FamilySkillManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FamilySkillManager");

	KingWarRankManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("KingWarRankManager");

	KingWarPersonalManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("KingWarPersonalManager");

	GeneralEventTargetTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("GeneralEventTargetTableManager");

	GeneralEventTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("GeneralEventTableManager");

	LevelLimitTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LevelLimitTableManager");

	LevelAddExpRateTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LevelAddExpRateTableManager");

	HolidayManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("HolidayManager");

	CareerManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("CareerManager");

	RechargeTotalManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("RechargeTotalManager");

	RechargeCheapManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("RechargeCheapManager");

	GoldLuckydrawManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("GoldLuckydrawManager");

	FriendlinessManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FriendlinessManager");

	FashionMerageTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FashionMerageTableManager");

	FamilyWarRankManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FamilyWarRankManager");

	FamilyWarPersonalManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FamilyWarPersonalManager");

	FightTypeTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FightTypeTableManager");
	
	OfficeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("OfficeManager");

	HandBookManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("HandBookManager");

	TimeRankManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TimeRankManager");

    release_common_config();

    //检查表的释放
    CHECK_CONFIG_LIST_SIZE();
    //////////////////////////////////////////////////////////////////////////////////////
    //以下是旧表读取，请用新表后，把下面的旧表删除！！！
    //////////////////////////////////////////////////////////////////////////////////////
    ConfMgr<Hades::Data::Garrison, uint32_t>::DestructSingleton();
    ConfMgr<Hades::Data::GarrisonCitylist, uint32_t>::DestructSingleton();

    log_info("release config success!");
}

void reload_config()
{
	proto::server::gall_reload_config_notify msg;
	env::server->send_msg_to_center(op_cmd::ge_reload_config_notify, 0, msg);
	env::server->send_msg_to_db(op_cmd::gd_reload_config_notify, 0, msg);
	env::server->send_msg_to_all_gate(op_cmd::ga_reload_config_notify, 0, msg);
	env::server->send_msg_to_chat(op_cmd::gh_reload_config_notify, 0, msg);

	// 这个先清理
	field_boss_manager_t::reward_clear();

    //game_server
    reloadAllDTManager(env::xml->config_path);

    sys_notice_manager_t::reload();

    //notify center_server

    //notify db_server

    //notify gate_server

    //notify chat_server
}

bool load_common_config()
{	
	if (NULL != ComprehensiveManager::getInstancePtr() && 
	!ComprehensiveManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("ComprehensiveManager");
    }

    if (NULL != HeroStarManager::getInstancePtr() && 
		!HeroStarManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("HeroStarManager");
    }

    if (NULL != HeroManager::getInstancePtr() && 
		!HeroManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("HeroManager");
    }

    if (NULL != HeroGradeManager::getInstancePtr() && 
		!HeroGradeManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("HeroGradeManager");
	}

	if (NULL != HeroPluginManager::getInstancePtr() &&
		!HeroPluginManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("HeroPluginManager");
	}

	if (NULL != HeroTalentPosManager::getInstancePtr() &&
		!HeroTalentPosManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("HeroTalentPosManager");
	}

	if (NULL != HeroTalentAddManager::getInstancePtr() &&
		!HeroTalentAddManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("HeroTalentAddManager");
	}

	if (NULL != HeroTalentLevelManager::getInstancePtr() &&
		!HeroTalentLevelManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("HeroTalentLevelManager");
	}

	if (NULL != HeroTalentPlanManager::getInstancePtr() &&
		!HeroTalentPlanManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("HeroTalentPlanManager");
	}

    if (NULL != HeroJobTransferManager::getInstancePtr() && 
		!HeroJobTransferManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("HeroJobTransferManager");
    }

    if (NULL != HeroGetManager::getInstancePtr() && 
		!HeroGetManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("HeroGetManager");
    }

	if (NULL != HeroFamousManager::getInstancePtr() &&
		!HeroFamousManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("HeroFamousManager");
	}

	if (NULL != FamousManager::getInstancePtr() &&
		!FamousManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("FamousManager");
	}

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

    if (NULL != TacticManager::getInstancePtr() && 
		!TacticManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("TacticManager");
    }

    if (NULL != BuffTableManager::getInstancePtr() && 
		!BuffTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("BuffTableManager");
	}

	if (NULL != BuffBigTypeManager::getInstancePtr() &&
		!BuffBigTypeManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("BuffBigTypeManager");
	}

	if (NULL != BuffSmallTypeManager::getInstancePtr() &&
		!BuffSmallTypeManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("BuffSmallTypeManager");
	}

//     if (NULL != SkillTableManager::getInstancePtr() && 
// 		!SkillTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
//     {
//         return false;
//     }
//     else
//     {
//         PUSH_TO_CONFIG_LIST("SkillTableManager");
//     }

    if (NULL != SkillEffectTableManager::getInstancePtr() && 
		!SkillEffectTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("SkillEffectTableManager");
	}

	if (NULL != SkillAIManager::getInstancePtr() &&
		!SkillAIManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("SkillAIManager");
	}

	if (NULL != SkillTargetAIManager::getInstancePtr() &&
		!SkillTargetAIManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("SkillTargetAIManager");
	}

    if (NULL != MonsterBaseAttrTableManager::getInstancePtr() && 
		!MonsterBaseAttrTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("MonsterBaseAttrTableManager");
	}

	if (NULL != RestraintTableManager::getInstancePtr() &&
		!RestraintTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("RestraintTableManager");
	}

	if (NULL != EquipManager::getInstancePtr() &&
		!EquipManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("EquipManager");
	}

	if (NULL != EquipSuitManager::getInstancePtr() &&
		!EquipSuitManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("EquipSuitManager");
	}

	if (NULL != EquipSpecialManager::getInstancePtr() &&
		!EquipSpecialManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("EquipSpecialManager");
	}

	if (NULL != EquipStrengthenManager::getInstancePtr() &&
		!EquipStrengthenManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("EquipStrengthenManager");
	}

	if (NULL != EquipExattrManager::getInstancePtr() &&
		!EquipExattrManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("EquipExattrManager");
	}

	if (NULL != EquipNameManager::getInstancePtr() &&
		!EquipNameManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("EquipNameManager");
	}

	if (NULL != LifeStarManager::getInstancePtr() &&
		!LifeStarManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("LifeStarManager");
	}

	if (NULL != SkillScoreManager::getInstancePtr() &&
		!SkillScoreManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("SkillScoreManager");
	}

	if (NULL != TalentScoreManager::getInstancePtr() &&
		!TalentScoreManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("TalentScoreManager");
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

	if (NULL != LiandanTableManager::getInstancePtr() &&
		!LiandanTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("LiandanTableManager");
	}

	if (NULL != LiandanluTableManager::getInstancePtr() &&
		!LiandanluTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("LiandanluTableManager");
	}

	if (NULL != FightTypeTableManager::getInstancePtr() &&
		!FightTypeTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("FightTypeTableManager");
	}
    return true;
}

void release_common_config()
{
    ComprehensiveManager::delInstance();
    REMOVE_FROM_CONFIG_LIST("ComprehensiveManager");

    HeroStarManager::delInstance();
    REMOVE_FROM_CONFIG_LIST("HeroStarManager");

    HeroManager::delInstance();
    REMOVE_FROM_CONFIG_LIST("HeroManager");

    HeroGradeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("HeroGradeManager");

	HeroPluginManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("HeroPluginManager");

	HeroTalentPosManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("HeroTalentPosManager");

	HeroTalentPlanManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("HeroTalentPlanManager");

	HeroTalentAddManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("HeroTalentAddManager");

	HeroTalentLevelManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("HeroTalentLevelManager");

    HeroJobTransferManager::delInstance();
    REMOVE_FROM_CONFIG_LIST("HeroJobTransferManager");

    HeroGetManager::delInstance();
    REMOVE_FROM_CONFIG_LIST("HeroGetManager");

	HeroFamousManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("HeroFamousManager");

	FamousManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FamousManager");

    MonsterManager::delInstance();
    REMOVE_FROM_CONFIG_LIST("MonsterManager");

    FightAgainstManager::delInstance();
    REMOVE_FROM_CONFIG_LIST("FightAgainstManager");

    MonsterHeroManager::delInstance();
    REMOVE_FROM_CONFIG_LIST("MonsterHeroManager");

    TacticManager::delInstance();
    REMOVE_FROM_CONFIG_LIST("TacticManager");

    BuffTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("BuffTableManager");

	BuffBigTypeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("BuffBigTypeManager");

	BuffSmallTypeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("BuffSmallTypeManager");

//     SkillTableManager::delInstance();
//     REMOVE_FROM_CONFIG_LIST("SkillTableManager");

    SkillEffectTableManager::delInstance();
    REMOVE_FROM_CONFIG_LIST("SkillEffectTableManager");

	SkillAIManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SkillAIManager");

	SkillTargetAIManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SkillTargetAIManager");

    MonsterBaseAttrTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MonsterBaseAttrTableManager");

	RestraintTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("RestraintTableManager");
	
	EquipManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipManager");

	EquipSuitManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipSuitManager");

	EquipSpecialManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipSpecialManager");

	EquipStrengthenManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipStrengthenManager");

	EquipExattrManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipExattrManager");

	EquipNameManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("EquipNameManager");

	LifeStarManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LifeStarManager");

	SkillScoreManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SkillScoreManager");

	TalentScoreManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TalentScoreManager");

	ItemManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ItemManager");

	LiandanTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LiandanTableManager");

	LiandanluTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("LiandanluTableManager");

	FightTypeTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FightTypeTableManager");
}

bool load_cross_config()
{
    if (!load_common_config())
        return false;
    log_info("load config success!");
    return true;
}

void release_cross_config()
{
    release_common_config();

    //检查表的释放
    CHECK_CONFIG_LIST_SIZE();
    log_info("release config success!");
}

void reload_cross_config()
{
    reloadAllDTManager(env::xml->config_path);
}

void print_config_list()
{
    for (auto& it : config_list)
    {
        log_error("[%s]table release fail!", it.c_str());
    }
}