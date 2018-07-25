#ifndef __COMMON_CSV_HPP__
#define __COMMON_CSV_HPP__

#include <string>
#include <map>
#include <vector>
#include "assert.hpp"
#include "log.hpp"
#include "common_struct.hpp"
#include "utility.hpp"
#include <boost/tokenizer.hpp>

#define CONFIG_SRC "../config/csv/"

NS_COMMON_BEGIN

class csv_t
{
public:
    csv_t();
    csv_t(const char* path);
    ~csv_t();

    bool load(const char* tb_name);
    void clear();

    int32_t count() const { return int32_t(m_tab_list.size()); }

    template<typename T_LIST_TYPE>
    T_LIST_TYPE get_val(int32_t row, std::string in_field)
    {
        if (in_field.empty())
        {
            log_error("the field[%s] is invlid!", in_field.c_str());
            return T_LIST_TYPE();
        }

        int32_t column = -1;
        for (int32_t i = 0 ; i < (int32_t)m_field_list.size(); ++i)
        {
            std::string field = m_field_list[i];
            if (0 == strcmp(in_field.c_str(), field.c_str()))
            {
                column = i;
                break;
            }
        }
        if (column < 0)
        {
            log_error("%s can't find the field[%s]!", m_path.c_str(), in_field.c_str());
            return T_LIST_TYPE();
        }

        std::map<int32_t, str_vec>::iterator it = m_tab_list.find(row+1);
        std::map<int32_t, str_vec>::iterator it_end = m_tab_list.end();
        if (it != it_end)
        {
            std::vector<std::string>& row_str_list = it->second;
            int32_t total_colunm = int32_t(row_str_list.size());
            if (column < total_colunm)
            {
                std::string column_str = row_str_list[column];
                if (!column_str.empty())
                {
                    return boost::lexical_cast<T_LIST_TYPE>(column_str);
                }
                else
                {
                    log_error("%s row[%d] field[%s]'s value is empty!", m_path.c_str(), row+2, in_field.c_str());
                }
            }
            else
            {
                log_error("%s field[%d] column[%d] >= total column[%d]!", m_path.c_str(), in_field.c_str(), column, total_colunm);
            }
        }
        else
        {
            log_error("%s can't find the row[%d]!", m_path.c_str(), row+2);
        }

        return T_LIST_TYPE();
    }

