#pragma once
#include "macros.hpp"

// 存储set主键
static const std::string SAVE_SET_NAME = "SAVE_DB_SET";
// 所有数据主键
static const std::string PRIMARY_KEY = "rk";
// 所有数据失效键
static const std::string INVALID_KEY = "invalid";
// 最大数据数据
static const size_t CACHE_VALUEX_LEN_MA = 1024 * 256; // 256K
// db启动状态字段
static const std::string REDIS_DB_STATE = "REDIS_STATE";

enum redis_server_state : uint32_t
{
	redis_server_none		= 0,	// redis 未初始化状态
	redis_server_init		= 1,	// redis 正在初始化状态
	redis_server_runing		= 2,	// redis 正在运行
	redis_server_closing	= 3,	// redis 正在关闭
};

enum DB_TYPE : uint32_t
{
	DB_NONE,		/**< 未定义类型 */
	DB_DWORD,		/**< DWORD类型 4字节长度 */
	DB_QWORD,		/**< QWORD类型 8字节长度 */
	DB_FLOAT,		/**< FLOAT类型 8字节长度 */
	DB_STR,			/**< 字符串类型 */
	DB_BIN,			/**< 二进制数据类型 */
};

typedef struct
{
	const char *table;		/**< 数据库表名 */
	bool is_load;			/**< 是否起服加载数据 */
	const char *where;		/**< 对于起服不加载的数据加载方式 */
	const char *fields;		/**< 建立索引字段 */
} sql_table;
