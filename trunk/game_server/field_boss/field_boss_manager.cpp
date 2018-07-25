#include "field_boss_manager.hpp"
#include "field_boss.hpp"
#include "scene/scene_manager.hpp"
#include "scene/scene.hpp"
#include "monster/monster.hpp"
#include "monster/monster_manager.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "fight/fight_manager.hpp"
#include "user_troop/user_troop_mgr.hpp"
#include "tblh/errcode_enum.hpp"
std::map<uint32_t, std::vector<uint32_t>> field_boss_manager_t::m_field_boss_persion_reward;
std::map<uint32_t, std::vector<uint32_t>> field_boss_manager_t::m_field_boss_family_reward;

std::vector<uint32_t> field_boss_manager_t::m_empty_boss_reward;

bool field_boss_manager_t::summon_field_boss(uint32_t id, uint64_t scene_id, uint32_t monster_id, uint64_t object_id, uint32_t x, uint32_t y, uint32_t dir)
{
	const scene_ptr& p_scene = scene_manager_t::find_scene(scene_id);
	if (NULL == p_scene) {
		log_error("scene[%lu] can not find, when summon field boss[%u]", scene_id, id);
		return false;
	}

	monster_ptr p_monster = p_scene->summon_monster<field_boss_t>(monster_id, x, y, dir, object_id);
	if (NULL == p_monster) {
		log_error("p_field_boss summon_monster null error");
		return false;
	}

	field_boss_ptr p_field_boss = p_monster->get_field_boss();
	if (p_field_boss == NULL) {
		log_error("summon filed boss type error");
		return false;
	}

	// 设置配置表id
	p_field_boss->set_id(id);

	BOSS_LOG("scene[%lu] summon one field_boss[%u:%u], object id[%lu]", scene_id, id, monster_id, object_id);

	return true;
}

void field_boss_manager_t::delete_field_boss(uint64_t object_id, proto::server::EM_FIELD_MODE mode)
{
	monster_ptr p_monster = monster_manager_t::find_monster(object_id);
	if (NULL == p_monster) {
		//BOSS_LOG("field_boss[%lu] not find, maybe is killed", object_id);
		return;
	}

	field_boss_ptr p_field_boss = p_monster->get_field_boss();
	if (p_field_boss == NULL) {
		log_error("monster[%lu] not field boss error", object_id);
		return;
	}

	// 已经死亡，说明已经处理了结算
	if (!p_field_boss->is_die()) {
		p_field_boss->field_boss_end(mode);
	}

	BOSS_LOG("field_boss[%lu] del by center, mode[%u]", object_id, mode);
}

