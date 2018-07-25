/**
* 文件类，用于文件内容读取和写入
* @author will
*/
#ifndef __CT_FILE_H__
#define __CT_FILE_H__ 

#include <stdio.h>

namespace common
{

class CTFile
{
public:
    CTFile()
        :m_pstFile(NULL)
    {

    }

    virtual ~CTFile()
    {
        if (m_pstFile)
        {
            fclose(m_pstFile);
        }
    }
    
    bool HasOpened()
    {
        return (m_pstFile != NULL);
    }

    int Pos()
    {
        if (m_pstFile)
        {
            return ftell(m_pstFile);
        }

        return 0;
    }

    int Seek(int iPos)
    {
        if (m_pstFile)
        {
            return fseek(m_pstFile, iPos, SEEK_SET);
        }
        return -1;
	}

	void CloseFile()
	{
		if (m_pstFile != NULL)
		{
			fclose(m_pstFile);
			m_pstFile = NULL;
		}
	}

protected:
    FILE* m_pstFile;
};

class CTReadFile : public CTFile
{
public:
    CTReadFile()
    {
        m_iBufLen = 10240;
        m_sBuf = (char*)malloc(m_iBufLen);
    }

    virtual ~CTReadFile()
    {
        free(m_sBuf);
    }

    int Open(const char* szFilePos)
    {
        if (m_pstFile)
        {
            fclose(m_pstFile);
        }

        m_pstFile = fopen(szFilePos, "rb");
        if (m_pstFile == NULL)
        {
            return -1;
        }
        
        return 0; 
    }

    //读取一行，以delim结尾
    int Read(char delim)
    {
        if (m_pstFile == NULL)
        {
            return -1;
        }

        int iNowPos = Pos();  //记录当前位置

        m_iReadSize = (int)getdelim(&m_sBuf, &m_iBufLen, delim, m_pstFile); 
        if (m_iReadSize < 0)
        {
            m_iReadSize = 0;
            return -2;
        }

        if (m_sBuf[m_iReadSize - 1] != (char)delim) //一行没读完文件就结束了
        {
            Seek(iNowPos);
            return -3;
        }
        
        return 0;
    }

	int Read()
	{
		if (m_pstFile == NULL)
		{
			return -1;
		}

		//int iNowPos = Pos();  //记录当前位置

		m_iReadSize = (int)getline(&m_sBuf, &m_iBufLen, m_pstFile); 
		if (m_iReadSize < 0)
		{
			m_iReadSize = 0;
			return -2;
		}

		return 0;
	}

	int ReadAll()
	{
		if (m_pstFile == NULL)
		{
			return -1;
		}

		fseek(m_pstFile, 0L, SEEK_END);
		int nTotalSize = (int)ftell(m_pstFile);

		fseek(m_pstFile, 0L, SEEK_SET);

		m_iReadSize = fread(m_sBuf, 1, nTotalSize, m_pstFile);
		if (m_iReadSize <= 0)
		{
			m_iReadSize = 0;
			return -2;
		}

		return 0;
	}

    char* GetBuf()
    {
        return m_sBuf;
    }

    int ReadSize()
    {
        return (int)m_iReadSize; 
    }

    //读取一行，以\n结尾
    char* ReadLine()
    {
        int iRet = Read('\n'); 
        if (iRet != 0)
        {
            return NULL;
        }

        char* sTemp = GetBuf();
        if (sTemp[ReadSize()-1] != '\n')
        {
            return NULL;
        }
        
        sTemp[ReadSize()-1] = '\0';
        return sTemp;
    }


private:
    size_t m_iBufLen;
    char* m_sBuf;
    int m_iReadSize;
};

class CTWriteFile : public CTFile
{
public:
    CTWriteFile()
    {

    }

    virtual ~CTWriteFile()
    {

    }

    int Open(const char* szFilePos, bool bAppend = false)
    {
        if (m_pstFile)
        {
            return 0;
        }
        
        if (bAppend)
        {
            m_pstFile = fopen(szFilePos, "ab+");
        }
        else
        {
            m_pstFile = fopen(szFilePos, "wb");
        }
        
        if (m_pstFile == NULL)
        {
            return -1;
        }

        return 0; 
    }

    int Write(const char* szBuf, int iLen, char delim)
    {
        if (szBuf == NULL || iLen <= 0 || m_pstFile == NULL)
        {
            return -1;
        }
        
        int iWriteSize = fwrite(szBuf, sizeof(char), iLen, m_pstFile);
        if (iWriteSize != iLen)
        {
            return -2;
        }
        
        iWriteSize = fwrite(&delim, sizeof(char), 1, m_pstFile);
        if (iWriteSize != 1)
        {
            return -3;
        }

        fflush(m_pstFile);
        return 0;
    }

    int Write(const char* szBuf, int iLen)
    {
        if (szBuf == NULL || iLen <= 0 || m_pstFile == NULL)
        {
            return -1;
        }

        int iWriteSize = fwrite(szBuf, sizeof(char), iLen, m_pstFile);
        if (iWriteSize != iLen)
        {
            return -2;
        }

        fflush(m_pstFile);
        return 0;
    }

    int WriteLine(const char* szLine)
    {
        return Write(szLine, strlen(szLine), '\n');
    }

};

}

#endif
