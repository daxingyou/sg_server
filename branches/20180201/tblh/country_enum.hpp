#ifndef __COMMON_COUNTRY_CODE__
#define __COMMON_COUNTRY_CODE__
#include "macros.hpp"

enum officer_type : uint32_t
{
	officer_type_none = 0, // Ĭ��ֵ 
	officer_type_king = 101, // ���� 
	officer_type_prime_minister = 102, // ة�� 
	officer_type_general = 103, // �󽫾� 
	officer_type_chief_secretariat = 104, // ������ 
	officer_type_agile_cavalry_general = 105, // ���ｫ�� 

};

enum privilige_type : uint32_t
{
	country_appoint_chenxiang = 101, // ��ְ-����ة�� 
	country_appoint_dajiangjun = 102, // ��ְ-����󽫾� 
	country_appoint_shangshuling = 103, // ��ְ-���������� 
	country_appoint_biaoqijiangjun = 104, // ��ְ-�������ｫ�� 
	country_invite_family_join = 201, // �������ͬ�� 
	country_edit_title = 202, // �޸Ĺ������ 
	country_edit_announce = 203, // �޸Ĺ��ҹ��� 

};

#endif
