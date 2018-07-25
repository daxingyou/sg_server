#include "talent.h"

talent_t::talent_t(uint32_t tid, uint32_t level, uint32_t wake)
	: m_tid(tid)
	, m_level(level)
	, m_wake(wake)
{
}

talent_t::~talent_t()
{
}
