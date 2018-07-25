#ifndef __XNN_COMMON_UTIL_DATA_VECTOR_H__
#define __XNN_COMMON_UTIL_DATA_VECTOR_H__

#include <string>
#include <vector>
#include "data_loader.h"
#include "data_hash.h"

namespace Utils
{
    // DataVector
    template <class MetaUnit>
    class DataVector : public DataLoader<int, MetaUnit, MetaUnit>
    {
    public:
        const std::vector<MetaUnit>& GetUnits() const { return m_units; }
    protected:
        virtual bool OnGetUnit(const MetaUnit& metaUnit)
        {
            m_units.push_back(metaUnit);
            return true;
        }
	private:
        std::vector<MetaUnit> m_units;
    };

    /*template <class MetaUnit>
    class DataVector : public DataVectorFull<std::string, MetaUnit>
    {};

    template <class MetaUnit>
    class DataIntVector : public DataVectorFull<int, MetaUnit>
    {};*/
}

#endif // end #ifndef __XNN_UTIL_RES_HOLDER_H__

