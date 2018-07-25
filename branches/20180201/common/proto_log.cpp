#include "proto_log.hpp"
#include "log.hpp"
void SelfGameLogHandler(google::protobuf::LogLevel level, const char* filename, int line,
	const std::string& message)
{
	static char* temp = NULL;
	if (temp == NULL)
	{
		temp = new char[1000];
	}
	// We use fprintf() instead of cerr because we want this to work at static
	// initialization time.
	sprintf(temp, "proto [%s:%d] %s",
		filename, line, message.c_str());
	if( level == google::protobuf::LOGLEVEL_ERROR)
	{
		log_error(temp);
	}
	else if( level == google::protobuf::LOGLEVEL_INFO)
	{
		log_info(temp);
	}
	else if( level == google::protobuf::LOGLEVEL_FATAL)
	{
		log_error(temp);
	}
	else if( level == google::protobuf::LOGLEVEL_WARNING)
	{
		log_warn(temp);
	}
	else
	{
		log_trace(temp);
	}
}
