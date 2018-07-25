#include "ParseDropItem.hpp"
#include "CsvReader.hpp"

string StrUtil::Replace(const string& str, const string& src, const string& dest)
{
	string ret;

	string::size_type pos_begin = 0;
	string::size_type pos = str.find(src);
	while (pos != string::npos)
	{
		ret.append(str.data() + pos_begin, pos - pos_begin);
		ret += dest;
		pos_begin = pos + src.length();
		pos = str.find(src, pos_begin);
	}
	if (pos_begin < str.length())
	{
		ret.append(str.begin() + pos_begin, str.end());
	}
	return ret;
}

string StrUtil::TrimStr(const string& src, const char* charsTrim)
{
	string retStr;
	for (size_t i = 0; i < src.length(); ++i)
	{
		bool isFound = false;
		for (int k = 0; charsTrim[k] != 0; ++k)
		{
			if (src[i] == charsTrim[k])
			{
				isFound = true;
				break;
			}
		}
		if (!isFound)
			retStr.push_back(src[i]);
	}
	return retStr;
}

string StrUtil::TrimEndStr(const string& src, const char* charsTrim)
{
	int start = 0;
	int end = src.length();
	for (; start < end; ++start)
	{
		bool isFound = false;
		for (int k = 0; charsTrim[k] != 0; ++k)
		{
			if (src[start] == charsTrim[k])
			{
				isFound = true;
				break;
			}
		}

		if (!isFound)
			break;
	}

	for (--end; end >= start; --end)
	{
		bool isFound = false;
		for (int k = 0; charsTrim[k] != 0; ++k)
		{
			if (src[end] == charsTrim[k])
			{
				isFound = true;
				break;
			}
		}
		if (!isFound)
			break;
	}
	++end;
	if (end > start)
	{
		return src.substr(start, end);
	}
	return "";
}


vector<string> ParseDropItem::SpliteStr(string src, char seprator)
{
	vector<string> retList;
	string temp;
	int inGuohao = 0;
	for (size_t i = 0; i < src.length(); ++i)
	{
		if (src[i] == '(')
		{
			inGuohao++;
			if (inGuohao == 1)
			{
				continue;
			}
		}
		if (src[i] == ')' && inGuohao == 1)
		{
			inGuohao--;
			continue;
		}
		if (inGuohao > 0)
		{
			temp += src[i];
		}
		else
		{
			if (src[i] == seprator)
			{
				if (temp.length() > 0)
				{
					retList.push_back(temp);
				}
				temp.clear();
			}
			else
			{
				temp += src[i];
			}
		}

		if (src[i] == ')')
		{
			inGuohao--;
		}
	}
	if (temp.length() > 0)
	{
		retList.push_back(temp);
	}
	return retList;
}





vector<ItemStructDrop> ParseDropItem::GetZhuangbeiDropList(string diaoluo, int zheyeItemXuhao)
{
	diaoluo = StrUtil::Replace(diaoluo, "，", ",");
	char trimsArr[] = { '\t',' ' };
	diaoluo = StrUtil::TrimStr(diaoluo, trimsArr);

	vector<string> itemsList;
	vector<ItemStructDrop> itemDropList;
	string temp;
	for (size_t i = 0; i < diaoluo.length(); ++i)
	{
		if (diaoluo[i] == '\r' || diaoluo[i] == '\n' || diaoluo[i] == '|')
			break;
		if (diaoluo[i] == ' ' || diaoluo[i] == '\t' || diaoluo[i] == '%')
		{
			continue;
		}
		if (diaoluo[i] == '[' || diaoluo[i] == ']')
		{
			temp.clear();
			continue;
		}
		if (diaoluo[i] == '{')
		{
			temp.clear();
			continue;
		}
		if (diaoluo[i] == '}')
		{
			if (temp.length() > 0)
			{
				itemsList.push_back(temp);
				temp.clear();
			}
			continue;
		}
		temp += diaoluo[i];
	}

	for (size_t i = 0; i < itemsList.size(); ++i)
	{
		string item = itemsList[i];
		vector<string> itemDropStr = SpliteStr(item, ',');
		if (itemDropStr.size() < 2)
			continue;
		ItemStructDrop itemDrop;
		vector<string> tempDropsByZheYe;
		if (itemDropStr[0].find_first_of(';') > 0)
		{
			tempDropsByZheYe = SpliteStr(itemDropStr[0], ';');
		}
		else
		{
			tempDropsByZheYe = SpliteStr(itemDropStr[0], ',');
		}
		if (tempDropsByZheYe.size() > static_cast<size_t>(zheyeItemXuhao))
		{
			itemDrop.itemBaseId = atoi(tempDropsByZheYe[zheyeItemXuhao].c_str());
		}
		else
			itemDrop.itemBaseId = atoi(tempDropsByZheYe[0].c_str());
		itemDrop.num = atoi(itemDropStr[1].c_str());
		if (itemDropStr.size() >= 3)
		{
			itemDrop.percent = atoi(itemDropStr[2].c_str());;
		}
		else
		{
			itemDrop.percent = 0;
		}

		itemDropList.push_back(itemDrop);
	}

	return itemDropList;

}
int ParseDropItem::GetZhiyeXuhao(int CreatureTypeId)
{
	if (CreatureTypeId == 1)
		return 0;
	if (CreatureTypeId == 4)
		return 1;
	if (CreatureTypeId == 2)
		return 2;
	return 0;
}