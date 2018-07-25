#ifndef SKILL_DATA_HPP
#define SKILL_DATA_HPP

#include "tblh/SkillEffectTable.tbls.h"
#include "protocpp/common.pb.h"

class fight_hero_t;
typedef boost::shared_ptr<fight_hero_t> fight_hero_ptr;

struct skill_data_t
{
public:
	void set_skill_cd(uint32_t cd);
public:
	uint32_t skill_id = 0;
	uint32_t skill_level = 0;				//���ܵȼ�
	uint32_t skill_cd = 0;					//����cd
	proto::common::skill_type type;			//��������
	uint32_t is_use_public_cd = 0;		//�Ƿ�ʹ�ù�������CD
};


//����ԭʼ������� �����滻����ǿ����û��ʹ�úܺ���� buff_element��ǿ������������ͬһ��λ�õļ��ܣ�
//����ʹ�ù���CD ,���߶���CD��ֻ�ܲ���λ�ö�Ӧ��˫CD�ߵķ�ʽ��ʵ����
struct skill_info_t
{
public:
	skill_data_t *get_skill_data_by_skill_id(uint32_t skill_id );
	skill_data_t *get_skill_data_by_turn_level(uint32_t turn_level);

	uint32_t get_skill_cd(uint32_t skill_id );
	uint32_t get_skill_id(uint32_t turn_level);
	uint32_t get_skill_level(uint32_t skill_id );

	void set_skill_cd(uint32_t skill_id,uint32_t cd);
	void add_skill_cd( fight_hero_ptr owner, uint32_t skill_id, uint32_t cd = 99);
	void add_skill_cd_ex(uint32_t skill_id,uint32_t cd);
	void sub_skill_cd(uint32_t skill_id, uint32_t cd);

	void set_pub_skill_cd(uint32_t cd);
public:
	uint32_t m_public_cd = 0;						//����CD
	uint32_t m_skill_level = 0;						//�������ܵȼ�
	std::vector<skill_data_t> m_skill_data_list;	//�����Ӧ-�����б�(0) ����ͨ����
};

#endif
