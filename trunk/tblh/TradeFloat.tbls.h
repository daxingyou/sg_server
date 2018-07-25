#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:TradeFloatBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:16 X64
///           �ֶ�����:4
///           �����ļ�:t���̱�.xlsx trade_float
///           ��������:id
#pragma pack(1)
struct TradeFloatBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	uint32_t                  sell_cnt;                     ///< ������
	uint32_t                  down_prob;                    ///< ���²�������(��ֱ�)
	uint32_t                  stay_prob;                    ///< �����������(��ֱ�)
};
#pragma pack()

struct TradeFloat : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  sell_cnt()                    { return t_sell_cnt; }
	uint32_t                  down_prob()                   { return t_down_prob; }
	uint32_t                  stay_prob()                   { return t_stay_prob; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_sell_cnt;                   ///< ������
	uint32_t                  t_down_prob;                  ///< ���²�������(��ֱ�)
	uint32_t                  t_stay_prob;                  ///< �����������(��ֱ�)

public:
	void reset();
	void fill( const TradeFloatBase& base );
	const char* getClassName() const { return "t���̱�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TradeFloat, TradeFloatBase > TradeFloatManager;

inline void TradeFloat::reset()
{
	this->t_id = 0;
	this->t_sell_cnt = 0;
	this->t_down_prob = 0;
	this->t_stay_prob = 0;
};

inline void TradeFloat::fill( const TradeFloatBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_sell_cnt = base.sell_cnt;
	this->t_down_prob = base.down_prob;
	this->t_stay_prob = base.stay_prob;
};

inline const std::string TradeFloat::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TradeFloat.tbls64";
#else
	return "TradeFloat.tbls32";
#endif
};
