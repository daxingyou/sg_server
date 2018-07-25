#ifndef __COMMON_COUNTRY_CODE__
#define __COMMON_COUNTRY_CODE__
#include "macros.hpp"

enum officer_type : uint32_t
{
	officer_type_none = 0, // 默认值 
	officer_type_king = 101, // 国王 
	officer_type_prime_minister = 102, // 丞相 
	officer_type_general = 103, // 大将军 
	officer_type_chief_secretariat = 104, // 尚书令 
	officer_type_agile_cavalry_general = 105, // 骠骑将军 

};

enum privilige_type : uint32_t
{
	country_appoint_chenxiang = 101, // 官职-任免丞相 
	country_appoint_dajiangjun = 102, // 官职-任免大将军 
	country_appoint_shangshuling = 103, // 官职-任免尚书令 
	country_appoint_biaoqijiangjun = 104, // 官职-任免骠骑将军 
	country_invite_family_join = 201, // 邀请家族同盟 
	country_edit_title = 202, // 修改国家年号 
	country_edit_announce = 203, // 修改国家公告 

};

#endif
