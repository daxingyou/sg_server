#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:FriendlinessBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:20 X64
///           �ֶ�����:5
///           �����ļ�:h���ѽ���.xlsx ���ܶ�
///           ��������:id
#pragma pack(1)
struct FriendlinessBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0512FDD9; ///< I|iiII
#else
	static const int          FormatCheckCode = 0x0512FDD9; ///< I|iiII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	int32_t                   friendliness_range;           ///< ���ܶ�����
	int32_t                   friend_point;                 ///< ��ȡ�������ֵ
	uint32_t                  extra_exp;                    ///< ����ӳ�
	uint32_t                  extra_gold;                   ///< ��Ҽӳ�
};
#pragma pack()

struct Friendliness : public Entry
{
	uint32_t                  id()                          { return t_id; }

	int32_t                   friendliness_range()          { return t_friendliness_range; }
	int32_t                   friend_point()                { return t_friend_point; }
	uint32_t                  extra_exp()                   { return t_extra_exp; }
	uint32_t                  extra_gold()                  { return t_extra_gold; }

private:
	uint32_t                  t_id;                         ///< id

	int32_t                   t_friendliness_range;         ///< ���ܶ�����
	int32_t                   t_friend_point;               ///< ��ȡ�������ֵ
	uint32_t                  t_extra_exp;                  ///< ����ӳ�
	uint32_t                  t_extra_gold;                 ///< ��Ҽӳ�

public:
	void reset();
	void fill( const FriendlinessBase& base );
	const char* getClassName() const { return "h���ѽ���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Friendliness, FriendlinessBase > FriendlinessManager;

inline void Friendliness::reset()
{
	this->t_id = 0;
	this->t_friendliness_range = 0;
	this->t_friend_point = 0;
	this->t_extra_exp = 0;
	this->t_extra_gold = 0;
};

inline void Friendliness::fill( const FriendlinessBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_friendliness_range = base.friendliness_range;
	this->t_friend_point = base.friend_point;
	this->t_extra_exp = base.extra_exp;
	this->t_extra_gold = base.extra_gold;
};

inline const std::string Friendliness::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Friendliness.tbls64";
#else
	return "Friendliness.tbls32";
#endif
};
