#pragma once

#include "log.hpp"
#include "EntryManager.h"
#include "Singleton.h"
#include "Entry.h"
#include "TblxFileHead.h"
#include "StringTool.h"
#include "random_util.hpp"
#include "utility.hpp"

#ifndef _MSC_VER
#define _PLATFORM_X64
#endif 
USING_NS_COMMON;

struct IDTManager
{
	virtual bool reload( const std::string& config_path ) = 0;
	virtual bool load( const std::string& filename ) = 0;
};


extern void addDTManager( const std::string& tblfilename, IDTManager* data );
extern bool reloadDTManager(const std::string& config_path, const std::string& tblfilename );
extern bool reloadAllDTManager( const std::string& config_path );

template < class TEntry, class TBase >
class DataTableManager 
	: public EntryManager< EntryID, MultiEntryName >
	, public SingletonBase< DataTableManager< TEntry, TBase > >
	, public IDTManager
{
private:
	friend class SingletonBase< DataTableManager< TEntry, TBase > >;
	DataTableManager(){}
	virtual ~DataTableManager()
	{
		deleteAll();
	}

public:

	uint32_t getSize() const
	{
		return size();
	}

	TEntry* getDataByID( const uint64_t id ) const
	{
		TEntry *ret =(TEntry *)getEntryByID(id);
		return ret;	
	}

	TEntry* getDataByName( const char* name ) const
	{
		Entry * ret = NULL;
		MultiEntryName::find( name, ret, true );
		return ( TEntry* )ret;
	}

	template <class YourSceneEntry>
	bool execEveryData(execEntry<YourSceneEntry> &exec)
	{
		bool ret=execEveryEntry<>(exec);
		return ret;
	}

	bool getAllData(std::map<uint32_t, TEntry*>& ret)
	{
		struct ExecAllEntry : public execEntry< TEntry >
		{
			std::map<uint32_t, TEntry*>* _ret;
			ExecAllEntry(std::map<uint32_t, TEntry*>*  res) : _ret(res) {};
			bool exec(TEntry* entry)
			{
				if (NULL == entry)
				{
					return true;
				}

				if (NULL != _ret)
				{
					_ret->insert(std::make_pair(entry->get_id(), entry));
				}
				
				return true;
			}
		}exec(&ret);

		execEveryData(exec);
		return true;
	}

	bool loadBinary(const std::string& config_path)
	{
        std::string filename(config_path);
		filename.append("/tbl/");
        filename.append(TEntry::getBinaryName().c_str());
		uint32_t before_read_time = time_util_t::now_time();
		if (!this->load(filename))
		{
			log_error("read file[%s] field", TEntry::getBinaryName().c_str());
			return false;
		}
		uint32_t now_time = time_util_t::now_time();
		log_info("read file[%s] time[%d]", TEntry::getBinaryName().c_str(), now_time - before_read_time);
        return true;
	};

	virtual bool reload(const std::string& config_path)
	{
        std::string filename(config_path);
		filename.append("/tbl/");
        filename.append(TEntry::getBinaryName().c_str());
		uint32_t before_read_time = time_util_t::now_time();
		if (!loadImp(false, filename))
		{
			return false;
		}
		uint32_t now_time = time_util_t::now_time();
		log_info("read file[%s] time[%d]", TEntry::getBinaryName().c_str(), now_time - before_read_time);
		return true;
	}

	virtual bool load( const std::string& filename )
	{
		addDTManager( TEntry::getBinaryName(), this );
		return loadImp( true, filename );
	}

private:
	bool loadImp( const bool first , const std::string& file_name )
	{
		FILE* fp = fopen( file_name.c_str(), "rb" );
		if( !fp )
		{
			log_error("open [%s] failed!", file_name.c_str());
			return false;
		}

        ///< 读取文件头
        TblxFileHead head;
        if (fread(&head, sizeof(head), 1, fp) != 1)
        {
            log_error("read [%s] head failed!", file_name.c_str());
            fclose(fp);
            return false;
        }
        ///< 读取结构描述符
        std::string descseg;
        descseg.resize(head.desc_seg_len);
        if (fread(&descseg[0], head.desc_seg_len, 1, fp) != 1)
        {
            log_error("read [%s] struct descriptor failed!", file_name.c_str());
            fclose(fp);
            return false;
        }
        int hash = StringTool::ELFhash(descseg);
        if (hash != TBase::FormatCheckCode)
        {
            log_error("check [%s]: tbls64、xml and head file not match!", file_name.c_str());
            fclose(fp);
            return false;
        }

        std::vector< uint32_t > stringoffset;
        uint32_t itemsize = parseDescribleSymbol(descseg, stringoffset);
        if (itemsize == 0)
        {
            log_error("[%s] size[%d] error!", file_name.c_str(), itemsize);
            fclose(fp);
            return false;
        }

        if (head.data_seg_len % itemsize != 0)
        {
            log_error("[%s] size[%d] error!", file_name.c_str(), itemsize);
            fclose(fp);
            return false;
        }

        std::vector< char > data_seg;
        if (head.data_seg_len)
        {
            data_seg.resize(head.data_seg_len);
            if (fread(&data_seg[0], head.data_seg_len, 1, fp) != 1)
            {
                log_error("read [%s] datas failed!", file_name.c_str());
                fclose(fp);
                return false;
            }
        }

        std::vector< char > str_seg;
        if (head.str_seg_len)
        {
            str_seg.resize(head.str_seg_len);
            if (fread(&str_seg[0], head.str_seg_len, 1, fp) != 1)
            {
                log_error("read [%s] strings failed!", file_name.c_str());
                fclose(fp);
                return false;
            }
        }
		
		fclose(fp);

        if (!refresh(head, itemsize, stringoffset, data_seg, str_seg, first))
        {
            log_error("refresh [%s] failed!", file_name.c_str());
            return false;
        }

        return true;
	}

	bool refresh( 
		const TblxFileHead& head, 
		const uint32_t itemsize,
		const std::vector< uint32_t >& stringoffset, 
		std::vector< char >& data_seg,
		const std::vector< char >& str_seg,
		const bool first )
	{
		if( data_seg.empty() )
			return true;

		uint32_t itemcount = head.data_seg_len / itemsize;
		///< 处理变长字符串
		if( str_seg.size() > 0 )
		{
			char* data = &data_seg[0];
			uint32_t platform = sizeof( char* ) * 8;

			for( uint32_t i = 0; i < itemcount; ++i, data += itemsize )
			{
				std::vector< uint32_t >::const_iterator it;
				for( it = stringoffset.begin(); it != stringoffset.end(); ++it )
				{
					///< 根据不同的平台做不同指针操作
					if( platform == 32 )
					{
						uint32_t* p = ( uint32_t* )( data + *it );
						uint32_t straddress = ( uint32_t )( uint64_t )( &str_seg[0] );
						*p += straddress;
					}
					else if( platform == 64 )
					{
						uint64_t* p = ( uint64_t* )( data + *it );
						uint64_t straddress = ( uint64_t )( &str_seg[0] );
						*p += straddress;				
					}
				}
			}
		}

		///< 序列化数据内容
		TBase* tbl = ( TBase* )&data_seg[0];
		for( uint32_t i = 0; i < itemcount; ++i )
		{
			if( !refresh( *tbl, first ) )
			{
				
				return false;
			}
			tbl++;
		}
		return true;
	}

	bool refresh( TBase& data, const bool first )
	{
		TEntry* entry = ( TEntry* )getDataByID( data.getUniqueID() );
		if( first && entry )
		{
			log_error( "primary key[%u] repeat!", data.getUniqueID() );
			return false;				
		}

		if( entry == NULL )
		{
			entry = new TEntry();
			if( entry == NULL )
			{
				log_error( "cannot allocate memory: %s %d", __FILE__, __LINE__ );
				return false;			
			}
			entry->fill( data );
			if( !addEntry( entry ) )
			{
				SAFE_DELETE_P( entry );
				log_error( "primary key[%d] repeat!", data.getUniqueID() );
				return false;			
			}
		}
		else
		{
			entry->fill( data );
			//重新调整名字hash中的位置，这样即使名称改变也可以查询到
			MultiEntryName::remove((Entry * &)entry);
			MultiEntryName::push((Entry * &)entry);
		}
		return true;
	}


	uint32_t parseDescribleSymbol( const std::string& descseg, std::vector< uint32_t >& stringoffset )
	{
		stringoffset.clear();
		uint32_t size = 0;

		for( uint32_t i = 0; i < descseg.size(); ++i )
		{
			switch( descseg[i] )
			{
			case 'b':	/// sbyte
			case 'B':	/// byte
				size += 1;
				break;
			case 's':	/// short
			case 'S':	/// ushort
				size += 2;
				break;
			case 'i':	/// int
			case 'I':	/// uint
				size += 4;
				break;
			case 'l':	/// long
			case 'L':	/// ulong
				size += 8;
				break;
			case 'f':	/// float
				size += 4;
				break;
			case 'd':	/// double
				size += 8;
				break;
			case 'x':	/// 定长字符串
				{
					const char* pnumber = descseg.c_str() + i + 1;
					int fixstrsize = 0;
					char* pend = NULL;

					if( StringTool::try_parse_int( pnumber, &fixstrsize, &pend ) == false || fixstrsize < 0 )
					{
						return -1;
					}

					size += fixstrsize;
					i += ( pend - pnumber );
				}
				break;
			case 'V':	/// 变长字符串
				stringoffset.push_back( size );
				size += 8;
				break;
			case 'v':	/// 变长字符串 
				stringoffset.push_back( size );
				size += 4;
				break;
			case '|':	/// 分隔符
				break;
			default:
				return 0;
				break;
			}
		}

		return size;
	}
};


