#ifndef __CITY_H__
#define __CITY_H__
#include <vector>
#include <map>
#include "common.pb.h"
#include "object/object_position.hpp"
#include "tblh/MapTable.tbls.h"
#include "macros.hpp"
#include <boost/system/error_code.hpp>
#include <boost/asio/deadline_timer.hpp>

#define HP_CHANGE_GATE    1

enum city_gate_state
{
	city_gate_state_none = 0,
	city_gate_state_invincible = 1,
	city_gate_state_destroyed = 2,
	city_gate_state_attacked_by_user = 3,
	city_gate_state_attacked_by_vihiche = 4,
};

enum city_type
{
	city_type_imperial = 1,
	city_type_king = 2,
	city_type_normal = 3,
	city_type_assemble = 4,
	city_type_attacker_assemble = 5,
	city_type_near_king = 6,
};


class monster_t;
class city_t : public boost::enable_shared_from_this<city_t>
{
public:
	city_t(uint32_t city_id, uint32_t country_id);
	~city_t() {}

	void init();
	uint32_t get_id() { return m_id; }
	uint32_t get_capture_country() { return m_capture_country_id; }
	
    // -------------------ó��(����)--------------------------------------
	// ���ٶ�
	void		set_prosperity_value(uint32_t prosperity_value);
	uint32_t	get_prosperity_value() { return m_prosperity_value; }

	///ռ�ж�
	uint32_t get_hold_value() { return m_hold_value; }
	void	 set_hold_value(uint32_t hold_value);

	//���ٶȵȼ�
	void		set_prosperity_level(uint32_t prosperity_level);
	uint32_t	get_prosperity_level() { return m_prosperity_level; }
	
private:
	// proto::common::city_state cur_state;			/// 1 �޵� 2�Ѵݻ� 3ֻ�ܱ��˹��� 4ֻ�ܱ�ս������ 5���Ա��˺�ս��ͬʱ����
	uint32_t m_peer_country_id = 0;					/// ���ҹ���ս�Ĺ���
	uint32_t m_capture_country_id = 0;				/// ռ��óǹ���id
	uint32_t m_id = 0;								/// ����ID
	uint32_t m_prosperity_value = 0;				/// ���ٶ�
	uint32_t m_prosperity_level = 1;				/// ���ٶȵȼ�
	uint32_t m_hold_value = 0;						/// ռ�ж�
	uint32_t m_country_id = 0;						/// ����id
};
typedef boost::shared_ptr<city_t> city_ptr;
#endif
