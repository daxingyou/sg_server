#ifndef __GAME_MAP_COMMON_H__
#define __GAME_MAP_COMMON_H__

#include "macros.hpp"

#include <vector>

/**
 * \brief 定义一些地图的信息
 */

/**
* \brief 屏宽
*/
#define SCREEN_WIDTH	20

/**
* \brief 屏高
*/
#define SCREEN_HEIGHT	20

/**
 * \brief 无效的屏索引
 */
#define INVALID_INDEX	(uint32_t)-1

// 移动方向的定义
enum {
	_DIR_UP = 0,			// 向上
	_DIR_UPRIGHT = 1,		// 右上
	_DIR_RIGHT = 2,			// 向右
	_DIR_RIGHTDOWN = 3,		// 右下
	_DIR_DOWN = 4,			// 向下
	_DIR_DOWNLEFT = 5,		// 左下
	_DIR_LEFT = 6,			// 向左
	_DIR_LEFTUP = 7,		// 左上

	_DIR_MAX = 8,			// 最大
	_DIR_NOT_CROSS = 9		// 两个屏不交叉
};

/**
* \brief 格子定义
*/
#define GRID_STAT_BLOCK		0x00    // 静态阻挡
#define GRID_NONE_BLOCK		0x01    // 可行走点
#define GRID_SYNC_BLOCK		0x02	// 动态阻挡点

#pragma  pack(1)

/**
* \brief 地图文件头结构定义
*/
struct map_file_header
{
	uint32_t ver;			/**< 版本 MAP_VERSION */
	uint32_t width;			/**< 宽度 */
	uint32_t height;		/**< 高度 */
};

/**
* \brief 格字数据结构 高8为是阻挡信息 低24未是区域信息
*/

struct grid_tile_t
{
	union {
		struct {
			uint16_t flags;	// 格子属性（1，可行走 >1 区域id）
			uint8_t none;	// 无效
			uint8_t type;	// 格子类型(目前没有用,没有填充信息)
		};
		uint32_t value;
	};

	grid_tile_t() : value(GRID_STAT_BLOCK) {};
};

#pragma pack()

typedef std::vector<grid_tile_t> grid_tiles;

/**
* \brief 场景坐标
*/
struct sPos
{
	uint32_t m_x;
	uint32_t m_y;
	uint32_t m_dir;

	sPos() : m_x(0), m_y(0), m_dir(0) {};
	sPos(uint32_t x, uint32_t y, uint32_t dir) : m_x(x), m_y(y), m_dir(dir) {};
	sPos(const sPos& pos) : m_x(pos.m_x), m_y(pos.m_y), m_dir(pos.m_dir) {};

	sPos & operator= (const sPos &pos)
	{
		m_x = pos.m_x;
		m_y = pos.m_y;
		m_dir = pos.m_dir;
		return *this;
	}

	const bool operator== (const sPos &pos) const
	{
		return (m_x == pos.m_x && m_y == pos.m_y && m_dir == pos.m_dir);
	}

	const bool operator != (const sPos &pos) const
	{
		return !(*this == pos);
	}

	const bool is_zero() const
	{
		return (m_x == 0 && m_y == 0 && m_dir == 0);
	}
};

#endif
