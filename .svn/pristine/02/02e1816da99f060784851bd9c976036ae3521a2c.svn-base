#ifndef __COMMON_UTILITY_HPP__
#define __COMMON_UTILITY_HPP__

#include "macros.hpp"
#include "assert.hpp"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/locale.hpp>
#include <map>

#include <time.h>

NS_COMMON_BEGIN

template <typename int_type=uint32_t>
class bit_array_t
{
public:
    typedef int_type inner_type;

    bit_array_t() : inner_int(0)
    {
        //reset();
    }

    ~bit_array_t()
    {
    }

    inner_type get_inner_val() const
    {
        return inner_int;
    }

    void reset()
    {
        inner_int = 0;
    }

    bool set_bit(uint32_t bit_idx)
    {
        if (bit_idx>(sizeof(int_type)*8-1))
        { 
            return false;
        }

        inner_int |= (0x00000001<<bit_idx);
        return true;
    }

    bool set_bit(int32_t bit_idx)
    {
        return set_bit(uint32_t(bit_idx));
    }

    bool clear_bit(uint32_t bit_idx)
    { 
        if (bit_idx>(sizeof(int_type)*8-1))
        { 
            return false;
        }

        inner_int &= (~(0x00000001<<bit_idx));
        return true;
    }

    bool clear_bit(int32_t bit_idx)
    {
        return clear_bit(uint32_t(bit_idx));
    }

    bool is_bit_set(uint32_t bit_idx) const
    {
        if (bit_idx>(sizeof(int_type)*8-1))
        {
            return false;
        }

        return (inner_int & (0x00000001<<bit_idx)) > 0;
    }

    bool is_bit_set(int32_t bit_idx) const
    {
        return is_bit_set(uint32_t(bit_idx));
    }

private:
    int_type inner_int;
};
typedef bit_array_t<uint32_t> bit_array;

struct bit_util_t
{
    template <class T>  
    static void printf_binary(T val)  
    {  
        uint32_t i  = 0;  
        for (i = sizeof(val) * 8 - 1; i >= 0; --i)  
        {  
            if ((val >> i) & 1)  
                printf("1");
            else   
                printf("0");
            if (0 == i % 8)  
                printf(" ");
        }  
        printf("\n");
    }

    static void set_bit_val(uint32_t& src_val, uint32_t pos, uint32_t bit_val)
    {
        src_val |=  bit_val << pos;
    }

    static uint32_t get_bit_val(uint32_t src_val, uint32_t pos)
    {
        return (src_val >> pos) & 0x1;
    }

    static bool is_bit_same_bit_val(uint32_t src_val, uint32_t pos, uint32_t bit_val)
    {
        uint32_t n = bit_val << pos;
        if ((src_val & n) == n)
        {
            return true;
        }
        return false;
    }

    static int32_t get_low_pos(uint32_t src_val, uint32_t max_pos, uint32_t bit_val)
    {
        uint32_t src_tmp = 0;
        for (uint32_t pos = 0; pos < max_pos; ++pos)
        {
            src_tmp = src_val & bit_val;
            if (src_tmp == 0)
            {
                return pos;
            }
            src_val >>= 1;
        }
        return -1;
    }

    static uint32_t get_cumulative_num(uint32_t src_val, uint32_t bit_val)
    {
        uint32_t times = 0;
        while (src_val > 0)
        {
            times += src_val & bit_val;
            src_val >>= 1;
        }
        return times;
    }
};

struct string_util_t
{
    //get a substring with cnt character start from start
    //note:start and cnt means character not byte ;only for ansi and chinese
    static std::string trim_invalid_character(std::string str)
    {
        auto str_trimmed = str;
        uint32_t length = str.length();
        if( (uint8_t)str_trimmed[length - 1] >= 0x80)
        {
            uint32_t i = length -2;
            uint32_t cnt = 1;
            for(; i > 0; i--)
            {
                if( (uint8_t)str_trimmed[length - 1] >= 0x80)
                {
                    cnt++;
                }
                if((uint8_t)str_trimmed[i] >= 0xe0)
                {
                    break;
                }
            }
            if( cnt  != 3)
            {
                memset(&str_trimmed[length - cnt], 0, cnt);
            }
        }
        return str_trimmed;
    }
    static std::string substr(const std::string& str, uint32_t start, uint32_t cnt)
    {
        auto substring = trim_invalid_character(str);
        uint32_t count = 0;
        uint32_t start_pos = 0;
        uint32_t end_pos = 0;
        uint32_t i = 0;
        for(; i < substring.length(); )
        {
            if( count == start)
            {
                start_pos = i;
            }
            if( count == start + cnt)
            {
                end_pos = i;
                break;
            }
            if((uint8_t)substring[i] >= 0x80)
            {
                i += 3;
            }
            else
            {
                i++;
            }
            count++;
        }
        if( i >= substring.length())
        {
            end_pos = substring.length();
        }
        substring = substring.substr(start_pos, end_pos - start_pos);
        return substring;
    }

