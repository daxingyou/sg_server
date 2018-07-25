#ifndef __XNN_UTILS_DATA_MULTIHASH_H__
#define __XNN_UTILS_DATA_MULTIHASH_H__

#include <map>
#include <string>
#include <vector>
#include "data_loader.h"

namespace Utils
{
    /**
     * @brief 一个key对应多个Unit.
     */
    template <class Key, class LoadMetaUnit, class SaveMetaUnit>
    class DataMultiHashFull : public DataLoader<Key, LoadMetaUnit, SaveMetaUnit>
    {
    public:
        const std::vector<SaveMetaUnit>& GetUnit(const Key& key) const
        {
            typename DataHashMap::const_iterator ret = _dataUnits.find(key);
            if (ret != _dataUnits.end())
                return ret->second;
            return _emptyDataUnit;
        }

        const std::map<Key, std::vector<SaveMetaUnit> >& GetUnits() const
        { return _dataUnits; }
    protected:
        bool InsertUnit(const Key& key, const SaveMetaUnit& metaUnit)
        {
            auto it = _dataUnits.find(key);
            if (it != _dataUnits.end())
            {
                it->second.push_back(metaUnit);
            }
            else
            {
                DataUnitVec unitVec;
                unitVec.push_back(metaUnit);
                _dataUnits.insert(std::make_pair(key, unitVec));
            }
            return true;
        }
    private:
        typedef std::vector<SaveMetaUnit> DataUnitVec;
        typedef std::map<Key, DataUnitVec> DataHashMap;
        DataHashMap _dataUnits;
        std::vector<SaveMetaUnit> _emptyDataUnit;
    };

    template <class LoadMetaUnit, class SaveMetaUnit>
    class DataMultiStrHashFull : public DataMultiHashFull<std::string, LoadMetaUnit, SaveMetaUnit>
    {
    protected:
        virtual bool OnGetUnit(const SaveMetaUnit& metaUnit, const LoadMetaUnit& loadUnit)
        {
            std::string key = this->GetStrKey(loadUnit);
            return this->InsertUnit(key, metaUnit);
        }
    };

    template <class LoadMetaUnit, class SaveMetaUnit>
    class DataMultiIntHashFull : public DataMultiHashFull<int, LoadMetaUnit, SaveMetaUnit>
    {
    protected:
        virtual bool OnGetUnit(const SaveMetaUnit& metaUnit, const LoadMetaUnit& loadUnit)
        {
            int key = this->GetIntKey(loadUnit);
            return this->InsertUnit(key, metaUnit);
        }
    };

    template <class LoadMetaUnit>
    class DataMultiHash : public DataMultiStrHashFull<LoadMetaUnit, LoadMetaUnit>
    {};

    template <class LoadMetaUnit>
    class DataMultiIntHash : public DataMultiIntHashFull<LoadMetaUnit, LoadMetaUnit>
    {};

	template<typename ConfigType, typename SaveType = ConfigType> bool LoadCnfMulti(const std::string& config_path, std::map<std::string, std::vector<SaveType> >& confs, std::string keyName = "id")
	{
		DataMultiStrHashFull<ConfigType, SaveType> config;
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

	template<typename ConfigType, typename SaveType = ConfigType> bool LoadCnfMulti(const std::string& config_path, std::map<uint32_t, std::vector<SaveType> >& confs, std::string keyName = "id")
	{
		DataMultiIntHashFull<ConfigType, SaveType> config;
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

