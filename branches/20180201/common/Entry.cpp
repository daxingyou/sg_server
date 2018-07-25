
#include "Entry.h"
#include "log.hpp"
#include "StringTool.h"
#include "macros.hpp"
#include <stdarg.h>

#define MSGBUF_MAX 2048

#define FMTMessage( msg, msglen, pat ) \
	do \
{ \
	va_list ap; \
	bzero( msg, msglen ); \
	va_start( ap, pat ); \
	vsnprintf( msg, msglen - 1, pat, ap ); \
	va_end( ap ); \
}while( false )

void Entry::debug( const char* pattern, ... )
{
	char message[MSGBUF_MAX];
	FMTMessage( message, MSGBUF_MAX, pattern );
	if(m_use_gb2312_name )
	{
		log_debug( "[%s [%u, %u, %s] %s", 
			this->getClassName(), 
			this->m_id,
			this->m_tempid,
			this->getGB2312Name().c_str(), 
			message );
	}
	else
	{
		log_debug( "[%s [%u, %u, %s] %s", 
			this->getClassName(), 
			this->m_id,
			this->m_tempid,
			this->m_name,
			message );
	}

}

void Entry::error( const char* pattern, ... )
{
	char message[MSGBUF_MAX];
	FMTMessage( message, MSGBUF_MAX, pattern );
	if(m_use_gb2312_name )
	{
		log_error( "[%s [%u, %u, %s] %s", 
			this->getClassName(), 
			this->m_id,
			this->m_tempid,
			this->getGB2312Name().c_str(), 
			message );
	}
	else
	{
		log_error( "[%s [%u, %u, %s] %s", 
			this->getClassName(), 
			this->m_id,
			this->m_tempid,
			this->m_name,
			message );
	}
}

void Entry::info( const char* pattern, ... )
{
	char message[MSGBUF_MAX];
	FMTMessage( message, MSGBUF_MAX, pattern );
	if(m_use_gb2312_name )
	{
		log_info( "[%s [%u, %u, %s] %s", 
			this->getClassName(), 
			this->m_id,
			this->m_tempid,
			this->getGB2312Name().c_str(), 
			message );
	}
	else
	{
		log_info( "[%s [%u, %u, %s] %s", 
			this->getClassName(), 
			this->m_id,
			this->m_tempid,
			this->m_name,
			message );
	}
}

void Entry::fatal( const char* pattern, ... )
{
	char message[MSGBUF_MAX];
	FMTMessage( message, MSGBUF_MAX, pattern );
	if(m_use_gb2312_name )
	{
		log_error( "[%s [%u, %u, %s] %s", 
			this->getClassName(), 
			this->m_id,
			this->m_tempid,
			this->getGB2312Name().c_str(), 
			message );
	}
	else
	{
		log_error( "[%s [%u, %u, %s] %s", 
			this->getClassName(), 
			this->m_id,
			this->m_tempid,
			this->m_name,
			message );
	}
}

void Entry::warn( const char* pattern, ... )
{
	char message[MSGBUF_MAX];
	FMTMessage( message, MSGBUF_MAX, pattern );
	if(m_use_gb2312_name )
	{
		log_warn( "[%s [%u, %u, %s] %s", 
			this->getClassName(), 
			this->m_id,
			this->m_tempid,
			this->getGB2312Name().c_str(), 
			message );
	}
	else
	{
		log_warn( "[%s [%u, %u, %s] %s", 
			this->getClassName(), 
			this->m_id,
			this->m_tempid,
			this->m_name,
			message );
	}	
}

std::string Entry::getGB2312Name()
{
	/*
	std::string temp;
	StringTool::charConv(temp, (unsigned char *)this->m_name, "UTF-8", "GBK");
	return temp;
	*/

	return get_name();
}
