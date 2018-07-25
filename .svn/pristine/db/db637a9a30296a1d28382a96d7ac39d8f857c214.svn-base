#include "name_manager.hpp"
#include "config/config_manager.hpp"
#include "tblh/PrefixNameTable.tbls.h"
#include "tblh/SurNameTable.tbls.h"
#include "tblh/FemaleNameTable.tbls.h"
#include "tblh/MaleNameTable.tbls.h"
#include "config_mgr.h"

USING_NS_COMMON;

std::string name_manager_t::create_new_name()
{
	std::string new_name;
	if (NULL == PrefixNameTableManager::getInstancePtr())
	{
		log_error("NULL == PrefixNameTableManager::getInstancePtr()");
		return "";
	}
	if (NULL == PrefixNameTableManager::getInstancePtr())
	{
		log_error("NULL == PrefixNameTableManager::getInstancePtr()");
		return "";
	}
	if (NULL == MaleNameTableManager::getInstancePtr())
	{
		log_error("NULL == MaleNameTableManager::getInstancePtr()");
		return "";
	}
	if (NULL == FemaleNameTableManager::getInstancePtr())
	{
		log_error("NULL == FemaleNameTableManager::getInstancePtr()");
		return "";
	}
	int32_t prefix_id = random_util_t::randBetween(1, PrefixNameTableManager::getInstancePtr()->getSize());
	if (prefix_id == -1)
	{
		log_error("PrefixNameTable config rand from 1 to [%d] error", (int32_t)PrefixNameTableManager::getInstancePtr()->getSize());
		return "";
	}

	PrefixNameTable* p_prefix_conf = GET_CONF(PrefixNameTable, prefix_id);
	if (p_prefix_conf == NULL)
	{
		log_error("PrefixNameTable[%u] null error", prefix_id);
		return "";
	}

	new_name.append(p_prefix_conf->name());

	int32_t surname_id = random_util_t::randBetween(1, SurNameTableManager::getInstancePtr()->getSize());
	if (surname_id == -1)
	{
		log_error("SurNameTable config rand from 1 to [%d] error", (int32_t)SurNameTableManager::getInstancePtr()->getSize());
		return "";
	}
	SurNameTable* p_sur_conf = GET_CONF(SurNameTable, surname_id);
	if (p_sur_conf == NULL)
	{
		log_error("SurNameTable[%u] null error", surname_id);
		return "";
	}

	new_name.append(p_sur_conf->name());

	if (rand() % 2)
	{
		int32_t male_id = random_util_t::randBetween(1, MaleNameTableManager::getInstancePtr()->getSize());
		if (male_id == -1)
		{
			log_error("MaleNameTable config rand from 1 to [%d] error", (int32_t)MaleNameTableManager::getInstancePtr()->getSize());
			return "";
		}
		MaleNameTable* p_male_conf = GET_CONF(MaleNameTable, male_id);
		if (p_male_conf == NULL)
		{
			log_error("MaleNameTable[%u] null error", male_id);
			return "";
		}

		new_name.append(p_male_conf->name());
	}
	else
	{
		int32_t female_id = random_util_t::randBetween(1, FemaleNameTableManager::getInstancePtr()->getSize());
		if (female_id == -1)
		{
			log_error("FemaleNameTable config rand from 1 to [%d] error", (int32_t)FemaleNameTableManager::getInstancePtr()->getSize());
			return "";
		}
		FemaleNameTable* p_female_conf = GET_CONF(FemaleNameTable, female_id);
		if (p_female_conf == NULL)
		{
			log_error("FemaleNameTable[%u] null error", female_id);
			return "";
		}

		new_name.append(p_female_conf->name());
	}

	return common::string_util_t::convert_to_utf8(new_name);
}

