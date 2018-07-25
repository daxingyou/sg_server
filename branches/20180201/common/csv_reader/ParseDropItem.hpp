

#ifndef _ParseDropItem_H
#define _ParseDropItem_H


#include <sstream>
#include <string>
#include <vector>
#include "Singleton.h"
using namespace std;

namespace StrUtil
{
	string Replace(const string& str, const string& src, const string& dest);
	string TrimStr(const string& src, const char* charsTrim);
	string TrimEndStr(const string& src, const char* charsTrim);
}

class ItemStructDrop
{
public:
	int itemBaseId;
	int num;
	int percent;
};

class ParseDropItem
{
	static vector<string> SpliteStr(string src, char seprator);
public:
	
	static vector<ItemStructDrop> GetZhuangbeiDropList(string diaoluo, int zheyeItemXuhao=0);

	static int GetZhiyeXuhao(int CreatureTypeId);
};

#endif

