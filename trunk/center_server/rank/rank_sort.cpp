#include "rank_sort.hpp"
#include "redis_client.hpp"
#include "tblh/RankType.tbls.h"
#include "config/config_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "family/family_manager.hpp"
#include "redis/global_user_data_mgr.hpp"
#include "country/country_mgr.hpp"
#include "redis/rank_data_mgr.hpp"
#include "tblh/RankReward.tbls.h"
#include "tblh/mailcode_enum.hpp"
#include "mail/mail_manager.hpp"
#include "tblh/MailTable.tbls.h"

rank_sort_t::rank_sort_t(common::rank_common::rank_type type, const char* name)
	: rank_t(type, name)
{

}

bool rank_sort_t::init()
{
	if (!rank_t::init()) {
		return false;
	}

	std::vector<uint64_t> vecs;
	redis_client_t::get_all_keys<uint64_t, std::vector<uint64_t>>(m_rank_name.c_str(), vecs);

	std::vector<std::pair<uint64_t, uint32_t>> batch_set;
	for (std::vector<uint64_t>::const_iterator citr = vecs.begin();
		citr != vecs.end(); ++citr) {
		uint32_t score = redis_client_t::get_uint32(m_rank_name.c_str(), "score", *citr);
		if (score < m_min_condi) {
			rank_data_mgr_t::del_rank_sort(m_rank_name, *citr);
			continue;
		}

		batch_set.push_back(std::pair<uint64_t, uint32_t>(*citr, score));
	}

	// 起服批量设置
	update_rank_score_batch(batch_set);

	update_rank();

	return true;
}

uint32_t rank_sort_t::get_page_list(uint32_t page, proto::client::ec_rank_reply& reply)
{
	if (m_curr_count == 0) return common::errcode_enum::error_ok;

	if (page > m_curr_count / RANK_PAGE_COUNT) {
		return common::errcode_enum::rank_error_page_max;
	}

	std::vector<uint64_t> list;
	int32_t start = page * RANK_PAGE_COUNT;
	int32_t end = m_curr_count < (page + 1) * RANK_PAGE_COUNT ? m_curr_count - 1 : (page + 1) * RANK_PAGE_COUNT - 1;
	get_range_by_rank(start, end, list);

	for (std::vector<uint64_t>::const_iterator citr = list.begin();
		citr != list.end(); ++citr) {
		peek_data(*citr, reply.add_rank_list());
	}

	return common::errcode_enum::error_ok;
}

bool rank_sort_t::peek_data(uint64_t uid, proto::common::rank_single_data* p_data)
{
	if (!p_data) return false;

	switch (m_rank_type) {
		case common::rank_common::criminal_rank_0: // 全职业通缉 
		case common::rank_common::lv_rank_0: // 等级排行榜 
		case common::rank_common::towers_rank_0: // 千层塔排名榜 
		case common::rank_common::charm_rank_0: // 魅力排行
		case common::rank_common::recruit_rank_0: //招募次数
		case common::rank_common::consume_copper_rank_0: //消耗铜币
		case common::rank_common::consume_golden_rank_0: //消耗金币
		case common::rank_common::combat_power_rank_0:  //战斗力
		case common::rank_common::achievement_rank_0: { // 成就积分榜 
			p_data->set_id(common::string_util_t::uint64_to_string(uid));
			uint32_t rank_index = get_rank_value(uid);
			if (rank_index == 0) { // 未入榜
				p_data->set_index(0);
				p_data->set_key(redis_client_t::get_uint32(m_rank_name.c_str(), "score", uid));
				return false;
			}
			p_data->set_index(rank_index);
			p_data->set_key(redis_client_t::get_uint32(m_rank_name.c_str(), "score", uid));
			p_data->set_name(global_user_data_mgr_t::get_name(uid));
			peek_role_base(uid, p_data->mutable_role_data());
			uint64_t family_id = global_user_data_mgr_t::get_user_family_id(uid);
			if (family_id != 0) {
				peek_family_base(family_id, p_data->mutable_family_data());
			}

			uint32_t country_id = global_user_data_mgr_t::get_user_country_id(uid);
			if (country_id != 0) {
				country_mgr_t::get_country_common(country_id, p_data->mutable_country_data());
			}

			break;
		}
		case common::rank_common::family_rank_0: {
			p_data->set_id(common::string_util_t::uint64_to_string(uid));
			uint32_t rank_index = get_rank_value(uid);
			if (rank_index == 0) { // 未入榜
				p_data->set_index(0);
				p_data->set_key(redis_client_t::get_uint32(m_rank_name.c_str(), "score", uid));
				return false;
			}

			p_data->set_index(rank_index);
			p_data->set_key(redis_client_t::get_uint32(m_rank_name.c_str(), "score", uid));
			
			family_ptr p_family = family_manager_t::get_family(uid);
			if (p_family) {
				proto::common::rank_family_base* p_family_base = p_data->mutable_family_data();
				if (p_family_base) {
					p_family_base->set_name(p_family->get_name());
					p_family_base->set_level(p_family->get_level());
				}

				uint32_t country_id = p_family->get_country_id();
				if (country_id != 0) {
					country_mgr_t::get_country_common(country_id, p_data->mutable_country_data());
				}
			}
			break;
		}
		default:
			log_error("rank[%s] type[%u] peek_data not define", m_rank_name.c_str(), m_rank_type);
			return false;
	}

	return true;
}