bool field_boss_manager_t::role_attack_field_boss(const role_ptr& p_role, uint64_t object_id, proto::client::gc_field_boss_attack_reply& reply)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return false;
	}

	if (p_role->is_troop_member_not_leave()) {
		log_error("p_role[%lu] is troop member, can't fight", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::user_troop_can_not_fight);
		return false;
	}

	monster_ptr p_monster = monster_manager_t::find_monster(object_id);
	if (NULL == p_monster) {
		BOSS_LOG("role[%lu] request to fight field boss, can not find field boss[%lu]", p_role->get_uid(), object_id);
		reply.set_reply_code(common::errcode_enum::notice_fight_monster_not_exist);
		return false;
	}

	field_boss_ptr p_boss = p_monster->get_field_boss();
	if (NULL == p_boss) {
		log_error("object[%lu] is not field boss", object_id);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return false;
	}

	if (p_boss->is_die()) {
		log_error("field boss[%lu] is die", object_id);
		reply.set_reply_code(common::errcode_enum::notice_field_boss_die);
		return false;
	}

	if (p_role->get_troop_id() != 0) {
		troop_ptr p_troop = game_troop_mgr_t::get_troop(p_role->get_troop_id());
		if (p_troop != NULL) {
			const troop_t::troop_member_vec& member_vec = p_troop->get_members();
			for (troop_t::troop_member_vec::const_iterator citr = member_vec.begin();
				citr != member_vec.end(); ++citr) {
				const troop_t::troop_member_ptr& p_troop_member = *citr;
				if (p_troop_member != NULL && p_troop_member->get_object_type() == proto::common::SCENEOBJECT_USER && p_troop_member->get_state() == proto::common::troop_state_normal) {
					uint32_t cd_time = p_boss->get_fight_cd_time(p_troop_member->get_uid());
					if (cd_time > 0) {
						BOSS_LOG("troop member[%lu] fight boss cd time[%u]", p_troop_member->get_uid(), cd_time);
						reply.set_reply_code(common::errcode_enum::notice_field_boss_cd);
						reply.set_cool_time(cd_time);
						if (p_troop_member->get_uid() != p_role->get_uid())
							reply.set_cool_name(p_troop_member->get_name());
						return false;
					}
				}
			}
		}
	} else {
		uint32_t cd_time = p_boss->get_fight_cd_time(p_role->get_uid());
		if (cd_time > 0) {
			BOSS_LOG("role[%lu] fight boss cd time[%u]", p_role->get_uid(), cd_time);
			reply.set_reply_code(common::errcode_enum::notice_field_boss_cd);
			reply.set_cool_time(cd_time);
			//reply.set_cool_name(p_role->get_name());
			return false;
		}
	}

	proto::common::fight_param fp;
	fp.set_type(proto::common::fight_type_field_boss);
	fp.mutable_light()->set_monster_id(p_boss->get_monster_id());
	fp.mutable_light()->set_object_id(common::string_util_t::uint64_to_string(object_id));
	if (!p_boss->get_hero_state_data(fp.mutable_start()->mutable_enemy())) {
		log_error("role[%lu] get boss[%u] hero data error", p_role->get_uid(), p_boss->get_monster_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return false;
	}

	uint64_t fight_id = fight_manager_t::fight_pve(p_role, p_boss->get_against_form_id(), &fp);
	if (fight_id == 0) {
		log_error("role[%lu] fight_pve boss[%u] error", p_role->get_uid(), p_boss->get_monster_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return false;
	}

	p_boss->add_combat(fight_id);
	return true;
}

void field_boss_manager_t::fight_role_hurt(uint64_t object_id, uint64_t fight_id,
	const proto::common::fight_camp& self_camp, const proto::common::fight_camp& enemy_camp, 
	const combat_ptr& combat)
{
	monster_ptr p_monster = monster_manager_t::find_monster(object_id);
	if (NULL == p_monster) {
		log_error("can not find p_monster[%lu]", object_id);
		return;
	}

	field_boss_ptr p_boss = p_monster->get_field_boss();
	if (NULL == p_boss) {
		log_error("object[%lu] is not field boss", object_id);
		return;
	}

	p_boss->fight_role_hurt(fight_id, self_camp, enemy_camp, combat);
}

void field_boss_manager_t::role_leave_fight(uint64_t object_id, uint64_t fight_id, const proto::common::fight_camp& self_camp)
{
	monster_ptr p_monster = monster_manager_t::find_monster(object_id);
	if (NULL == p_monster) {
		log_error("can not find p_monster[%lu]", object_id);
		return;
	}

	field_boss_ptr p_boss = p_monster->get_field_boss();
	if (NULL == p_boss) {
		log_error("object[%lu] is not field boss", object_id);
		return;
	}

	p_boss->leave_fight(fight_id, self_camp);
}

void field_boss_manager_t::reward_clear()
{
	m_field_boss_persion_reward.clear();
	m_field_boss_family_reward.clear();
}

void field_boss_manager_t::add_persion_reward(uint32_t monster_tid, uint32_t id)
{
	m_field_boss_persion_reward[monster_tid].push_back(id);
}

const std::vector<uint32_t>& field_boss_manager_t::get_boss_persion_reward(uint32_t monster_tid)
{
	std::map<uint32_t, std::vector<uint32_t>>::const_iterator citr = m_field_boss_persion_reward.find(monster_tid);
	if (citr != m_field_boss_persion_reward.end()) {
		return citr->second;
	}

	log_error("field boss[%u] persion_reward empty", monster_tid);
	return m_empty_boss_reward;
}

void field_boss_manager_t::add_family_reward(uint32_t monster_tid, uint32_t id)
{
	m_field_boss_family_reward[monster_tid].push_back(id);
}

const std::vector<uint32_t>& field_boss_manager_t::get_boss_family_reward(uint32_t monster_tid)
{
	std::map<uint32_t, std::vector<uint32_t>>::const_iterator citr = m_field_boss_family_reward.find(monster_tid);
	if (citr != m_field_boss_family_reward.end()) {
		return citr->second;
	}

	log_error("field boss[%u] family_reward empty", monster_tid);
	return m_empty_boss_reward;
}