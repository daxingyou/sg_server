#include "CsvParse.hpp"
#include "CsvReader.hpp"
#include "ParseDropItem.hpp"
#include "log.hpp"

string CsvParse::GetFieldString(int m, int n)//m ÐÐ n ÁÐ
{
	if( m < 0|| m >= static_cast<int>(data.size()))
	{
		return "";
	}
	if (n < 0 || n >= static_cast<int>(data[m].size()))
		return "";
	else
		return data[m][n];
}

float CsvParse::GetFieldFloat(int m, int n)
{
	return atof(GetFieldString(m,n).c_str());
}
int CsvParse::GetFieldInt(int m, int n)
{
	return atoi(GetFieldString(m,n).c_str());
}
string CsvParse::GetStringValueById(int Id, const string& columnName)
{
	size_t i = 0;
	for (; i < data[0].size(); i++) {
		if(  data[0][i] == columnName)
		{
			break;
		}
	}
	if (i >= data[0].size())
	{
		return "";
	}
	for (size_t k=1; k<data.size(); k++) {
		if( atoi(data[k][0].c_str()) == Id )
		{
			return data[k][i];
		}
	}
	return "";
}
int CsvParse::GetRowByIntColumnValue(string columnName1, int value1, string columnName2, int value2)
{
	size_t i = 0;
	for (; i < data[0].size(); i++) {
		if(  data[0][i] == columnName1)
		{
			break;
		}
	}
	if( i >= data[0].size())
		return 0;
	if( columnName2.length() > 0)
	{
		size_t j = 0;
		for (; j < data[0].size(); j++) {
			if(  data[0][j] == columnName2)
			{
				break;
			}
		}
		if( j >= data[0].size())
			return 0;
		for (size_t k=1; k<data.size(); k++) {
			if( atoi(data[k][i].c_str()) == value1 
				&& atoi(data[k][j].c_str()) == value2)
			{
				return k;
			}
		}
	}
	else
	{
		for (size_t k=1; k<data.size(); k++) {
			if( atoi(data[k][i].c_str()) == value1 )
			{
				return k;
			}
		}
	}
	
	return 0;
}

char* CsvParse::FOpen(const char* filepth)
{
	char* buf = NULL;
	/*char configPath[256] = { 0 };
	snprintf(configPath, sizeof(configPath), "%s/%s", env::xml->config_path.c_str(), filename);*/
	FILE* file = 0;
#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
	// fopen_s( &file, configPath, "rb" );
#else
	file = fopen(filepth, "rb" );
#endif
	if (file == NULL)
	{
		log_error("open path %s", filepth);
		return buf;
	}

	do
	{
		long length = 0;
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fseek(file, 0, SEEK_SET);

		// Strange case, but good to handle up front.
		if (length <= 0)
		{
			log_error("open path %s size=%d", filepth, (int)length);
			break;
		}

		buf = new char[length + 1];
		buf[0] = 0;
		buf[length] = 0;
		int readLen = 0;
		if ((readLen = fread(buf, length, 1, file)) <= 0) {
			log_error("open path lenth=%d", readLen);
			delete[] buf;
			buf = NULL;
		}
	} while (false);

	fclose(file);
	return buf;
}
//"./config/playerLevel.csv"

bool CsvParse::LoadFile(const string& path, const string& split)
{
	maxX = 0;
	maxY = 0;
	
	char* buf = FOpen(path.c_str());
	if(buf == NULL)
	{
		log_error("open CsvParse path %s", path.c_str());
		return false;
	}
	
	stringstream fin;
	fin<<buf;
	
	CsvReader csvreader(fin, split);
	
	string s;  
	while( csvreader.getline(s) && s.length() >= 2)
	{   
		int tempmaxX = csvreader.getnfield();
		if (maxX != tempmaxX)
		{
			if (maxX != 0)
			{
				log_error("maxX=%d tempmaxX=%d", maxX, tempmaxX);
			}
			maxX = tempmaxX;
		}
		data.push_back(csvreader.field);
		maxY++;
	}
	delete[] buf;
	if (data.size()<= 1)
	{
		log_error("data size=%d",data.size());
		return false;
	}
	else
	{
		log_error("data size=%d,%d",data.size(),data[0].size());
	}
	
	for (size_t i=0; i<data.size(); i++) {
		for (size_t k=0; k<data[i].size(); k++) {
			data[i][k] = StrUtil::Replace(data[i][k], "£¬", ",");
			char trimsArr[] = {'\t',' '};
			data[i][k] = StrUtil::TrimEndStr(data[i][k], trimsArr);
		}
	}
	return true;
}