void rank_sort_t::peek_role_base(uint64_t uid, proto::common::rank_role_base* p_role_base)
{
	if (!p_role_base) return;

	p_role_base->set_level(global_user_data_mgr_t::get_user_level(uid));
	p_role_base->set_plugin(global_user_data_mgr_t::get_user_plugin(uid));
	p_role_base->set_head_frame_id(global_user_data_mgr_t::get_user_hero_plugin(uid));
}

void rank_sort_t::peek_family_base(uint64_t family_id, proto::common::rank_family_base* p_family_base)
{
	if (!p_family_base) return;

	p_family_base->set_id(common::string_util_t::uint64_to_string(family_id));
	family_ptr p_family = family_manager_t::get_family(family_id);
	if (p_family) {
		p_family_base->set_name(p_family->get_name());
		p_family_base->set_level(p_family->get_level());
	}
}

void rank_sort_t::award()
{
	RankType* p_rank_conf = GET_CONF(RankType, m_rank_type);
	if (NULL == p_rank_conf)
	{
		log_error("NULL == p_rank_mail RankType[%d]", m_rank_type);
		return;
	}
	MailTable* p_rank_mail = GET_CONF(MailTable, p_rank_conf->reward_mail());
	if (NULL == p_rank_mail)
	{
		log_error("NULL == p_rank_mail MailTable[%d]", p_rank_conf->reward_mail());
		return;
	}
	
	std::map<uint32_t, RankReward*> conf_map;
	GET_ALL_CONF(RankReward, conf_map);
	for (auto it : conf_map)
	{
		RankReward* p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		if (p_conf->rank_type() != m_rank_type)
		{
			continue;
		}
		uint32_t min_rank = 0;
		uint32_t max_rank = 0;
		if (1 == p_conf->rank_range().size())
		{
			min_rank = p_conf->rank_range().at(0);
			max_rank = min_rank;
		}
		else if (2 == p_conf->rank_range().size())
		{
			min_rank = p_conf->rank_range().at(0);
			max_rank = p_conf->rank_range().at(1);
		}
		if (0 == min_rank || 0 == max_rank)
		{
			log_error("RankReward[%d] rank_range invalid", p_conf->id());
			continue;
		}
		min_rank -= 1;
		max_rank -= 1;
		std::vector<uint64_t> uids;
		get_range_by_rank(min_rank, max_rank, uids);
		for (uint32_t i = 0; i < uids.size(); ++i)
		{
			std::string content_text;
			StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_rank_mail->content()).c_str(), min_rank+i+1);

			mail_manager_t::send_mail(uids[i], proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_rank_mail->from_name()), common::string_util_t::convert_to_utf8(p_rank_mail->title()), content_text,
				p_conf->item(), proto::common::MAIL_GET_TYPE_RANK);
		}
	}
}
