#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:FriendAwardBase
/// @remark   记录大小:24 X32
///           记录大小:24 X64
///           字段数量:6
///           数据文件:h好友交互.xlsx 羁绊奖励
///           排序主键:羁绊等级
#pragma pack(1)
struct FriendAwardBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x012FDD89; ///< I|iiIII
#else
	static const int          FormatCheckCode = 0x012FDD89; ///< I|iiIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return lv; }; // TODO: 请用 羁绊等级 生成唯一键

	uint32_t                  lv;                           ///< 羁绊等级

	int32_t                   need_friends_count;           ///< 需要好友数
	int32_t                   need_friendliness;            ///< 需要好友亲密度
	uint32_t                  give_count;                   ///< 每日赠送次数
	uint32_t                  get_count;                    ///< 每日收取次数
	uint32_t                  award_id;                     ///< 道具奖励掉落包
};
#pragma pack()

struct FriendAward : public Entry
{
	uint32_t                  lv()                          { return t_lv; }

	int32_t                   need_friends_count()          { return t_need_friends_count; }
	int32_t                   need_friendliness()           { return t_need_friendliness; }
	uint32_t                  give_count()                  { return t_give_count; }
	uint32_t                  get_count()                   { return t_get_count; }
	uint32_t                  award_id()                    { return t_award_id; }

private:
	uint32_t                  t_lv;                         ///< 羁绊等级

	int32_t                   t_need_friends_count;         ///< 需要好友数
	int32_t                   t_need_friendliness;          ///< 需要好友亲密度
	uint32_t                  t_give_count;                 ///< 每日赠送次数
	uint32_t                  t_get_count;                  ///< 每日收取次数
	uint32_t                  t_award_id;                   ///< 道具奖励掉落包

public:
	void reset();
	void fill( const FriendAwardBase& base );
	const char* getClassName() const { return "h好友交互"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< FriendAward, FriendAwardBase > FriendAwardManager;

inline void FriendAward::reset()
{
	this->t_lv = 0;
	this->t_need_friends_count = 0;
	this->t_need_friendliness = 0;
	this->t_give_count = 0;
	this->t_get_count = 0;
	this->t_award_id = 0;
};

inline void FriendAward::fill( const FriendAwardBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_lv = base.lv;
	this->t_need_friends_count = base.need_friends_count;
	this->t_need_friendliness = base.need_friendliness;
	this->t_give_count = base.give_count;
	this->t_get_count = base.get_count;
	this->t_award_id = base.award_id;
};

inline const std::string FriendAward::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "FriendAward.tbls64";
#else
	return "FriendAward.tbls32";
#endif
};
