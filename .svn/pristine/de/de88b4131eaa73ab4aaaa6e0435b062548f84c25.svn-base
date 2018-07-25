#ifndef __SMISC_HPP__
#define __SMISC_HPP__

#include <math.h>

//////////////////////////////////////////////////////////////////////////
#define __PI (3.14159265f)

//用来定义在世界的浮点位置
struct spos
{
	float	m_x;
	float	m_z;

	spos() : m_x(0.0f), m_z(0.0f) {}
	spos(float fx, float fz) : m_x(fx), m_z(fz) {}
	void clear() {
		m_x = 0.0f;
		m_z = 0.0f;
	};

	spos& operator=(spos const& rhs)
	{
		m_x = rhs.m_x;
		m_z = rhs.m_z;
		return *this;
	}
	bool operator==(spos& Ref) const
	{
		return (fabs(m_x - Ref.m_x) + fabs(m_z - Ref.m_z)) < 0.0001f;

	}
	bool operator==(const spos& Ref) const
	{
		return (fabs(m_x - Ref.m_x) + fabs(m_z - Ref.m_z)) < 0.0001f;
	}
};

extern float my_sqrt(const spos& pCur, const spos& pTar);

extern float my_angle(const spos& pCur, const spos& pTar);

#endif
