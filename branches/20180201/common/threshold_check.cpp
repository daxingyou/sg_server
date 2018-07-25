#include "threshold_check.hpp"

#include "config_mgr.h"
#include "tblh/Comprehensive.tbls.h"

bool threshold_check::check_special_value(comprehensive_common::comprehensive_type type, uint32_t value)
{
	auto conf = GET_CONF(Comprehensive, type);
	uint32_t max_value = GET_COMPREHENSIVE_VALUE_1(conf);

	if (max_value > value)
		return false;

	return true;
}
