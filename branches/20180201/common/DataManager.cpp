#include "DataManager.h"

static std::map< std::string, IDTManager* > S_DTMANAGER_MAP;

void addDTManager( const std::string& tblfilename, IDTManager* data )
{
	S_DTMANAGER_MAP[tblfilename] = data;
}

bool reloadDTManager(const std::string& config_path, const std::string& tblfilename )
{
	std::map< std::string, IDTManager* >::iterator it;
	it = S_DTMANAGER_MAP.find( tblfilename );
	if( it == S_DTMANAGER_MAP.end() )
		return false;

	if (NULL == it->second)
	{
		return false;
	}

	return it->second->reload(config_path);
}

bool reloadAllDTManager(const std::string& config_path)
{
	bool suc = true;
	std::map< std::string, IDTManager* >::iterator it;
	it = S_DTMANAGER_MAP.begin();
	for( ; it != S_DTMANAGER_MAP.end(); ++it )
	{
		if (NULL == it->second)
		{
			continue;
		}

		if( it->second->reload(config_path) )
		{
			log_debug( "reload [%s] success", it->first.c_str() );
		}
		else
		{
			log_error( "reload [%s] failed", it->first.c_str() );
			suc = false;
		}
	}
	
	if (suc)
	{
		log_debug("reload all data manager success!");
	}
	else
	{
		log_error("reload all data manager failed!");
	}

	return suc;
}