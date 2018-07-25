#include "tbl_reader.h"

#include "common/log.hpp"

#include "StringTool.h"

void tbl_reader_t::read_tbl(const char* name, uint32_t id)
{
	std::string filename = name;
	if (filename.empty())
	{
		printf("\033[31m Name is Empty! \033[0m\n");
		return;
	}

	if (!loadBinary(filename, id))
	{
		printf("\033[31m LoadBinary Error! \033[0m\n");
		return;
	}
}

bool tbl_reader_t::loadBinary(const std::string& name, uint32_t id)
{
	std::string file_name = TBL_PATH + name + TBL_FILE;
	return this->load(file_name, id);
}

bool tbl_reader_t::load(const std::string& filename, uint32_t id)
{
	FILE* fp = fopen(filename.c_str(), "rb");
	if (!fp)
	{
		printf("\033[31m Open [%s] Failed! \033[0m\n", filename.c_str());
		return false;
	}

	printf("\033[33m [%s] Read Success \033[0m\n", filename.c_str());

	///< 读取文件头
	TblxFileHead head;
	if (fread(&head, sizeof(head), 1, fp) != 1)
	{
		printf("\033[31m Read [%s] Head Failed! \033[0m\n", filename.c_str());
		fclose(fp);
		return false;
	}

	///< 读取结构描述符
	std::string descseg;
	descseg.resize(head.desc_seg_len);
	if (fread(&descseg[0], head.desc_seg_len, 1, fp) != 1)
	{
		printf("\033[31m Read [%s] struct Descriptor Failed! \033[0m\n", filename.c_str());
		fclose(fp);
		return false;
	}

	int hash = StringTool::ELFhash(descseg);
	printf("\033[33m DESCSEG:%s MD5 Code:%08X \033[0m\n", descseg.c_str(), hash);

	printf("\033[32m");

	uint32_t index = 0;
	for (uint32_t i = 0; i < descseg.size(); ++i)
	{
		char str[80] = { 0 };

		switch (descseg[i])
		{
		case 'b':	/// sbyte
		case 'B':	/// byte
            snprintf(str, sizeof(str), "|%u-%s", ++index, "BYTE");
			printf("%-8s", str);
			break;
		case 's':	/// short
		case 'S':	/// ushort
            snprintf(str, sizeof(str), "|%u-%s", ++index, "SHORT");
			printf("%-8s", str);
			break;
		case 'i':	/// int
		case 'I':	/// uint
            snprintf(str, sizeof(str), "|%u-%s", ++index, "INT");
			printf("%-10s", str);
			break;
		case 'l':	/// long
		case 'L':	/// ulong
            snprintf(str, sizeof(str), "|%u-%s", ++index, "LONG");
			printf("%-10s", str);
			break;
		case 'f':	/// float
            snprintf(str, sizeof(str), "|%u-%s", ++index, "FLOAT");
			printf("%-10s", str);
			break;
		case 'd':	/// double
            snprintf(str, sizeof(str), "|%u-%s", ++index, "DOUBLE");
			printf("%-10s", str);
			break;
		case 'x':	/// 定长字符串
            snprintf(str, sizeof(str), "|%u-%s", ++index, "VAR_STRING");
			printf("%-20s", str);
			break;
		case 'V':	/// 变长字符串
            snprintf(str, sizeof(str), "|%u-%s", ++index, "VAR_STRING");
			printf("%-20s", str);
			break;
		case 'v':	/// 变长字符串 
            snprintf(str, sizeof(str), "|%u-%s", ++index, "VAR_STRING");
			printf("%-20s", str);
			break;
		case '|':	/// 分隔符
			break;
		default:
            fclose(fp);
			return 0;
			break;
		}
	}
	printf("\033[0m\n");

	std::vector< uint32_t > stringoffset;
	uint32_t itemsize = parseDescribleSymbol(descseg, stringoffset);
	if (itemsize == 0)
	{
		printf("\033[31m [%s] size[%d] error! \033[0m\n", filename.c_str(), itemsize);
		fclose(fp);
		return false;
	}

	if (head.data_seg_len % itemsize != 0)
	{
		printf("\033[31m [%s] size[%d] error! \033[0m\n", filename.c_str(), itemsize);
		fclose(fp);
		return false;
	}

	std::vector< char > data_seg;
	if (head.data_seg_len)
	{
		data_seg.resize(head.data_seg_len);
		if (fread(&data_seg[0], head.data_seg_len, 1, fp) != 1)
		{
			printf("\033[31m Read [%s] Datas Failed! \033[0m\n", filename.c_str());
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
			printf("\033[31m Read [%s] String Failed! \033[0m\n", filename.c_str());
			fclose(fp);
			return false;
		}
	}

	fclose(fp);

	if (!refresh(head, descseg, itemsize, stringoffset, data_seg, str_seg, id))
	{
		printf("\033[31m Refresh [%s] Failed! \033[0m\n", filename.c_str());
		return false;
	}

	return true;
}

