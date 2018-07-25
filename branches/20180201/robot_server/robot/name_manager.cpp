#include "name_manager.hpp"
#include "config/config_manager.hpp"
#include "tblh/PrefixNameTable.tbls.h"
#include "tblh/SurNameTable.tbls.h"
#include "tblh/FemaleNameTable.tbls.h"
#include "tblh/MaleNameTable.tbls.h"
#include "config_mgr.h"

USING_NS_COMMON;

bool name_manager_t::create_new_name(uint32_t jod, std::string& new_name)
{
	new_name.clear();
	uint32_t max_size = 0;
	if (NULL != PrefixNameTableManager::getInstancePtr())
	{
		max_size = PrefixNameTableManager::getInstancePtr()->getSize();
	}

	int32_t prefix_id = random_util_t::randBetween(1, max_size);
	if (prefix_id == -1)
	{
		log_error("PrefixNameTable config rand from 1 to [%d] error", (int32_t)max_size);
		return false;
	}
	PrefixNameTable* p_prefix_conf = GET_CONF(PrefixNameTable, prefix_id);
	if (p_prefix_conf == NULL)
	{
		log_error("PrefixNameTable[%u] null error", prefix_id);
		return false;
	}

	new_name.append(common::string_util_t::convert_to_utf8(p_prefix_conf->name()));

	max_size = 0;
	if (NULL != SurNameTableManager::getInstancePtr())
	{
		max_size = SurNameTableManager::getInstancePtr()->getSize();
	}

	int32_t surname_id = random_util_t::randBetween(1, max_size);
	if (surname_id == -1)
	{
		log_error("SurNameTable config rand from 1 to [%d] error", (int32_t)max_size);
		return "";
	}
	SurNameTable* p_sur_conf = GET_CONF(SurNameTable, surname_id);
	if (p_sur_conf == NULL)
	{
		log_error("SurNameTable[%u] null error", surname_id);
		return false;
	}

	new_name.append(common::string_util_t::convert_to_utf8(p_sur_conf->name()));

	if (jod == 1 || jod == 4)
	{
		max_size = 0;
		if (NULL != MaleNameTableManager::getInstancePtr())
		{
			max_size = MaleNameTableManager::getInstancePtr()->getSize();
		}

		int32_t male_id = random_util_t::randBetween(1, max_size);
		if (male_id == -1)
		{
			log_error("MaleNameTable config rand from 1 to [%d] error", (int32_t)max_size);
			return "";
		}
		MaleNameTable* p_male_conf = GET_CONF(MaleNameTable, male_id);
		if (p_male_conf == NULL)
		{
			log_error("MaleNameTable[%u] null error", male_id);
			return false;
		}

		new_name.append(common::string_util_t::convert_to_utf8(p_male_conf->name()));
	}
	else
	{
		uint32_t max_size = 0;
		if (NULL != FemaleNameTableManager::getInstancePtr())
		{
			max_size = FemaleNameTableManager::getInstancePtr()->getSize();
		}

		int32_t female_id = random_util_t::randBetween(1, max_size);
		if (female_id == -1)
		{
			log_error("FemaleNameTable config rand from 1 to [%d] error", (int32_t)max_size);
			return "";
		}
		FemaleNameTable* p_female_conf = GET_CONF(FemaleNameTable, female_id);
		if (p_female_conf == NULL)
		{
			log_error("FemaleNameTable[%u] null error", female_id);
			return false;
		}

		new_name.append(common::string_util_t::convert_to_utf8(p_female_conf->name()));
	}

	return true;
}

