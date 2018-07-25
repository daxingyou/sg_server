

#ifndef _CsvParse_
#define _CsvParse_


#include <sstream>
#include <string>
#include <vector>

using namespace std;

class CsvParse
{
public:
	int maxX;
	int maxY;
	vector<vector<string> > data;
	static char* FOpen(const char* filepth);
	bool LoadFile(const string& path, const string& split);
	string GetStringValueById(int Id, const string& columnName);
	int GetRowByIntColumnValue(string columnName1, int value1, string columnName2="", int value2=-1);
	string GetFieldString(int m, int n);
	float GetFieldFloat(int m, int n);
	int GetFieldInt(int m, int n);
};

#endif

