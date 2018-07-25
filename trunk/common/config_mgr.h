#ifndef __CONF_MGR_H__
#define __CONF_MGR_H__
#include <map>
#include "xls_reader/data_hash.h"
#include "xls_reader/data_multi_hash.h"
#include "utility.hpp"
#include "tblh/comprehensive_common.hpp"

USING_NS_COMMON;


template<typename ConfigType, typename KeyType, bool isMulti = false, typename SaveType =ConfigType>
class ConfMgr
{
public:
	static ConfMgr<ConfigType, KeyType, isMulti, SaveType>* GetSingletonPtr();
	static void DestructSingleton();
	static ConfMgr<ConfigType, KeyType, isMulti, SaveType> & GetSingleton();

public:
	SaveType* FindCnf(KeyType Id);
	std::vector<SaveType>* FindCnfMulti(KeyType Id);
	bool Init(const std::string& config_path, std::string keyName = "id");
	const std::map<KeyType, SaveType>& GetAllConfs() { return m_confs; }
	const std::map<KeyType, std::vector<SaveType> >& GetAllConfsMulti() { return m_confs_multi; }
private:
	std::map<KeyType, SaveType> m_confs;
	std::map<KeyType, std::vector<SaveType> > m_confs_multi;
	static ConfMgr<ConfigType, KeyType, isMulti, SaveType>* m_pInstance;
};

template<typename ConfigType, typename KeyType, bool isMulti, typename SaveType > ConfMgr<ConfigType, KeyType, isMulti, SaveType>* ConfMgr<ConfigType, KeyType, isMulti, SaveType>::m_pInstance = NULL;
template<typename ConfigType, typename KeyType, bool isMulti, typename SaveType > ConfMgr<ConfigType, KeyType, isMulti, SaveType>* ConfMgr<ConfigType, KeyType, isMulti, SaveType>::GetSingletonPtr()
{
	if (!m_pInstance)
	{
		m_pInstance = new ConfMgr<ConfigType, KeyType, isMulti, SaveType>();
	}
	return m_pInstance;
}

template<typename ConfigType, typename KeyType, bool isMulti, typename SaveType > ConfMgr<ConfigType, KeyType, isMulti, SaveType> & ConfMgr<ConfigType, KeyType, isMulti, SaveType>::GetSingleton()
{
	if (!m_pInstance)
	{
		m_pInstance = new ConfMgr<ConfigType, KeyType, isMulti, SaveType>();
	}
	return *m_pInstance;
}

template<typename ConfigType, typename KeyType, bool isMulti, typename SaveType> void ConfMgr<ConfigType, KeyType, isMulti, SaveType>::DestructSingleton()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

template<typename ConfigType, typename KeyType, bool isMulti, typename SaveType> bool ConfMgr<ConfigType, KeyType, isMulti, SaveType>::Init(const std::string& config_path, std::string keyName)
{
	m_confs.clear();
	m_confs_multi.clear();
	if (!isMulti)
	{
		if (Utils::LoadCnf<ConfigType, SaveType>(config_path, m_confs, keyName))
		{
			return true;
		}
	}
	else
	{
		if (Utils::LoadCnfMulti<ConfigType, SaveType>(config_path, m_confs_multi, keyName))
		{
			return true;
		}
	}


	return false;
}

template<typename ConfigType, typename KeyType, bool isMulti, typename SaveType > SaveType* ConfMgr<ConfigType, KeyType, isMulti, SaveType>::FindCnf(KeyType Id)
{
	auto ptr = m_confs.find(Id);
	if (ptr != m_confs.end())
	{
		return &ptr->second;
	}
	return NULL;
}

template<typename ConfigType, typename KeyType, bool isMulti, typename SaveType> std::vector<SaveType>* ConfMgr<ConfigType, KeyType, isMulti, SaveType>::FindCnfMulti(KeyType Id)
{
	auto ptr = m_confs_multi.find(Id);
	if (ptr != m_confs_multi.end())
	{
		return &ptr->second;
	}
	return NULL;
}

int32_t  GetErrorCodeByStr(const std::string& errorCodeStr);

#define GET_COMPREHENSIVE_VALUE_1(conf) \
	(NULL != conf && !conf->parameter1().empty()) ? (uint32_t)(atoi(conf->parameter1().c_str())) : 0;

#define GET_COMPREHENSIVE_VALUE_2(conf) \
	(NULL != conf && !conf->parameter2().empty()) ? (uint32_t)(atoi(conf->parameter2().c_str())) : 0;

#define GET_COMPREHENSIVE_VALUE_3(conf) \
	(NULL != conf && !conf->parameter3().empty()) ? (uint32_t)(atoi(conf->parameter3().c_str())) : 0;

#define GET_CONFIG(table_name, table_id)	\
	ConfMgr<Hades::Data::table_name, uint32_t>::GetSingleton().FindCnf(table_id)

#define GET_CONFIG_SPECIAL(table_name, table_id, is_multi, second_struct)	\
	ConfMgr<Hades::Data::table_name, uint32_t, is_multi, second_struct>::GetSingleton().FindCnf(table_id)

#define GET_CONFIG_MULTI(table_name, table_id, is_multi, second_struct)	\
	ConfMgr<Hades::Data::table_name, uint32_t, is_multi, second_struct>::GetSingleton().FindCnfMulti(table_id)

#define GET_CONF(table_name, table_id)	\
	table_name##Manager::getInstancePtr()->getDataByID(table_id)

#define GET_ALL_CONF(table_name, container)	\
	table_name##Manager::getInstancePtr()->getAllData(container)


#endif // !__CONF_MGR_H__
