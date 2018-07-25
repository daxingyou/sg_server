#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:RechargeBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:16 X64
///           �ֶ�����:4
///           �����ļ�:c��ֵ��.xlsx Sheet1
///           ��������:id
#pragma pack(1)
struct RechargeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	uint32_t                  CNY;                          ///< �����ҽ��
	uint32_t                  money;                        ///< ��ý��
	uint32_t                  gift_money;                   ///< �׳����ͽ��
};
#pragma pack()

struct Recharge : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  CNY()                         { return t_CNY; }
	uint32_t                  money()                       { return t_money; }
	uint32_t                  gift_money()                  { return t_gift_money; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_CNY;                        ///< �����ҽ��
	uint32_t                  t_money;                      ///< ��ý��
	uint32_t                  t_gift_money;                 ///< �׳����ͽ��

public:
	void reset();
	void fill( const RechargeBase& base );
	const char* getClassName() const { return "c��ֵ��"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Recharge, RechargeBase > RechargeManager;

inline void Recharge::reset()
{
	this->t_id = 0;
	this->t_CNY = 0;
	this->t_money = 0;
	this->t_gift_money = 0;
};

inline void Recharge::fill( const RechargeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_CNY = base.CNY;
	this->t_money = base.money;
	this->t_gift_money = base.gift_money;
};

inline const std::string Recharge::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Recharge.tbls64";
#else
	return "Recharge.tbls32";
#endif
};
