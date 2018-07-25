#include "sMisc.hpp"

float my_sqrt(const spos& pCur, const spos& pTar)
{
	return sqrt((pCur.m_x - pTar.m_x)*(pCur.m_x - pTar.m_x) +
		(pCur.m_z - pTar.m_z)*(pCur.m_z - pTar.m_z));
}

float my_angle(const spos& pCur, const spos& pTar)
{
	//double Sqrt = sqrt( (pCur->m_fX-pTar->m_fX)*(pCur->m_fX-pTar->m_fX)+
	//					(pCur->m_fZ-pTar->m_fZ)*(pCur->m_fZ-pTar->m_fZ) ) ;
	//double Acos = acos((pTar->m_fZ-pCur->m_fZ)/ Sqrt);
	//return (FLOAT)Acos ;

	float Sqrt = sqrt((pCur.m_x - pTar.m_x)*(pCur.m_x - pTar.m_x) +
		(pCur.m_z - pTar.m_z)*(pCur.m_z - pTar.m_z));

	if (Sqrt <= 0.001f) return 0.0f;

	float fACos = (pTar.m_z - pCur.m_z) / Sqrt;
	if (fACos > 1.0) fACos = 0.0;
	if (fACos < -1.0) fACos = __PI;

	fACos = ::acos(fACos);

	// [0~180]
	if (pTar.m_x >= pCur.m_x)
		return (float)fACos;
	//(180, 360)
	else
		return (float)(2 * __PI - fACos);
}