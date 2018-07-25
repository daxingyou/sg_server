#ifndef __GAME_MAP_COMMON_H__
#define __GAME_MAP_COMMON_H__

#include "macros.hpp"

#include <vector>

/**
 * \brief ����һЩ��ͼ����Ϣ
 */

/**
* \brief ����
*/
#define SCREEN_WIDTH	20

/**
* \brief ����
*/
#define SCREEN_HEIGHT	20

/**
 * \brief ��Ч��������
 */
#define INVALID_INDEX	(uint32_t)-1

// �ƶ�����Ķ���
enum {
	_DIR_UP = 0,			// ����
	_DIR_UPRIGHT = 1,		// ����
	_DIR_RIGHT = 2,			// ����
	_DIR_RIGHTDOWN = 3,		// ����
	_DIR_DOWN = 4,			// ����
	_DIR_DOWNLEFT = 5,		// ����
	_DIR_LEFT = 6,			// ����
	_DIR_LEFTUP = 7,		// ����

	_DIR_MAX = 8,			// ���
	_DIR_NOT_CROSS = 9		// ������������
};

/**
* \brief ���Ӷ���
*/
#define GRID_STAT_BLOCK		0x00    // ��̬�赲
#define GRID_NONE_BLOCK		0x01    // �����ߵ�
#define GRID_SYNC_BLOCK		0x02	// ��̬�赲��

#pragma  pack(1)

/**
* \brief ��ͼ�ļ�ͷ�ṹ����
*/
struct map_file_header
{
	uint32_t ver;			/**< �汾 MAP_VERSION */
	uint32_t width;			/**< ��� */
	uint32_t height;		/**< �߶� */
};

/**
* \brief �������ݽṹ ��8Ϊ���赲��Ϣ ��24δ��������Ϣ
*/

struct grid_tile_t
{
	union {
		struct {
			uint16_t flags;	// �������ԣ�1�������� >1 ����id��
			uint8_t none;	// ��Ч
			uint8_t type;	// ��������(Ŀǰû����,û�������Ϣ)
		};
		uint32_t value;
	};

	grid_tile_t() : value(GRID_STAT_BLOCK) {};
};

#pragma pack()

typedef std::vector<grid_tile_t> grid_tiles;

/**
* \brief ��������
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