uint32_t tbl_reader_t::parseDescribleSymbol(const std::string& descseg, std::vector< uint32_t >& stringoffset)
{
	stringoffset.clear();
	uint32_t size = 0;

	for (uint32_t i = 0; i < descseg.size(); ++i)
	{
		switch (descseg[i])
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

			if (StringTool::try_parse_int(pnumber, &fixstrsize, &pend) == false || fixstrsize < 0)
			{
				return -1;
			}

			size += fixstrsize;
			i += (pend - pnumber);
		}
		break;
		case 'V':	/// 变长字符串
			stringoffset.push_back(size);
			size += 8;
			break;
		case 'v':	/// 变长字符串 
			stringoffset.push_back(size);
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

bool tbl_reader_t::refresh(const TblxFileHead& head, const std::string& descseg, const uint32_t itemsize, const std::vector< uint32_t >& stringoffset, std::vector< char >& data_seg, const std::vector< char >& str_seg, uint32_t id)
{
	if (data_seg.empty())
		return true;

	uint32_t itemcount = head.data_seg_len / itemsize;

	///< 处理变长字符串
	if (str_seg.size() > 0)
	{
		char* data = &data_seg[0];
		uint32_t platform = sizeof(char*) * 8;

		for (uint32_t i = 0; i < itemcount; ++i, data += itemsize)
		{
			std::vector< uint32_t >::const_iterator it;
			for (it = stringoffset.begin(); it != stringoffset.end(); ++it)
			{
				///< 根据不同的平台做不同指针操作
				if (platform == 32)
				{
					uint32_t* p = (uint32_t*)(data + *it);
					uint32_t straddress = (uint32_t)(uint64_t)(&str_seg[0]);
					*p += straddress;
				}
				else if (platform == 64)
				{
					uint64_t* p = (uint64_t*)(data + *it);
					uint64_t straddress = (uint64_t)(&str_seg[0]);
					*p += straddress;
				}
			}
		}
	}

	char* data = &data_seg[0];
	for (uint32_t i = 0; i < itemcount; ++i, data += itemsize)
	{
		if (id != 0)
		{
			bool is_equal = false;
			switch (descseg[0])
			{
			case 'b':	/// sbyte
			case 'B':	/// byte
				if (id == *(uint8_t*)(data))
					is_equal = true;
				break;
			case 's':	/// short
			case 'S':	/// ushort
				if (id == *(uint16_t*)(data))
					is_equal = true;
				break;
			case 'i':	/// int
			case 'I':	/// uint
				if (id == *(uint32_t*)(data))
					is_equal = true;
				break;
			case 'l':	/// long
			case 'L':	/// ulong
				if (id == *(uint64_t*)(data))
					is_equal = true;
				break;
			case 'f':	/// float
				printf("error key");
				break;
			case 'd':	/// double
				printf("error key");
				break;
			case 'x':	/// 定长字符串
				printf("error key");
				break;
			case 'V':	/// 变长字符串
				printf("error key");
				break;
			case 'v':	/// 变长字符串
				printf("error key");
				break;
			case '|':	/// 分隔符
				printf("error key");
				break;
			default:
				printf("error key");
				break;
			}

			if (!is_equal)
				continue;
		}

		uint32_t size = 0;

		printf("\033[32m");
		std::string str;

		for (uint32_t k = 0; k < descseg.size(); ++k)
		{
			switch (descseg[k])
			{
			case 'b':	/// sbyte
			case 'B':	/// byte
				printf("%-8d", *(uint8_t*)(data + size));
				size += 1;
				break;
			case 's':	/// short
			case 'S':	/// ushort
				printf("%-8d", *(uint16_t*)(data + size));
				size += 2;
				break;
			case 'i':	/// int
			case 'I':	/// uint
				printf("%-10d", *(uint32_t*)(data + size));
				size += 4;
				break;
			case 'l':	/// long
			case 'L':	/// ulong
				printf("%-10ld", *(uint64_t*)(data + size));
				size += 8;
				break;
			case 'f':	/// float
				printf("%-10f", *(float*)(data + size));
				size += 4;
				break;
			case 'd':	/// double
				printf("%-10f", *(double*)(data + size));
				size += 8;
				break;
			case 'x':	/// 定长字符串
			{
				const char* pnumber = descseg.c_str() + i + 1;
				int fixstrsize = 0;
				char* pend = NULL;

				if (StringTool::try_parse_int(pnumber, &fixstrsize, &pend) == false || fixstrsize < 0)
				{
                    return false;
				}

				size += fixstrsize;
				i += (pend - pnumber);
				printf("error");
			}
			break;
			case 'V':	/// 变长字符串
				str = *(const char**)(data + size);
				printf("%-20s", str.c_str());
				size += 8;
				break;
			case 'v':	/// 变长字符串
				printf("error");
				size += 4;
				break;
			case '|':	/// 分隔符
				break;
			default:
				return 0;
				break;
			}
		}

		printf("\033[0m\n");
	}

	return true;
}
