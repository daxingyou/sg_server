#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:JumpTableBase
/// @remark   记录大小:36 X32
///           记录大小:44 X64
///           字段数量:9
///           数据文件:t跳转表.xlsx Sheet1
///           排序主键:跳点ID
#pragma pack(1)
struct JumpTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0ADBBD39; ///< I|IVIIVIII
#else
	static const int          FormatCheckCode = 0x0AE5BCD9; ///< I|IvIIvIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 跳点ID 生成唯一键

	uint32_t                  id;                           ///< 跳点ID

	uint32_t                  from_scene;                   ///< 跳点所在场景
	const char*               from_position;                ///< 跳点坐标
	uint32_t                  from_radius;                  ///< 跳点有效半径
	uint32_t                  to_scene;                     ///< 到达场景编号
	const char*               to_position;                  ///< 到达坐标中心
	uint32_t                  to_radius;                    ///< 到达坐标半径
	uint32_t                  move_type;                    ///< 移动类型
	uint32_t                  to_angle;                     ///< 到达摄像机角度
};
#pragma pack()

struct JumpTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  from_scene()                  { return t_from_scene; }
	const std::string&        from_position()               { return t_from_position; }
	uint32_t                  from_radius()                 { return t_from_radius; }
	uint32_t                  to_scene()                    { return t_to_scene; }
	//const std::string&        to_position()                 { return t_to_position; }
	uint32_t                  to_radius()                   { return t_to_radius; }
	uint32_t                  move_type()					{ return t_move_type; }
	uint32_t                  to_angle()                    { return t_to_angle; }

	inline int32_t to_position_x()
	{
		return random_util_t::randBetween(t_to_position_x - (int32_t)to_radius(), t_to_position_x + (int32_t)to_radius());
	}

	inline int32_t to_position_y()
	{
		return random_util_t::randBetween(t_to_position_y - (int32_t)to_radius(), t_to_position_y + (int32_t)to_radius());
	}

private:
	uint32_t                  t_id;                         ///< 跳点ID

	uint32_t                  t_from_scene;                 ///< 跳点所在场景
	std::string               t_from_position;              ///< 跳点坐标
	uint32_t                  t_from_radius;                ///< 跳点有效半径
	uint32_t                  t_to_scene;                   ///< 到达场景编号
	//std::string               t_to_position;                ///< 到达坐标中心
	uint32_t                  t_to_radius;                  ///< 到达坐标半径
	uint32_t                  t_move_type;                  ///< 移动类型

	int32_t t_to_position_x;
	int32_t t_to_position_y;

	uint32_t                  t_to_angle;                   ///< 到达摄像机角度

public:
	void reset();
	void fill( const JumpTableBase& base );
	const char* getClassName() const { return "t跳转表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< JumpTable, JumpTableBase > JumpTableManager;

inline void JumpTable::reset()
{
	this->t_id = 0;
	this->t_from_scene = 0;
	this->t_from_position.clear();
	this->t_from_radius = 0;
	this->t_to_scene = 0;
	//this->t_to_position.clear();
	this->t_to_radius = 0;
	this->t_move_type = 0;
	this->t_to_angle = 0;

	this->t_to_position_x = 0;
	this->t_to_position_y = 0;
};

inline void JumpTable::fill( const JumpTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_from_scene = base.from_scene;
	if( base.from_position == NULL ) this->t_from_position.clear(); else this->t_from_position.assign( base.from_position );
	this->t_from_radius = base.from_radius;
	this->t_to_scene = base.to_scene;
	this->t_to_radius = base.to_radius;
	this->t_move_type = base.move_type;
	this->t_to_angle = base.to_angle;

	if (base.to_position)
	{
		std::vector<int32_t> pos_v;
		common::string_util_t::split<int32_t>(std::string(base.to_position), pos_v, "$");
		if (pos_v.size() == 2)
		{
			t_to_position_x = pos_v[0];
			t_to_position_y = pos_v[1];
		}
	}
};

inline const std::string JumpTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "JumpTable.tbls64";
#else
	return "JumpTable.tbls32";
#endif
};
