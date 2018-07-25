#include "csv.hpp"
#include <fstream>


NS_COMMON_BEGIN

csv_t::csv_t()
{

}

csv_t::csv_t(const char *path)
{
    load(path);
}

csv_t::~csv_t()
{
    clear();
}

bool csv_t::load(const char *tb_name)
{
    char path[256];
    snprintf(path, sizeof(path), "%s%s", CONFIG_SRC, tb_name);
    std::ifstream csv;
    csv.exceptions((csv.rdstate() & std::ios::failbit) | (csv.rdstate() & std::ios::badbit));
    csv.open(path, std::ios::binary|std::ios::in);
    if (!csv)
    {
        printf("\033[0m\033[31m[ERROR] Read config file [ %s ] failed!\n\033[0m", path);
        return false;
    }

    std::string line;
    int32_t row = 0;
    std::vector<std::string> row_str_list;
    m_field_list.clear();
    while (getline(csv, line))
    {
        row_str_list.clear();
        boost::char_separator<char> sep(",");
        typedef boost::tokenizer<boost::char_separator<char>> CustonTokenizer;
        CustonTokenizer tok(line, sep);
        CustonTokenizer::iterator beg = tok.begin();
        if (beg == tok.end())
        {
            printf("\033[0m\033[31mempty csv line, file:[%s] line:[%s], skip it\n\033[0m", path, line.c_str());
            continue;
        }
        std::string first_field(*beg);
        if (first_field.empty())
        {
            printf("\033[0m\033[31mempty csv line, file:[%s] line:[%s], skip it\n\033[0m", path, line.c_str());
            continue;
        }
        for(; beg != tok.end(); ++beg)
        {
            if (!(*beg).empty())
            {
                row_str_list.push_back(*beg);
            }
        }

        if (0 == row)
        {
            for (int32_t i = 0; i < (int32_t)row_str_list.size(); ++i)
            {
                std::string str = row_str_list[i];
                m_field_list[i] = str;
            }
        }
        else
        {
            m_tab_list[row] = row_str_list;
        }
        ++row;
    }
    m_path = tb_name;
    csv.close();
    return true;
}

void csv_t::clear()
{
    m_path = "";
    m_tab_list.clear();
    m_field_list.clear();
}


NS_COMMON_END
