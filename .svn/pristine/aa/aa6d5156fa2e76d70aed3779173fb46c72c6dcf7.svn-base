#ifndef __XNN_UTILS_DATA_HASH_H__
#define __XNN_UTILS_DATA_HASH_H__

#include <map>
#include <string>
#include "data_loader.h"
#include "data_exception.h"
#include <sstream>
#ifdef WIN32
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif
namespace Utils
{
    template <class Key, class LoadMetaUnit, class SaveMetaUnit>
    class DataHashFull : public DataLoader<Key, LoadMetaUnit, SaveMetaUnit>
    {
    public:
        const SaveMetaUnit& GetUnit(const Key& key, bool needLog = true) const
        {
            auto ret = m_dataUnits.find(key);
            if (ret == m_dataUnits.end())
            {
                const std::string& dataName = this->GetName();
                std::ostringstream oss;
                oss << key;
                throw UTILS_DATA_EXCEPTION(dataName, oss.str());
            }
            return ret->second;
        }

        const std::map<Key, SaveMetaUnit>& GetUnits() const { return m_dataUnits; }
    protected:
        bool InsertUnit(const Key& key, const SaveMetaUnit& metaUnit)
        {
            auto ret = m_dataUnits.insert(std::make_pair(key, metaUnit));
            if (!ret.second)
            {
                std::cerr << this->GetName() << "'s key[" << key << "] is existed!\n";
                return false;
            }

            return true;
        }
    private:
        typedef std::map<Key, SaveMetaUnit> DataUnits;
        DataUnits m_dataUnits;
    };

    template <class LoadMetaUnit, class SaveMetaUnit>
    class DataStrHashFull : public DataHashFull<std::string, LoadMetaUnit, SaveMetaUnit>
    {
    protected:
        virtual bool OnGetUnit(const SaveMetaUnit& metaUnit, const LoadMetaUnit& loadUnit)
        {
            std::string key = this->GetStrKey( loadUnit);
            return this->InsertUnit(key, metaUnit);
        }
    };

    template <class LoadMetaUnit, class SaveMetaUnit>
    class DataIntHashFull : public DataHashFull<int, LoadMetaUnit, SaveMetaUnit>
    {
    protected:
        virtual bool OnGetUnit(const SaveMetaUnit& metaUnit, const LoadMetaUnit& loadUnit)
        {
            int key = this->GetIntKey(loadUnit);
            return this->InsertUnit(key, metaUnit);
        }
    };

    template <class LoadMetaUnit>
    class DataHash : public DataStrHashFull<LoadMetaUnit, LoadMetaUnit>
    {};

    template <class LoadMetaUnit>
    class DataIntHash : public DataIntHashFull<LoadMetaUnit, LoadMetaUnit>
    {};

	template<typename ConfigType, typename SaveType = ConfigType> bool LoadCnf(const std::string& config_path, std::map<std::string, SaveType>& confs, std::string keyName = "id")
	{
		DataStrHashFull<ConfigType, SaveType> config;
		config.SetKeyName(keyName);
		std::stringstream ssConfigPath;
		std::string configPath = "";
		ssConfigPath << config_path << "/tbl/";
		ssConfigPath >> configPath;
		int ret = access(configPath.c_str(), F_OK);
		if (ret != 0)
		{
			configPath = "../config/tbl/";
			ret = access(configPath.c_str(), F_OK);
			if (ret != 0)
			{
				log_error("Can not find config path %s", configPath.c_str());
				return false;
			}
		}
		if (config.Load(configPath, ""))
		{
			auto units = config.GetUnits();
			confs.insert(units.begin(), units.end());
			return true;
		}

		return false;
	}

	template<typename ConfigType, typename SaveType = ConfigType> bool LoadCnf(const std::string& config_path, std::map<uint32_t, SaveType>& confs, std::string keyName = "id")
	{
		DataIntHashFull<ConfigType, SaveType> config;
		config.SetKeyName(keyName);
		std::stringstream ssConfigPath;
		std::string configPath = "";
		ssConfigPath << config_path << "/tbl/";
		ssConfigPath >> configPath;
		int ret = access(configPath.c_str(), F_OK);
		if (ret != 0)
		{
			configPath = "../../config/tbl/";
			ret = access(configPath.c_str(), F_OK);
			if (ret != 0)
			{
				log_error("Can not find config path %s", configPath.c_str());
				return false;
			}
		}
		if (config.Load(configPath, ""))
		{
			auto units = config.GetUnits();
			confs.insert(units.begin(), units.end());
			return true;
		}

		return false;
	}
}

#endif // end #ifndef __XNN_UTIL_RES_HOLDER_H__

