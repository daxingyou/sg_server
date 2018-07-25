#ifndef __XNN_COMMON_UTILS_DATA_LOADER_H__
#define __XNN_COMMON_UTILS_DATA_LOADER_H__

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "google/protobuf/message.h"
#include "google/protobuf/descriptor.h"
#include "data_exception.h"
#include "../log.hpp"
#ifndef WIN32
#include <arpa/inet.h>
#endif
using namespace google;

namespace Utils
{
    /**
     * @brief DataLoaderBase作用在于调用者对象可以通过本对象的指针统一管理Load动作
     */
    struct DataLoaderBase
    {
        virtual ~DataLoaderBase() {}
        virtual bool Load(const std::string& fileName, const std::string& binName = std::string()) = 0;
    };

    template <class Key, class LoadMetaUnit, class SaveMetaUnit>
    class DataLoader : public DataLoaderBase
    {
    public:
		DataLoader():keyName("id") {}
        virtual ~DataLoader() {}

        bool Load(const std::string& path, const std::string& binName = std::string())
        {
            LoadMetaUnit loadUnit;
            if (!binName.empty())
                _dataName = binName;
            else
                _dataName = GetBinName(loadUnit);
            std::string dataPath = path + _dataName;
            std::ifstream file(dataPath, std::ios_base::in | std::ios_base::binary);
            if (!file)
            {
                std::cerr << "Open " << dataPath << " failed.\n";
				log_error("load [%s] failed!", dataPath.c_str());
                return false;
            }

            // check md5
            char buffer[65536];
            file.read(buffer, 32);
            std::string md5(buffer, 32);

            std::string metaMD5 = GetMetaMD5(loadUnit);
            if (md5 != metaMD5)
            {
                // 提示一下Meta字段个数，二进制文件字段个数
                std::cerr << _dataName << " meta and bytes isn't matched!\n";
                return false;
            }

            uint16_t pbNum;
            file.read((char*)&pbNum, sizeof(pbNum));
            pbNum = ntohs(pbNum);

            bool sameMeta = (typeid(LoadMetaUnit).name() == typeid(SaveMetaUnit).name());

            for (uint16_t i = 0; i < pbNum; i++)
            {
                uint16_t bufSize;
                file.read((char*)&bufSize, sizeof(bufSize));

                bufSize = ntohs(bufSize);
                file.read(buffer, bufSize);

                loadUnit.ParseFromArray(buffer, bufSize);

                OnPreUnit(loadUnit);

                if (sameMeta)
                {
                    if (!OnGetUnit((const SaveMetaUnit&)loadUnit, loadUnit))
                        return false;

                    OnPostUnit((const SaveMetaUnit&)loadUnit);
                }
                else
                {
                    SaveMetaUnit saveUnit;
                    saveUnit = loadUnit;
                    if (!OnGetUnit(saveUnit, loadUnit))
                        return false;

                    OnPostUnit(saveUnit);
                }
            }

            return true;
        }

        int GetCount() const
        {
            return _count;
        }
        const std::string& GetName() const
        {
            return _dataName;
        }
		void SetKeyName(std::string name) { keyName = name; }
    private:
        static std::string GetBinName(const protobuf::Message& loadUnit)
        {
            const protobuf::Descriptor* desc = loadUnit.GetDescriptor();
            std::string name = desc->name();
            // RoleInit-->role_init.bin
            std::vector<size_t> poss;
            poss.push_back(0);
            for (size_t i = 1; i < name.size(); i++)
            {
                int c = name[i];
                if (c >= 'A' && c <= 'Z')
                    poss.push_back(i);
            }
            poss.push_back(name.size());

            std::vector<std::string> strs;
            for (size_t i = 0; i < poss.size() - 1; i++)
            {
                std::string str = name.substr(poss[i], poss[i + 1] - poss[i]);
                std::transform(str.begin(), str.begin() + 1, str.begin(), ::tolower);
                strs.push_back(str);
            }

            std::string binName;
            for (auto it = strs.begin(); it != strs.end(); ++it)
            {
                if (!binName.empty())
                    binName += "_";
                binName += *it;
            }

            return binName + ".bytes";
        }

        static std::string GetMetaMD5(const protobuf::Message& loadUnit)
        {
            // data.RoleInit--->data.MD5OfRoleInit
            const protobuf::Descriptor* desc = loadUnit.GetDescriptor();
            std::string fullName = desc->full_name();
            int pos = fullName.rfind(".");
            fullName = fullName.substr(0, ++pos);

            std::string md5MsgName = "MD5Of";
            md5MsgName += desc->name();
            md5MsgName = fullName + md5MsgName;

            desc = protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(md5MsgName);
            const protobuf::Message* prototype = protobuf::MessageFactory::generated_factory()->GetPrototype(desc);
            if (!prototype)
                return "";
            protobuf::Message* md5Msg = prototype->New();
            if (!md5Msg)
                return "";
            const protobuf::Reflection* ref = md5Msg->GetReflection();
            if (!ref)
                return "";
            std::string str = ref->GetString(*md5Msg, desc->field(0));
            delete md5Msg;
            md5Msg = NULL;
            return str;
        }
    private:
        virtual void OnPreUnit(LoadMetaUnit& loadUnit) {}
        virtual void OnPostUnit(const SaveMetaUnit& saveUnit) {}
        virtual std::string GetKeyName() const { return keyName; }
    protected:
        std::string GetStrKey(const LoadMetaUnit& loadUnit) const
        {
            std::pair<const protobuf::Reflection*, const protobuf::FieldDescriptor*> pair = GetKey(loadUnit);
            return pair.first->GetString(loadUnit, pair.second);
        }

        int GetIntKey(const LoadMetaUnit& loadUnit) const
        {
            std::pair<const protobuf::Reflection*, const protobuf::FieldDescriptor*> pair = GetKey(loadUnit);
            return pair.first->GetInt32(loadUnit, pair.second);
        }

        // 逻辑模块要自己处理MetaUnit, 重载
        virtual bool OnGetUnit(const SaveMetaUnit& saveUnit, const LoadMetaUnit& loadUnit) = 0;
    private:
        std::pair<const protobuf::Reflection*, const protobuf::FieldDescriptor*> GetKey(
            const LoadMetaUnit& loadUnit) const
        {
            const protobuf::Reflection* ref = loadUnit.GetReflection();
            const protobuf::Descriptor* desc = loadUnit.GetDescriptor();
            const protobuf::FieldDescriptor* fDesc = desc->FindFieldByName(GetKeyName());
            if (!fDesc)
                throw UTILS_DATA_EXCEPTION(_dataName, "-1");
            return std::make_pair(ref, fDesc);
        }
    private:
        int _count = 0;
        std::string _dataName;
        Key _emptyKey;
		std::string keyName;
    };

}

#endif // end #ifndef __XNN_UTIL_RES_HOLDER_H__

