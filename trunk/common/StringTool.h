#pragma once

#include <string>
#include <vector>
#ifndef _MSC_VER
#include <string.h>
#endif

class StringTool
{
public:
	static const int DEFAULT_BUFFER_SIZE = 1024;	///< vformat时默认的缓冲区大小

	StringTool(void);
	~StringTool(void);

	static int ELFhash( const std::string& key );
	static std::string trim(const std::string& str);
	static std::string replace(const std::string& str, const std::string& src, const std::string& dest);
	static int split(const std::string& str, std::vector<std::string>& ret, const std::string sep = "-");

	static bool try_parse_int( const char* str, int *ret, char** end = NULL );
	static std::string base64_encrypt(const unsigned char* Data, const int DataByte);
	static std::string base64_decrypt(const char* Data, const int DataByte,int& OutByte);

	//static unsigned char* charConv(unsigned char *in,const char *fromEncoding = "GBK",const char *toEncoding = "UTF-8");
	//static void charConv( std::string& out, unsigned char *in,const char *fromEncoding = "GBK",const char *toEncoding = "UTF-8");
	

	/// @brief 将字符串就地转换为小写
	static std::string& tolower(std::string& str);

	/// @brief 字符串格式化
	static std::string& format(std::string& resultOUT, const char* fmt, ...);
	/// @brief 字符串格式化
	static std::string format(const char* fmt, ...);

	/// @brief 字符串格式化
	/// @param default_size 默认的缓冲区大小,如果该缓冲区大小不够用则会2倍增长直到缓冲区足够
	static std::string& vformat(std::string& resultOUT, const char *fmt, va_list argptr, int default_size = DEFAULT_BUFFER_SIZE);
	/// @brief 字符串格式化
	/// @param default_size 默认的缓冲区大小,如果该缓冲区大小不够用则会2倍增长直到缓冲区足够
	static std::string vformat(const char *fmt, va_list argptr, int default_size = DEFAULT_BUFFER_SIZE);


	//static std::string getGB2312Str( const char* str );
	//static std::string getUTF8Str( const char* str );

	/**
	* \brief 把字符串根据token转化为多个字符串
	*
	* 下面是使用例子程序：
	*    <pre>
	*    std::list<string> ls;
	*    stringtok (ls," this  \t is\t\n  a test  ");
	*    for(std::list<string>const_iterator i = ls.begin(); i != ls.end(); ++i)
	*        std::cerr << ':' << (*i) << ":\n";
	*     </pre>
	*
	* \param container 容器，用于存放字符串
	* \param in 输入字符串
	* \param delimiters 分隔符号
	* \param deep 深度，分割的深度，缺省没有限制
	*/
	template <typename Container>
	static void
		stringtok(Container &container, std::string const &in,
			const char * const delimiters = " \t\n",
			const int deep = 0)
	{
		const std::string::size_type len = in.length();
		std::string::size_type i = 0;
		int count = 0;

		while (i < len)
		{
			i = in.find_first_not_of(delimiters, i);
			if (i == std::string::npos)
				return;   // nothing left

						  // find the end of the token
			std::string::size_type j = in.find_first_of(delimiters, i);

			// 如果第一个就是分隔符，说明需要添加一个空字符创
			if (i == 1)
			{
				container.push_back("");
			}
			count++;
			// push token
			if (j == std::string::npos
				|| (deep > 0 && count > deep)) {
				container.push_back(in.substr(i));
				return;
			}
			else
				container.push_back(in.substr(i, j - i));

			// set up for next loop
			i = j + 1;
		}
	}
};