    template<typename T_LIST_TYPE>
    void get_list(int32_t row, std::string in_field, std::vector<T_LIST_TYPE>& list, const char* dec="|")
    {
        list.clear();

        if (in_field.empty())
        {
            log_error("the field[%s] is invlid!", in_field.c_str());
            return;
        }

        int32_t column = -1;
        for (int32_t i = 0 ; i < (int32_t)m_field_list.size(); ++i)
        {
            std::string field = m_field_list[i];
            if (0 == strcmp(in_field.c_str(), field.c_str()))
            {
                column = i;
                break;
            }
        }
        if (column < 0)
        {
            log_error("%s can't find the field[%s]!", m_path.c_str(), in_field.c_str());
            return;
        }

        std::map<int32_t, str_vec>::iterator it = m_tab_list.find(row+1);
        std::map<int32_t, str_vec>::iterator it_end = m_tab_list.end();
        if (it != it_end)
        {
            std::vector<std::string>& row_str_list = it->second;
            int32_t total_colunm = int32_t(row_str_list.size());
            if (column < total_colunm)
            {
                std::string column_str = row_str_list[column];
                if (!column_str.empty())
                {
                    //if (0 != strcmp(column_str.c_str(), "0"))
                    //{
                        std::string::size_type pos = column_str.find(dec);
                        if (pos != std::string::npos)
                        {
                            boost::char_separator<char> sep(dec);
                            typedef boost::tokenizer<boost::char_separator<char>> CustonTokenizer;
                            CustonTokenizer tok(column_str,sep);
                            for(CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
                            {
                                list.push_back(boost::lexical_cast<T_LIST_TYPE>(*beg));
                            }
                        }
                        else
                        {
                            list.push_back(boost::lexical_cast<T_LIST_TYPE>(column_str));
                        }

                        if (list.size() > 0)
                        {
                            return;
                        }
                    //}
                }
                else
                {
                    log_error("%s row[%d] field[%s]'s value is empty!", m_path.c_str(), row+2, in_field.c_str());
                }
            }
            else
            {
                log_error("%s field[%d] column[%d] >= total column[%d]!", m_path.c_str(), in_field.c_str(), column, total_colunm);
            }
        }
        else
        {
            log_error("%s can't find the row[%d]!", m_path.c_str(), row+2);
        }

        return;
    }

    template<
        typename T_LIST_TYPE, 
        typename T_TYPE_1, 
        typename T_TYPE_2, 
        typename T_TYPE_3=int32_t, 
        typename T_TYPE_4=int32_t, 
        typename T_TYPE_5=int32_t, 
        typename T_TYPE_6=int32_t, 
        typename T_TYPE_7=int32_t, 
        typename T_TYPE_8=int32_t, 
        typename T_TYPE_9=int32_t
    >
    bool set_param(int32_t row, const char* config_name, std::vector<T_LIST_TYPE>& obj_list, 
    std::string field_1, 
    std::string field_2, 
    std::string field_3="", 
    std::string field_4="",
    std::string field_5="", 
    std::string field_6="", 
    std::string field_7="", 
    std::string field_8="", 
    std::string field_9="")
    {
        int32_t field_num = 0;

        std::vector<T_TYPE_1> field_1_list;
        if (!field_1.empty())
        {
            get_list<T_TYPE_1>(row, field_1, field_1_list);
            field_num += 1;
        }

        std::vector<T_TYPE_2> field_2_list;
        if (!field_2.empty())
        {
            get_list<T_TYPE_2>(row, field_2, field_2_list);
            field_num += 1;
        }

        std::vector<T_TYPE_3> field_3_list;
        if (!field_3.empty())
        {
            get_list<T_TYPE_3>(row, field_3, field_3_list);
            field_num += 1;
        }

        std::vector<T_TYPE_4> field_4_list;
        if (!field_4.empty())
        {
            get_list<T_TYPE_4>(row, field_4, field_4_list);
            field_num += 1;
        }

        std::vector<T_TYPE_5> field_5_list;
        if (!field_5.empty())
        {
            get_list<T_TYPE_5>(row, field_5, field_5_list);
            field_num += 1;
        }

        std::vector<T_TYPE_6> field_6_list;
        if (!field_6.empty())
        {
            get_list<T_TYPE_6>(row, field_6, field_6_list);
            field_num += 1;
        }

        std::vector<T_TYPE_7> field_7_list;
        if (!field_7.empty())
        {
            get_list<T_TYPE_7>(row, field_7, field_7_list);
            field_num += 1;
        }

        std::vector<T_TYPE_8> field_8_list;
        if (!field_8.empty())
        {
            get_list<T_TYPE_8>(row, field_8, field_8_list);
            field_num += 1;
        }

        std::vector<T_TYPE_9> field_9_list;
        if (!field_9.empty())
        {
            get_list<T_TYPE_9>(row, field_9, field_9_list);
            field_num += 1;
        }

        if (2 == field_num)
        {
            if (field_1_list.size() == field_2_list.size())
            {
                for (int32_t i = 0; i < int32_t(field_1_list.size()); ++i)
                {
                    T_LIST_TYPE obj;
                    obj.field_1 = field_1_list[i];
                    obj.field_2 = field_2_list[i];
                    obj_list.push_back(obj);
                }
            }
            else
            {
                log_error("%s row[%d]: [%s][%s] size error!", config_name, row+1, field_1.c_str(), field_2.c_str());
                return false;
            }
        }
        else if (3 == field_num)
        {
            if (field_1_list.size() == field_2_list.size() && field_1_list.size() == field_3_list.size())
            {
                for (int32_t i = 0; i < int32_t(field_1_list.size()); ++i)
                {
                    T_LIST_TYPE obj;
                    obj.field_1 = field_1_list[i];
                    obj.field_2 = field_2_list[i];
                    obj.field_3 = field_3_list[i];
                    obj_list.push_back(obj);
                }
            }
            else
            {
                log_error("%s row[%d]: [%s][%s][%s] size error!", config_name, row+2, field_1.c_str(), field_2.c_str(), field_3.c_str());
                return false;
            }
        }
        else if (9 == field_num)
        {
            if (field_1_list.size() == field_2_list.size() && field_1_list.size() == field_3_list.size() && field_1_list.size() == field_4_list.size() 
                && field_1_list.size() == field_5_list.size() && field_1_list.size() == field_6_list.size() && field_1_list.size() == field_7_list.size()
                && field_1_list.size() == field_8_list.size() && field_1_list.size() == field_9_list.size())
            {
                for (int32_t i = 0; i < int32_t(field_1_list.size()); ++i)
                {
                    T_LIST_TYPE obj;
                    obj.field_1 = field_1_list[i];
                    obj.field_2 = field_2_list[i];
                    obj.field_3 = field_3_list[i];
                    obj.field_4 = field_4_list[i];
                    obj.field_5 = field_5_list[i];
                    obj.field_6 = field_6_list[i];
                    obj.field_7 = field_7_list[i];
                    obj.field_8 = field_8_list[i];
                    obj.field_9 = field_9_list[i];
                    obj_list.push_back(obj);
                }
            }
            else
            {
                log_error("%s row[%d]: [%s][%s]....... size error!", config_name, row+2, field_1.c_str(), field_2.c_str());
                return false;
            }
        }
        return true;
    }

private:
    std::string m_path;
    typedef std::vector<std::string> str_vec;
    std::map<int32_t, str_vec> m_tab_list;
    std::map<int32_t, std::string> m_field_list;
};

NS_COMMON_END

#endif
