#ifndef __COMMON_CACHE_KEY_HPP__
#define __COMMON_CACHE_KEY_HPP__

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include "utility.hpp"
struct cache_key_t
{
    static std::string create(uint64_t id, const std::string& name)
    {
        std::string str = boost::str(boost::format("%lu|%s") % id % name.c_str());
        return std::move(str);
    }

    static std::string create(uint32_t id, const std::string& name)
    {
        std::string str = boost::str(boost::format("%u|%s") % id % name.c_str());
        return std::move(str);
    }

    static std::string create(int32_t id, const std::string& name)
    {
        std::string str = boost::str(boost::format("%d|%s") % id % name.c_str());
        return std::move(str);
    }

    static std::string create(int64_t id, const std::string& name)
    {
        std::string str = boost::str(boost::format("%ld|%s") % id % name.c_str());
        return std::move(str);
    }


    static std::string create(uint64_t id, const std::string& name, uint32_t sec_id)
    {
        std::string str = boost::str(boost::format("%lu|%s|%u") % id % name.c_str() %sec_id);
        return std::move(str);
    }

    static uint64_t extract(const std::string& key, std::string& name)
    {
        name = key.substr(key.find_first_of("|")+1, key.size());
        std::string id = key.substr(0, key.find_first_of("|"));
        return common::string_util_t::string_to_uint64(id);
    }
    static uint64_t extract(const std::string& key, std::string& name, uint32_t& sec_id)
    {
        auto pos1 = key.find_first_of("|");
        std::string id;
        if( std::string::npos != pos1)
        {
            id = key.substr(0, pos1);
        }
        
        name = key.substr(key.find_first_of("|")+1, key.size());
        auto pos2 = name.find_last_of("|");
        if( std::string::npos != pos2)
        {
            sec_id = boost::lexical_cast<uint32_t>(name.substr(pos2 + 1, name.size()));
            name = name.substr(0, pos2);
        }
        return common::string_util_t::string_to_uint64(id);
    }

};

namespace cache_name
{
    const std::string roles("roles");
    const std::string heros("heros");
	const std::string items("items");
	const std::string formations("formations");
	const std::string talent("talent");
	//const std::string pvp_skills("pvp_skills");
    //const std::string equips("equips");
    //const std::string pve_data("pve_data");
    //const std::string campaigns("campaigns");
    //const std::string matrixs("matrixs");
    //const std::string produces("produces");
    //const std::string pvp_info("pvp_info");
    //const std::string pvp_record("pvp_record");
    //const std::string shop("shop");
    //const std::string shop_item("shop_item");
    //const std::string mails("mails");
    //const std::string recharge("recharge");
    //const std::string sign("sign");
    //const std::string daily_task("daily_task");
    const std::string tasks("tasks");
	const std::string shilian("shilian");
	const std::string role_trade_item("role_trade_item");
	const std::string mount_data("mount_data");
	const std::string bounty("bounty");
	const std::string activity("activity");
	const std::string arena("arena");
	const std::string achieve("achieve");
	const std::string tower("tower");
	const std::string red_point("red_point");
	const std::string luckydraw("luckydraw");
	const std::string dungeon("dungeon");
	const std::string expedition("expedition");
	const std::string active_reward("active_reward");
	const std::string random_shop("random_shop");
	const std::string level_reward("level_reward");
	const std::string level_reward_mail("level_reward_mail");	//等级奖励邮件
	const std::string shake_money_tree("shake_money_tree");	//等级奖励邮件
	const std::string sevenday_login("sevenday_login");	//7日登陆
}


#endif
