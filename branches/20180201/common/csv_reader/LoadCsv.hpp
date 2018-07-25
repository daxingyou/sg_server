

#ifndef _LoadCsv_H
#define _LoadCsv_H


#include <string>
#include <vector>
#include "Singleton.h"
#include "CsvParse.hpp"

using namespace std;

class LoadCsv : public SingletonBase<LoadCsv>
{

public:
	CsvParse roleLevelRead;

	CsvParse vipLevelReader;
	CsvParse itemsReader;
	bool Init();
};

#endif