    static uint32_t get_character_cnt(const std::string& str)
    {
        uint32_t count = 0;
        for(uint32_t i = 0; i < str.length(); )
        {
            if((uint8_t)str[i] >= 0x80)
            {
                i += 3;
            }
            else
            {
                i++;
            }
            count++;
        }

        return count;
    }
    template<class T_FIRST>
    static void string_format(boost::format& fmt, T_FIRST&& first)
    {
        fmt % first;
    }

    template<class T_FIRST, class... T_OTHER>
    static void string_format(boost::format& fmt, T_FIRST&& first, T_OTHER&&... other)
    {
        fmt % first;
        string_format(fmt, other...);
    }

    template<class T_FIRST, class... T_OTHER>
    static std::string string_format(const char* format, T_FIRST&& first, T_OTHER&&... other)
    {
        std::string str;
        try
        {
            boost::format fmt(format);
            string_format(fmt, first, other...);
            str = fmt.str();
        }
        catch(std::exception& ec)
        {
            str = "";
        }
        return std::move(str);
    }

	template<typename T_TYPE>
    static void combine(T_TYPE val, std::string& str, const char* dec)
    {
        std::string val_str = boost::lexical_cast<std::string>(val) + dec;
        str += val_str;
    }

    template<typename T_TYPE>
    static void split(const std::string &str, std::vector<T_TYPE>& list, const char* dec, bool clear=true)
    {
        list.clear();
		try {
		
			typedef boost::tokenizer<boost::char_separator<char>> CustonTokenizer;
			boost::char_separator<char> sep(dec);
			CustonTokenizer tok(str, sep);
			CustonTokenizer::iterator it = tok.begin();
			CustonTokenizer::iterator it_end = tok.end();
			for(; it != it_end; ++it)
			{
			    if (!(*it).empty())
			    {
			        list.push_back(boost::lexical_cast<T_TYPE>(*it));
			    }
			}
	
	        if (clear && list.size() == 1 && boost::lexical_cast<T_TYPE>(list[0]) == boost::lexical_cast<T_TYPE>(0))
	        {
	            list.clear();
	        }
		}catch(...) {
           list.clear();
		}
    }

	static std::string convert_to_utf8(const std::string& str, const char* from = "GBK")
	{
		std::string charset(from);
		std::string res = boost::locale::conv::to_utf<char>(str, charset);
		return res;
	}

	static std::string convert_to_utf8(const char* str, const char* from = "GBK")
	{
		std::string charset(from);
		std::string conv(str);
		std::string res = boost::locale::conv::to_utf<char>(conv, charset);
		return res;
	}

	static std::string convert_from_utf8(const std::string& str, const char* to = "GBK")
	{
		std::string charset(to);
		std::string res = boost::locale::conv::from_utf<char>(str, charset);
		return res;
	}

	static std::string convert_from_utf8(const char* str, const char* to = "GBK")
	{
		std::string charset(to);
		std::string conv(str);
		std::string res = boost::locale::conv::from_utf<char>(conv, charset);
		return res;
	}

	// 匹配中文
	static bool is_no_special_char(const std::string& str)
	{
		boost::regex reg("^[\u4e00-\u9fa5a-zA-Z0-9]+$");
		return boost::regex_match(str, reg);
	}

	// 先转unicode在计算大小
	static uint32_t get_char_size(const std::string& str)
	{
		std::string unicode_str = common::string_util_t::convert_from_utf8(str.c_str());
		return unicode_str.size() / 2;
	}
    
    static bool is_a_number(std::string str)
    {
        if (str.empty() || 0 == str.compare(""))
        {
            return false;
        }

        for (size_t idx = 0; idx < str.size(); ++idx)
        {
            if (str.at(idx) < 30 || str.at(idx) > 39)
            {
                return false;
            }
        }

        return true;
    }
 
    static bool ParseIds(std::string str_to_parse, std::vector<uint32_t>& ret, const char sep = ':')
    {
        if (str_to_parse.empty())
        {
            return false;
        }

        std::string::size_type _pos = str_to_parse.find_first_of(sep);
        while (std::string::npos != _pos)
        {
            std::string id_str;
            id_str = str_to_parse.substr(0, _pos);
            str_to_parse = str_to_parse.substr(_pos + 1);
            uint32_t id = 0;
            id = atoi(id_str.c_str());
            ret.push_back(id);
            _pos = str_to_parse.find_first_of(sep);
        }
        if (!str_to_parse.empty())
        {
            uint32_t id = 0;
            id = atoi(str_to_parse.c_str());
            ret.push_back(id);
        }
        return true;
    }

