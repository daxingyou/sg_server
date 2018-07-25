#include "iconv.h"
#include "stdarg.h"
#include "StringTool.h"

StringTool::StringTool(void)
{
}

StringTool::~StringTool(void)
{
}

int StringTool::ELFhash( const std::string& key )
{ 
	unsigned long h=0;  
	for( int i = 0; i < ( int )key.size(); ++i )
	{
		h = ( h<<4 ) + key.at(i);
		unsigned long g = h & 0xF0000000;
		if(g) 
			h ^= g >> 24;  
		h &= ~g; 
	}
	return h % 0x7FFFFFFF;
}

std::string StringTool::trim(const std::string& str)
{
	std::string::size_type pos = str.find_first_not_of(' ');    
	if ( pos == std::string::npos )    
	{        
		return str;   
	}    
	std::string::size_type pos2 = str.find_last_not_of(' ');    
	if ( pos2 != std::string::npos )    
	{        
		return str.substr(pos, pos2 - pos + 1);   
	}    
	return str.substr( pos );
}

std::string StringTool::replace(const std::string& str, const std::string& src, const std::string& dest)
{
    std::string ret;    
	std::string::size_type pos_begin = 0;    
	std::string::size_type pos = str.find(src);    
	while (pos != std::string::npos)    
	{        
		//cout <<"replacexxx:" << pos_begin <<" " << pos <<"\n";        
		ret.append(str.data() + pos_begin, pos - pos_begin);        
		ret += dest;        
		pos_begin = pos + 1;        
		pos = str.find(src, pos_begin);    
	}    
	if (pos_begin < str.length())    
	{        
		ret.append(str.begin() + pos_begin, str.end());    
	}    
	return ret;
}

int StringTool::split(const std::string& str, std::vector<std::string>& ret, const std::string sep )
{
    if ( str.empty() )    
	{        
		return 0;    
	}    
	std::string tmp;    
	std::string::size_type pos_begin = str.find_first_not_of(sep);    
	std::string::size_type comma_pos = 0;    
	while ( pos_begin != std::string::npos )    
	{        
		comma_pos = str.find(sep, pos_begin);        
		if ( comma_pos != std::string::npos )        
		{            
			tmp = str.substr(pos_begin, comma_pos - pos_begin);            
			pos_begin = comma_pos + sep.length();        
		}        
		else        
		{            
			tmp = str.substr(pos_begin);            
			pos_begin = comma_pos;        
		}        
		if ( !tmp.empty() )        
		{            
			ret.push_back(tmp);            
			tmp.clear();        
		}    
	}    
	return ( int )ret.size();
}

bool StringTool::try_parse_int( const char* str, int *ret, char** end )
{
	char* p = NULL;
	long n = 0;
	if( str == NULL || ret == NULL )
		goto L_ErrorEnd;

	n = strtol( str, &p, 0 );
	if( n == 0 )
	{
		if( p == str )
			goto L_ErrorEnd;
	}
	if( end != NULL )
		*end = p;
	*ret = n;
	return true;
L_ErrorEnd:
	if( end != NULL )
		*end = const_cast< char* >( str );
	if( ret != NULL )
		*ret = 0;
	return false;
}

std::string StringTool::base64_encrypt( const unsigned char* Data,const int DataByte )
{
    //编码表
    const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    //返回值
	std::string strEncode;
    unsigned char Tmp[4]={0};
    int LineLength=0;
    for(int i=0;i<(int)(DataByte / 3);i++)
    {
        Tmp[1] = *Data++;
        Tmp[2] = *Data++;
        Tmp[3] = *Data++;
        strEncode+= EncodeTable[Tmp[1] >> 2];
        strEncode+= EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
        strEncode+= EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
        strEncode+= EncodeTable[Tmp[3] & 0x3F];
        if(LineLength+=4,LineLength==76) {strEncode+="\r\n";LineLength=0;}
    }
    //对剩余数据进行编码
    int Mod=DataByte % 3;
    if(Mod==1)
    {
        Tmp[1] = *Data++;
        strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
        strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4)];
        strEncode+= "==";
    }
    else if(Mod==2)
    {
        Tmp[1] = *Data++;
        Tmp[2] = *Data++;
        strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
        strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
        strEncode+= EncodeTable[((Tmp[2] & 0x0F) << 2)];
        strEncode+= "=";
    }
    
    return strEncode;
}

