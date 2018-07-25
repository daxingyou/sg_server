#ifndef __CENTER_SCENE_COMMON_H__
#define __CENTER_SCENE_COMMON_H__

#include "macros.hpp"

// ��������״̬
enum SceneState : uint32_t
{
	SCENE_STATE_NONE = 0,
	SCENE_STATE_NORMAL,//��������
	SCENE_STATE_UNLOADING,//����ж��
	SCENE_STATE_UNLOADED,//�Ѿ�ж��

	SCENE_STATE_MAX,
};

// ��������

class scene_base
{
protected:
	uint32_t m_state;
	uint32_t m_game_id;		// ����gameid 

public:
	scene_base() : m_state(SCENE_STATE_NONE), m_game_id(0) {};
	virtual ~scene_base() {};

	uint32_t get_scene_state() const { return m_state; }
	virtual void set_scene_state(uint32_t set) { m_state = set; }

	inline uint32_t get_game_id() const { return m_game_id; }
	inline void set_game_id(uint32_t val) { m_game_id = val; }
};

#endif