    static bool ParseIds(std::string str_to_parse, std::vector<uint64_t>& ret, const char sep = ':')
    {
        if (str_to_parse.empty())
        {
            return false;
        }

        std::string::size_type _pos = str_to_parse.find_first_of(sep);
        while (std::string::npos != _pos)
        {
            std::string id_str;
            id_str = str_to_parse.substr(0, _pos);
            str_to_parse = str_to_parse.substr(_pos + 1);
            uint64_t id = 0;
            id = atoll(id_str.c_str());
            ret.push_back(id);
            _pos = str_to_parse.find_first_of(sep);
        }
        if (!str_to_parse.empty())
        {
            uint64_t id = 0;
            id = atoll(str_to_parse.c_str());
            ret.push_back(id);
        }
        return true;
    }

    static bool ParseIdcnt(std::string str_to_parse, std::map<uint32_t, uint32_t>& ret, char sep = ':', const char sec_sep = ',')
    {
        if (str_to_parse.empty())
        {
            return false;
        }

        std::string::size_type _pos = str_to_parse.find_first_of(sep);
        while (std::string::npos != _pos)
        {
            std::string unit;
            unit = str_to_parse.substr(0, _pos);
            str_to_parse = str_to_parse.substr(_pos + 1);
            std::string::size_type _pos_in = unit.find_first_of(sec_sep);
            if (std::string::npos != _pos_in)
            {
                std::string id_str, cnt_str;
                uint32_t id = 0, cnt = 0;
                id_str = unit.substr(0, _pos_in);
                cnt_str = unit.substr(_pos_in + 1);
                id = atoi(id_str.c_str());
                cnt = atoi(cnt_str.c_str());
                ret[id] += cnt;
            }
            _pos = str_to_parse.find_first_of(sep);
        }
        if (!str_to_parse.empty())
        {
            std::string unit;
            unit = str_to_parse;
            std::string::size_type _pos_in = unit.find_first_of(sec_sep);
            if (std::string::npos != _pos_in)
            {
                std::string id_str, cnt_str;
                uint32_t id = 0, cnt = 0;
                id_str = unit.substr(0, _pos_in);
                cnt_str = unit.substr(_pos_in + 1);
                id = atoi(id_str.c_str());
                cnt = atoi(cnt_str.c_str());
                ret[id] += cnt;
            }
        }
        return true;
    }

    template< typename ContainerType, typename ResultType >
    static bool Parse_id_cnt_str_to_map(const ContainerType& to_parses, ResultType& ret, const char sep = ':')
    {
        for (auto to_parse : to_parses)
        {
            if (to_parse.empty())
            {
                return false;
            }
            std::string::size_type _pos = to_parse.find_first_of(sep);
            if (std::string::npos != _pos)
            {
                std::string id_str, cnt_str;
                id_str = to_parse.substr(0, _pos);
                cnt_str = to_parse.substr(_pos + 1);
                typename ResultType::key_type id = 0;
                typename ResultType::mapped_type cnt = 0;
                id = typename ResultType::key_type(atoll(id_str.c_str()));
                cnt = typename ResultType::mapped_type(atoll(cnt_str.c_str()));
                ret[id] += cnt;
            }
            else
            {
                return false;
            }
            if (to_parses.size() > 100)
            {
                return false;
            }
        }
        return true;
    }

