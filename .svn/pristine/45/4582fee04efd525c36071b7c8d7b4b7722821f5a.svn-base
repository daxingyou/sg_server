#ifndef __XNN_COMMON_UTIL_RES_DEKEY_HASH_H__
#define __XNN_COMMON_UTIL_RES_DEKEY_HASH_H__

#include <map>
#include <string>
#include "data_loader.h"
#include "data_exception.h"

namespace Utils
{
    /**
     * @brief 支持两个Key多级资源Hash.
     */
    template <class Key, class LoadMetaUnit, class SaveMetaUnit>
    class DataDekeyHashFull : public DataLoader<Key, LoadMetaUnit, SaveMetaUnit>
    {
    public:
        const SaveMetaUnit& GetUnit(const Key& key1, int key2, bool b = false) const
        {
            auto ret = _dataUnits.find(key1);
            if (ret == _dataUnits.end())
            {
                const std::string& dataName = GetName();
                throw UTILS_DEKEY_DATA_EXCEPTION(dataName, key1, key2);
            }

            auto ret2 = ret->second.find(key2);
            if (ret2 == ret->second.end())
            {
                const std::string& dataName = GetName();
                throw UTILS_DEKEY_DATA_EXCEPTION(dataName, key1, key2);
            }

            return ret2->second;
        }
    protected:
        bool InsertUnit(const Key& key, int key2, const SaveMetaUnit& metaUnit)
        {
            auto it = _dataUnits.find(key);
            if (it != _dataUnits.end())
                it->second.insert(std::make_pair(key2, metaUnit));
            else
            {
                DataHashMap dataHashMap;
                dataHashMap.insert(std::make_pair(key2, metaUnit));
                _dataUnits.insert(std::make_pair(key, dataHashMap));
            }

            return true;
        }

        // 获取第2级的key
        virtual int OnGetKey2(const SaveMetaUnit& metaUnit) const = 0;
    private:
        typedef std::map<int, SaveMetaUnit> DataHashMap;
        typedef std::map<Key, DataHashMap> DataDeKeyHashMap;
        DataDeKeyHashMap _dataUnits;
    };

    template <class LoadMetaUnit, class SaveMetaUnit>
    class DataDekeyStrHashFull : public DataDekeyHashFull<std::string, LoadMetaUnit, SaveMetaUnit>
    {
    protected:
        virtual bool OnGetUnit(const SaveMetaUnit& metaUnit)
        {
            std::string key = GetStrKey(metaUnit);
            int key2 = OnGetKey2(metaUnit);
            return InsertUnit(key, key2, metaUnit);
        }
    };

    template <class LoadMetaUnit, class SaveMetaUnit>
    class DataDekeyIntHashFull : public DataDekeyHashFull<int, LoadMetaUnit, SaveMetaUnit>
    {
    protected:
        virtual bool OnGetUnit(const SaveMetaUnit& metaUnit)
        {
            int key = GetStrKey(metaUnit);
            int key2 = OnGetKey2(metaUnit);
            return InsertUnit(key, key2, metaUnit);
        }
    };

    template <class LoadMetaUnit>
    class DataDekeyHash : public DataDekeyStrHashFull<LoadMetaUnit, LoadMetaUnit>
    {};

    template <class LoadMetaUnit>
    class DataDekeyInt : public DataDekeyIntHashFull<LoadMetaUnit, LoadMetaUnit>
    {};
}

#endif 