std::string StringTool::base64_decrypt(const char* Data, const int DataByte,int& OutByte)
{
    //解码表
    const char DecodeTable[] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        62, // '+'
        0, 0, 0,
        63, // '/'
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
        0, 0, 0, 0, 0, 0,
        26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
        39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
    };
    //返回值
	std::string strDecode;
    int nValue;
    int i= 0;
    while (i < DataByte)
    {
        if (*Data != '\r' && *Data!='\n')
        {
            nValue = DecodeTable[(unsigned char)*Data++] << 18;
            nValue += DecodeTable[(unsigned char)*Data++] << 12;
            strDecode+=(nValue & 0x00FF0000) >> 16;
            OutByte++;
            if (*Data != '=')
            {
                nValue += DecodeTable[(unsigned char)*Data++] << 6;
                strDecode+=(nValue & 0x0000FF00) >> 8;
                OutByte++;
                if (*Data != '=')
                {
                    nValue += DecodeTable[(unsigned char)*Data++];
                    strDecode+=nValue & 0x000000FF;
                    OutByte++;
                }
            }
            i += 4;
        }
        else// 回车换行,跳过
        {
            Data++;
            i++;
        }
     }
    return strDecode;
}

/*
void StringTool::charConv( std::string& out, unsigned char *in,const char *fromEncoding,const char *toEncoding)
{
	unsigned char *o = charConv(in,fromEncoding,toEncoding);
	if (o)
	{
		out = (char *)o;
		delete[] o;
		o = NULL;
	}
}

unsigned char* StringTool::charConv( unsigned char *in,const char *fromEncoding,const char *toEncoding )
{
  unsigned char *out;
  size_t ret,size,out_size;

  size = strlen((char *)in); 
  out_size = size * 2 + 1; 
  out = new unsigned char[out_size]; 
  memset(out,0,out_size);
  if (out)
  {
    if (fromEncoding!=NULL && toEncoding!=NULL)
    {
      iconv_t icv_in = iconv_open(toEncoding,fromEncoding);
      if ((iconv_t)-1 == icv_in)
      {
        delete[] out;
        out = NULL;
      }
      else
      {
#ifdef _MSC_VER
	  const char *fromtemp = (char *)in;
#else
	  char *fromtemp = (char *)in;
#endif
        
        char *totemp =(char *)out;
        size_t tempout = out_size-1;
        ret =iconv(icv_in,&fromtemp,&size,&totemp,&tempout);
        if ((size_t)-1 == ret)
        {
          delete[] out;
          out = NULL;
        }
        iconv_close(icv_in);
      }
    }
    else
      strncpy((char *)out,(char *)in,size);
  }
  return (out);
}
*/

std::string& StringTool::tolower(std::string& str)
{
	for (std::string::iterator i = str.begin(); i != str.end(); i++)
	{
		if (*i >= 'A' && *i <= 'Z')
			*i = (*i) + ('a' - 'A');
	}
	return str;
}

std::string& StringTool::format( std::string& resultOUT, const char* fmt, ... )
{
	va_list vl;
	va_start(vl, fmt);
	vformat(resultOUT, fmt, vl);
	va_end(vl);
	return resultOUT;
}

std::string StringTool::format(const char* fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);
	std::string resultOUT;
	vformat(resultOUT, fmt, vl);
	va_end(vl);
	return resultOUT;
}

std::string& StringTool::vformat( std::string& resultOUT, const char *fmt, va_list argptr, int default_size /*= DEFAULT_BUFFER_SIZE*/ )
{
	std::vector<char> buffer(default_size, 0);
	try
	{
		for (;;)
		{
#ifndef _MSC_VER
			int len = vsnprintf(&(buffer[0]), buffer.size(), fmt, argptr);
#else
			// MSDN: formats and writes up to count characters of the given data to the memory
			// pointed to by buffer and appends a terminating null.
			int len = vsnprintf_s(&(buffer[0]), buffer.size(), buffer.size() - 1, fmt, argptr);
#endif
			if (len == -1)
			{
				buffer.resize(buffer.size() * 2, 0);
				continue;
			}
			if (len == static_cast<int>(buffer.size()))
				buffer.push_back(0);
			break;
		}

		resultOUT.assign(&(buffer[0]));
	}
	catch (std::exception& ec)
	{
		resultOUT.assign("error string!!!!!!! [%s]", ec.what());
	}

	return resultOUT;
}


std::string StringTool::vformat(const char *fmt, va_list argptr, int default_size /*= DEFAULT_BUFFER_SIZE*/ )
{
	std::string resultOUT;
	return vformat(resultOUT, fmt, argptr, default_size);
}

/*
std::string StringTool::getGB2312Str( const char* str )
{
	std::string temp;
	StringTool::charConv( temp, (unsigned char *)str, "UTF-8", "GBK" );
	return temp;
}

std::string StringTool::getUTF8Str( const char* str )
{
	std::string temp;
	StringTool::charConv( temp, (unsigned char *)str, "GBK", "UTF-8" );
	return temp;
}
*/