    template<typename ContainerType>
    static bool Parse_type_id_cnt_str_to_map(const ContainerType& to_parses, std::vector<std::tuple<uint32_t, uint32_t, uint32_t> >& ret, bool* isOrder = NULL, const char sep = ':', const char sec_sep = ',')
    {
        for (auto to_parse : to_parses)
        {
            if (to_parse.empty())
            {
                return false;
            }
            if (to_parse == "@")
            {
                if (NULL != isOrder)
                {
                    *isOrder = true;
                }
                continue;
            }
            std::string::size_type _pos = to_parse.find_first_of(sep);
            if (std::string::npos != _pos)
            {
                std::string type_str, id_str, cnt_str;
                uint32_t type = 0, id = 0, cnt = 0;
                type_str = to_parse.substr(0, _pos);
                type = atoi(type_str.c_str());
                id_str = to_parse.substr(_pos + 1);
                _pos = id_str.find_first_of(sep);
                if (std::string::npos != _pos)
                {
                    cnt_str = id_str.substr(_pos + 1);
                    id_str = id_str.substr(0, _pos);
                    _pos = id_str.find_first_of(sec_sep);
                    if (std::string::npos != _pos)//id 为x,y坐标值，生成唯一id
                    {
                        uint32_t x = atoi(id_str.substr(0, _pos).c_str());
                        uint32_t y = atoi(id_str.substr(_pos + 1).c_str());
                        id = x * 10000 + y;
                    }
                    else
                    {
                        id = atoi(id_str.c_str());
                    }
                    cnt = atoi(cnt_str.c_str());
                    /*cnt_str = id_str.substr(_pos + 1);
                    cnt = atoi(cnt_str.c_str());
                    auto & id_cnt = ret[type];
                    id_cnt[id] += cnt;*/
                    ret.push_back(std::make_tuple(type, id, cnt));
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    template<typename ContainerType>
    static bool Parse_type_id_cnt_str_to_map(const ContainerType& to_parses, std::vector<std::tuple<uint32_t, uint32_t, uint32_t, int, float, float> >& ret, bool* isOrder = NULL, const char sep = ':', const char sec_sep = ',')
    {
        for (auto to_parse : to_parses)
        {
            if (to_parse.empty())
            {
                return false;
            }
            if (to_parse == "@")
            {
                if (NULL != isOrder)
                {
                    *isOrder = false;
                }
                continue;
            }
            std::string::size_type _pos = to_parse.find_first_of(sep);
            if (std::string::npos != _pos)
            {
                std::string type_str, id_str, cnt_str;
                uint32_t type = 0, id = 0, cnt = 0;
                int32_t map_id = -1;
                float x = 0, y = 0;
                type_str = to_parse.substr(0, _pos);
                type = atoi(type_str.c_str());
                id_str = to_parse.substr(_pos + 1);
                _pos = id_str.find_first_of(sep);
				int32_t area_id = -1;
				if (std::string::npos != _pos)
                {
                    cnt_str = id_str.substr(_pos + 1);
                    id_str = id_str.substr(0, _pos);
                    _pos = id_str.find_first_of(sec_sep);
                    std::vector<float> map_x_y_id;
                    while (std::string::npos != _pos)//
                    {
                        std::string str;
                        str = id_str.substr(0, _pos);
                        id_str = id_str.substr(_pos + 1);
                        _pos = id_str.find_first_of(sec_sep);
                        map_x_y_id.push_back(atof(str.c_str()));
                    }
                    if (!id_str.empty())
                    {
                        map_x_y_id.push_back(atoi(id_str.c_str()));
                    }
                    if (!map_x_y_id.empty())
                    {
                        id = map_x_y_id[map_x_y_id.size() - 1];
                    }
                    if (map_x_y_id.size() == 4)
                    {
                        map_id = (int32_t)map_x_y_id[0];
                        x = map_x_y_id[1];
                        y = map_x_y_id[2];
                        //id = map_x_y_id[3];
					}

					if (map_x_y_id.size() == 2)
					{
						area_id = map_x_y_id[0];
					}
                    /*else
                    {
                    return false;
                    }*/
                    cnt = atoi(cnt_str.c_str());
                }
                ret.push_back(std::make_tuple(type, id, cnt, map_id, x, y));
            }
            else
            {
                return false;
            }
        }
        return true;
    }

	template<typename ContainerType>
	static bool Parse_type_id_cnt_str_to_map(const ContainerType& to_parses, std::vector<std::tuple<uint32_t, uint32_t, uint32_t, int, float, float, uint32_t> >& ret, bool* isOrder = NULL, const char sep = ':', const char sec_sep = ',')
	{
		for (auto to_parse : to_parses)
		{
			if (to_parse.empty())
			{
				return false;
			}
			if (to_parse == "@")
			{
				if (NULL != isOrder)
				{
					*isOrder = false;
				}
				continue;
			}
			std::string::size_type _pos = to_parse.find_first_of(sep);
			if (std::string::npos != _pos)
			{
				std::string type_str, id_str, cnt_str;
				uint32_t type = 0, id = 0, cnt = 0;
				int32_t map_id = -1;
				float x = 0, y = 0;
				type_str = to_parse.substr(0, _pos);
				type = atoi(type_str.c_str());
				id_str = to_parse.substr(_pos + 1);
				_pos = id_str.find_first_of(sep);
				int32_t area_id = -1;
				if (std::string::npos != _pos)
				{
					cnt_str = id_str.substr(_pos + 1);
					id_str = id_str.substr(0, _pos);
					_pos = id_str.find_first_of(sec_sep);
					std::vector<float> map_x_y_id;
					while (std::string::npos != _pos)//
					{
						std::string str;
						str = id_str.substr(0, _pos);
						id_str = id_str.substr(_pos + 1);
						_pos = id_str.find_first_of(sec_sep);
						map_x_y_id.push_back(atof(str.c_str()));
					}
					if (!id_str.empty())
					{
						map_x_y_id.push_back(atoi(id_str.c_str()));
					}
					if (!map_x_y_id.empty())
					{
						id = map_x_y_id[map_x_y_id.size() - 1];
					}
					if (map_x_y_id.size() == 4)
					{
						map_id = (int32_t)map_x_y_id[0];
						x = map_x_y_id[1];
						y = map_x_y_id[2];
						//id = map_x_y_id[3];
					}

					if (map_x_y_id.size() == 2)
					{
						area_id = map_x_y_id[0];
					}
					/*else
					{
					return false;
					}*/
					cnt = atoi(cnt_str.c_str());
				}
				ret.push_back(std::make_tuple(type, id, cnt, map_id, x, y, area_id));
			}
			else
			{
				return false;
			}
		}
		return true;
	}

    template<typename ContainerType>
    static std::string Ints2Str(ContainerType ints, char sep = ':')
    {
        std::ostringstream ss;
        for (auto ptr : ints)
        {
            ss << ptr << sep;
        }
        return ss.str().substr(0, ss.str().length() - 1);
    }

    static uint64_t string_to_uint64(const std::string &str)
    {
        uint64_t result = 0;
        try
        {
			result = boost::lexical_cast<uint64_t>(str);
        }
        catch (boost::bad_lexical_cast&)
        {
            result = 0;
        }
        return result;
    };

    static uint32_t string_to_uint32(const std::string &str)
    {
        uint32_t result = 0;
        try
        {
			result = boost::lexical_cast<uint32_t>(str);
        }
        catch (boost::bad_lexical_cast&)
        {
            result = 0;
        }
        return result;
    };

    static int32_t string_to_int32(const std::string &str)
    {
        int32_t result = 0;
        try
        {
			result = boost::lexical_cast<int32_t>(str);
        }
        catch (boost::bad_lexical_cast&)
        {
            result = 0;
        }
        return result;
    };

    static std::string uint64_to_string(uint64_t int_value)
    {
		return boost::lexical_cast<std::string>(int_value);
    };

	static std::string uint32_to_string(uint32_t value)
	{
		return boost::lexical_cast<std::string>(value);
	};

	static std::string int32_to_string(int32_t value)
	{
		return boost::lexical_cast<std::string>(value);
	};
};

namespace Misc
{
	// 字符串分割，自己增加分割器（qhj）
	template <typename V>
	class Parse
	{
	public:
		bool operator () (const std::string& down, const std::string& separator_down, V& val)
		{
			std::string::size_type pos = 0;
			if ((pos = down.find(separator_down)) != std::string::npos) {
				val.first = down.substr(0, pos);
				val.second = down.substr(pos + separator_down.length());
				return true;
			}

			return false;
		}
	};

	template <template <typename> class P = Parse>
	class Split
	{
	public:

		template <typename T>
		void operator() (const std::string& list, T& dest, const std::string& separator_up = ";", const std::string& separator_down = ",")
		{
			typedef typename T::value_type value_type;
			//typedef typename T::pointer pointer;

			std::string::size_type lpos = 0;
			std::string::size_type pos = 0;
			P<value_type> p;

			value_type v;
			while ((lpos = list.find(separator_up, pos)) != std::string::npos) {
				if (p(list.substr(pos, lpos - pos), separator_down, v)) dest.push_back(v);
				pos = lpos + 1;
			}

			if (p(list.substr(pos, lpos - pos), separator_down, v)) dest.push_back(v);
		}
	};

	//解析2个分隔符的整形数列
	template<typename T1, typename T2>
	void Parse2SeperatorDataInfo(const std::string &str, std::vector<std::pair<T1, T2> > &dest, const std::string &seper_big = ";", const std::string &seper_small = ":")
	{
		typedef std::vector<std::pair<std::string, std::string> > Con;
		Con con;
		Misc::Split<Misc::Parse> s;
		s(str, con, seper_big, seper_small);
		dest.clear();
		for (Con::const_iterator iter = con.begin(); iter != con.end(); ++iter)
		{
			try
			{
				std::pair<T1, T2> info;
				info.first = boost::lexical_cast<T1>(iter->first);
				info.second = boost::lexical_cast<T2>(iter->second);
				dest.push_back(info);
			}
			catch (boost::bad_lexical_cast&)
			{
				return;
			}
		}
	}

	//解析2个分隔符的整形数列
	template<typename T1, typename T2>
	void Parse2SeperatorDataInfo(const std::string &str, std::map<T1, T2> &dest, const std::string &seper_big = ";", const std::string &seper_small = ":")
	{
		typedef std::vector<std::pair<std::string, std::string> > Con;
		Con con;
		Misc::Split<Misc::Parse> s;
		s(str, con, seper_big, seper_small);
		dest.clear();
		for (Con::const_iterator iter = con.begin(); iter != con.end(); ++iter)
		{
			try
			{
				std::pair<T1, T2> info;
				info.first = boost::lexical_cast<T1>(iter->first);
				info.second = boost::lexical_cast<T2>(iter->second);
				dest.insert(info);
			}
			catch (boost::bad_lexical_cast&)
			{
				return;
			}
		}
	}
};

//////////////////////////////////////////////////////////////////////////

#define HOUR_SECOND (60 * 60)

#define DAY_SECOND (24 * HOUR_SECOND)

#define WEEK_SECOND (DAY_SECOND * 7)

class time_util_t
{
private:
	// 时间差
	static uint32_t m_delta_time;
	/**
	* 保留的时区信息
	* 程序初始化的时候设置，运行过程中一般不变
	*/
	static long my_timezone;

public:
	static const std::string& get_local_tz();

	static long get_timezone() { return my_timezone; }

	// 得到当前UTC时间
	static inline uint32_t now_time() { return time(NULL) + m_delta_time; }

	// 必须是x86_64体系 否则这个调用极消耗效率
	static inline uint64_t now_msecs()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return (tv.tv_sec + m_delta_time) * 1000L + tv.tv_usec / 1000L;
	}

	// 设置向后时间戳
	static void set_delta_time(uint32_t time) { m_delta_time = time; }
	static uint32_t get_delta_time() { return m_delta_time; }

	static uint32_t get_local_day(time_t t)
	{
		if (t < DAY_SECOND) return 0;

		return (t - my_timezone) / DAY_SECOND;
	}

	static void get_local_time(struct tm& tv1, time_t t)
	{
		t -= my_timezone;
		gmtime_r(&t, &tv1);
	}

	static time_t get_utc_time(struct tm& tv1)
	{
		time_t t = timegm(&tv1);
		if (t != (time_t)-1)
			t += my_timezone; //UTC时间

		return t;
	}

	// 1970年1月1日是周四
	static uint32_t get_local_week(time_t t)
	{
		if (t < WEEK_SECOND) return 0;

		return (t - 4 * DAY_SECOND - my_timezone) / WEEK_SECOND;
	}

	static uint32_t get_local_month(time_t t)
	{
		struct tm tm;
		get_local_time(tm, t);

		uint32_t year = tm.tm_year + 1900;
		uint32_t month = tm.tm_mon + 1;

		uint32_t val = (year << 16) | month;
		return val;
	}

	static uint32_t get_today()
	{
		time_t cur_time = now_time();
		struct tm day_time;
		get_local_time(day_time, cur_time);
		uint32_t year = day_time.tm_year + 1900;
		uint32_t month = day_time.tm_mon + 1;
		uint32_t day = day_time.tm_mday;
		uint32_t today = (year << 9) | (month << 5) | day;
		return today;
	}

	static uint32_t get_tomorrow()
	{
		time_t cur_time = now_time() + DAY_SECOND;
		struct tm day_time;
		get_local_time(day_time, cur_time);
		uint32_t year = day_time.tm_year + 1900;
		uint32_t month = day_time.tm_mon + 1;
		uint32_t day = day_time.tm_mday;
		uint32_t tomorrow = (year << 9) | (month << 5) | day;
		return tomorrow;
	}

	static uint32_t get_day_after_tommrrow()
	{
		time_t cur_time = now_time() + DAY_SECOND * 2;
		struct tm day_time;
		get_local_time(day_time, cur_time);
		uint32_t year = day_time.tm_year + 1900;
		uint32_t month = day_time.tm_mon + 1;
		uint32_t day = day_time.tm_mday;
		uint32_t day_after_tommrrow = (year << 9) | (month << 5) | day;
		return day_after_tommrrow;
	}

	//获取过去的第几天的日期，昨天参数传1前天传2以此类推
	static uint32_t get_pass_day(uint32_t count)
	{
		time_t cur_time = now_time() - DAY_SECOND * count;
		struct tm day_time;
		get_local_time(day_time, cur_time);
		uint32_t year = day_time.tm_year + 1900;
		uint32_t month = day_time.tm_mon + 1;
		uint32_t day = day_time.tm_mday;
		uint32_t pass_day = (year << 9) | (month << 5) | day;
		return pass_day;
	}

	static std::string get_day_str(uint32_t date)
	{
		uint32_t day = date & 0x1F;
		uint32_t month = (date >> 5) & 0xF;
		uint32_t year = (date >> 9) & 0x7FFFFF;
		char szTime[256] = { 0 };
		snprintf(szTime, sizeof(szTime) - 1, "%4.4d-%2.2d-%2.2d", year, month, day);
		return szTime;
	}

	static uint32_t get_year()
	{
		time_t cur_time = now_time();
		struct tm day_time;
		get_local_time(day_time, cur_time);
		return uint32_t(day_time.tm_year + 1900);
	}

	static uint32_t get_month()
	{
		time_t cur_time = now_time();
		struct tm day_time;
		get_local_time(day_time, cur_time);
		return uint32_t(day_time.tm_mon + 1);
	}

	static uint32_t get_day()
	{
		time_t cur_time = now_time();
		struct tm day_time;
		get_local_time(day_time, cur_time);
		return uint32_t(day_time.tm_mday);
	}

	static uint32_t get_day(uint32_t time_val)
	{
		struct tm day_time;
		get_local_time(day_time, time_val);
		return uint32_t(day_time.tm_mday);
	}

	static uint32_t get_hour()
	{
		time_t cur_time = now_time();
		struct tm day_time;
		get_local_time(day_time, cur_time);
		return uint32_t(day_time.tm_hour);
	}

	static uint32_t get_minutes()
	{
		time_t cur_time = now_time();
		struct tm day_time;
		get_local_time(day_time, cur_time);
		return uint32_t(day_time.tm_min);
	}

	static uint32_t get_seconds()
	{
		time_t cur_time = now_time();
		struct tm day_time;
		get_local_time(day_time, cur_time);
		return uint32_t(day_time.tm_sec);
	}

	// [1-7]
	static uint32_t get_weekday()
	{
		time_t cur_time = now_time();
		struct tm day_time;
		get_local_time(day_time, cur_time);
		if (day_time.tm_wday > 0)
		{
			return day_time.tm_wday;
		}
		else
		{
			return 7;
		}
	}

	static uint32_t get_days_in_month()
	{
		time_t cur_time = now_time();
		struct tm day_time;
		get_local_time(day_time, cur_time);
		uint32_t y = day_time.tm_year + 1900;
		uint32_t m = day_time.tm_mon + 1;
		uint32_t d;
		uint32_t day[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		if (2 == m)
		{
			d = ((((0 == y % 4) && (0 != y % 100)) || (0 == y % 400)) ? 29 : 28);
		}
		else
		{
			d = day[m - 1];
		}
		return d;
	}

	static uint32_t get_month_days(uint32_t year, uint32_t month)
	{
		if (month > 12)
		{
			return 0;
		}

		uint32_t days;
		uint32_t day[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		if (2 == month)
		{
			days = ((((0 == year % 4) && (0 != year % 100)) || (0 == year % 400)) ? 29 : 28);
		}
		else
		{
			days = day[month - 1];
		}

		return days;
	}

	static bool is_same_day(time_t to_check_time, time_t cur_time = 0)
	{
		if (to_check_time <= 0)
		{
			return false;
		}
		if (cur_time == 0)
		{
			cur_time = now_time();
		}
		return get_local_day(cur_time) == get_local_day(to_check_time);
	}

	static bool is_same_week(time_t to_check_time)
	{
		// 对于1970年的第一周做过滤直接判为false
		if (to_check_time < WEEK_SECOND) return false;

		return get_local_week(to_check_time) == get_local_week(now_time());
	}

	//time_t --> YYYY-MM-DD HH:MM:SS
	static std::string FormatTime_to_string(time_t time1, bool all)
	{
		if (time1 == 0)
		{
			return "0";
		}

		struct tm tm1;
		get_local_time(tm1, time1);
		char szTime[256] = {};
		if (all)
		{
			snprintf(szTime, sizeof(szTime), "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",
				tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday,
				tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
		}
		else
		{
			snprintf(szTime, sizeof(szTime), "%4.4d-%2.2d-%2.2d",
				tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday);
		}
		std::string strTime = szTime;
		return std::move(strTime);
	}

	//YYYY-MM-DD HH:MM:SS --> time_t
	static uint32_t FormatTime_to_time_t(std::string szTime)
	{
		if (strcmp(szTime.c_str(), "0") == 0)
		{
			return 0;
		}

		struct tm tm1;
		time_t time1;
		sscanf(szTime.c_str(), "%4d%2d%2d%2d%2d%2d",
			&tm1.tm_year,
			&tm1.tm_mon,
			&tm1.tm_mday,
			&tm1.tm_hour,
			&tm1.tm_min,
			&tm1.tm_sec);

		tm1.tm_year -= 1900;
		tm1.tm_mon--;
		if (tm1.tm_hour < 0)
			tm1.tm_hour = 0;
		if (tm1.tm_min < 0)
			tm1.tm_min = 0;
		if (tm1.tm_sec < 0)
			tm1.tm_sec = 0;
		tm1.tm_isdst = -1;
		time1 = mktime(&tm1);
		return uint32_t(time1);
	}

	// 暂时留着
	static boost::posix_time::ptime get_loacl_day_time(uint32_t hour = 0, uint32_t minutes = 0)
	{
		boost::gregorian::date local_day(boost::gregorian::day_clock::local_day());
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		boost::posix_time::ptime recover_time(local_day, boost::posix_time::hours(hour));
		recover_time += boost::posix_time::minutes(minutes);

		if (now >= recover_time)
		{
			recover_time += boost::gregorian::days(1);
		}
		return recover_time;
	}

	// 获取deadline_timer的剩余时间
	static uint32_t get_remain_time(boost::shared_ptr<boost::asio::deadline_timer> p_timer)
	{
		if (NULL == p_timer)
		{
			return 0;
		}
		int32_t ret = p_timer->expires_from_now().total_seconds();
		if (ret <= 0)
		{
			ret = 1;
		}
		return ret;
	}
};

class cTimer
{
public:
	/**
	* \brief 重新设置定时器的精度和开始计时时间
	* \param how_long  定时器的时间，单位：秒
	* \param ctv       当前时间
	*
	*/
	void reset(const uint32_t how_long, const uint32_t start);

	cTimer(const uint32_t how_long, const uint32_t start);

	void next(const uint32_t start);

	bool operator() (const uint32_t curr);

	uint32_t sec() const { return _timer; }
private:
	uint32_t _long;
	uint32_t _timer;
};

struct hash_util_t
{
	template< typename T1, typename T2 >
	static uint32_t hash_to_uint32(const T1 t1, const T2 t2)
	{
		uint32_t key = t1;
		key <<= 16;
		key += t2;
		return key;
	}

	template< typename T >
	static inline T high_by_uint32(const uint32_t value)
	{
		return (T)(value >> 16);
	}

	template< typename T >
	static inline T low_by_uint32(const uint32_t value)
	{
		return (T)(value & 0x0000FFFF);
	}

	template< typename T1, typename T2 >
	static inline uint16_t hash_to_uint16(const T1 t1, const T2 t2)
	{
		uint16_t key = t1;
		key <<= 8;
		key += t2;
		return key;
	}

	template< typename T >
	static inline T high_by_uint16(const uint16_t value)
	{
		return (T)(value >> 8);
	}

	template< typename T >
	static inline T low_by_uint16(const uint16_t value)
	{
		return (T)(value & 0x00FF);
	}

	template< typename T1, typename T2, typename T3 >
	static inline uint32_t hash_to_uint32(const T1 t1, const T2 t2, const T3 t3)
	{
		uint16_t key1 = hash_to_uint16< T1, T2 >(t1, t2);
		uint32_t key = hash_to_uint32< uint16_t, T3 >(key1, t3);
		return key;
	}

	template< typename T1, typename T2, typename T3, typename T4 >
	static inline uint32_t hash_to_uint32(const T1 t1, const T2 t2, const T3 t3, const T4 t4)
	{
		uint16_t key1 = hash_to_uint16< T1, T2 >(t1, t2);
		uint16_t key2 = hash_to_uint16< T3, T4 >(t3, t4);
		uint32_t key = hash_to_uint32< uint16_t, uint16_t >(key1, key2);
		return key;
	}

	template< typename T1, typename T2, typename T3 >
	static inline uint32_t hash_to_uint32_ex(const T1 t1, const T2 t2, const T3 t3)
	{
		uint32_t key = t1;
		key <<= 10;
		uint32_t key1 = t2;
		key1 <<= 7;
		key += key1;
		key += t3;
		return key;
	}

	template< typename T1, typename T2 >
	static uint32_t hash_to_uint32_move_7(const T1 t1, const T2 t2)
	{
		uint32_t key = t1;
		key <<= 7;
		key += t2;
		return key;
	}

};

namespace Sort
{

	template<typename T>
	static void qsort_private(std::vector<T> &vec, int32_t low, int32_t high, bool(*_compare_func)(const T&, const T&))
	{
		if (low >= high)
		{
			return;
		}
		int first = low;
		int last = high;
		T key = vec[first];/*用字表的第一个记录作为枢轴*/

		while (first < last)
		{
			while (first < last && _compare_func(vec[last], key))
			{
				--last;
			}

			vec[first] = vec[last];/*将比第一个小的移到低端*/

			while (first < last && !_compare_func(vec[first], key))
			{
				++first;
			}

			vec[last] = vec[first];
			/*将比第一个大的移到高端*/
		}
		vec[first] = key;/*枢轴记录到位*/
		qsort_private(vec, low, first - 1, _compare_func);
		qsort_private(vec, first + 1, high, _compare_func);
	}

	template<typename T>
	static void Qsort(std::vector<T> &vec, uint32_t low, uint32_t high, bool(*_compare_func)(const T&, const T&))
	{
		if (low >= high)
		{
			printf("\033[31mQsort error low[%d] >= high[%d] \033[0m\n", low, high);
			return;
		}
		if (high >= vec.size() || low >= vec.size())
		{
			printf("\033[31mQsort error low[%d] or high[%d] >= vec_size[%d] \033[0m\n", low, high, (int)vec.size());
			return;
		}
		qsort_private(vec, (int32_t)low, (int32_t)high, _compare_func);
	}
}
NS_COMMON_END

#endif //__COMMON_UTILITY_HPP__
