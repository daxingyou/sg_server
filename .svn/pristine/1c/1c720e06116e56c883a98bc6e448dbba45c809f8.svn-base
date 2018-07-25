#include <stdio.h>   

#include "tbl_reader.h"
#include "log.hpp"

int main(int argc, char** argv)
{
	tbl_reader_t m_tbl_reader;

	if (argc == 2)
	{
		m_tbl_reader.read_tbl(argv[1]);
	}
	else if (argc == 3)
	{
		m_tbl_reader.read_tbl(argv[1], atoi(argv[2]));
	}
	else
	{
		printf("\033[31m Error Input! \033[0m\n");
	}

    return 0;
}