template < class TKey, class TValue, bool repeat = true >
class SelectByProbability
{
	typedef typename  std::multimap< TKey, TValue >::const_iterator const_iter;
	typedef typename  std::multimap< TKey, TValue >::iterator iter;

public:
	SelectByProbability(  )
		: m_total( 0 )
		, m_repeat( repeat )
	{
	}
	
	void add( TKey key, TValue value )
	{
		m_data.insert( std::pair< TKey, TValue >( key, value ) );
		m_total += key;
	}

	bool getResult( std::pair< TKey, TValue >& ret )
	{
		bool result = false;
		TKey rand = random_util_t::randBetween( 1, m_total );
		iter it = m_data.begin();
		for( ; it != m_data.end(); ++it )
		{
			if( rand <= it->first )
				break;

			rand -= it->first;
		}

		if( it != m_data.end() )
		{
			ret.first = it->first;
			ret.second = it->second;
			result = true;
		}

		if( result && !m_repeat )
		{
			m_total -= it->first;
			m_data.erase( it );
		}

		return result;
	}

	void clear()
	{
		m_data.clear();
		m_total = 0;
	}
	
private:
	std::multimap< TKey, TValue > m_data;
	TKey m_total;
	bool m_repeat;
};


//公共结构存放//////////////////////////////////////////////
////////////////////////////////////////////////////
struct skill_cd_info_t {
	uint32_t skill_id = 0;
	uint32_t is_use_public_cd = 0;
};
