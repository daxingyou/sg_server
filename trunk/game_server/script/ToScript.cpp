/*
** Lua binding: ToScript
** Generated automatically by tolua++-1.0.92 on Wed Jul 25 20:46:58 2018.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_ToScript_open (lua_State* tolua_S);

#include "script/game_script_func.h"
#include "script/script_role.h"
#include "script/script_fight_hero.h"
#include "script/script_dungeon.h"
#include "../tblh/attr_enum.hpp"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_script_fight_hero_t (lua_State* tolua_S)
{
 script_fight_hero_t* self = (script_fight_hero_t*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_script_dungeon_t (lua_State* tolua_S)
{
 script_dungeon_t* self = (script_dungeon_t*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_script_role_t (lua_State* tolua_S)
{
 script_role_t* self = (script_role_t*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"script_fight_hero_t");
 tolua_usertype(tolua_S,"ScriptProxy<role_t>");
 tolua_usertype(tolua_S,"script_role_t");
 tolua_usertype(tolua_S,"script_dungeon_t");
 tolua_usertype(tolua_S,"ScriptProxy<fight_hero_t>");
 tolua_usertype(tolua_S,"ScriptProxy<dungeon_scene_t>");
}

/* function: c_debug */
#ifndef TOLUA_DISABLE_tolua_ToScript_c_debug00
static int tolua_ToScript_c_debug00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   c_debug(msg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_debug'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: c_info */
#ifndef TOLUA_DISABLE_tolua_ToScript_c_info00
static int tolua_ToScript_c_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   c_info(msg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: c_error */
#ifndef TOLUA_DISABLE_tolua_ToScript_c_error00
static int tolua_ToScript_c_error00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   c_error(msg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_error'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: c_assert_fail */
#ifndef TOLUA_DISABLE_tolua_ToScript_c_assert_fail00
static int tolua_ToScript_c_assert_fail00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   c_assert_fail(msg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_assert_fail'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: c_get_root_dir */
#ifndef TOLUA_DISABLE_tolua_ToScript_c_get_root_dir00
static int tolua_ToScript_c_get_root_dir00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  c_get_root_dir();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_root_dir'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: c_do_file */
#ifndef TOLUA_DISABLE_tolua_ToScript_c_do_file00
static int tolua_ToScript_c_do_file00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* filename = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  c_do_file(filename);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_do_file'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: c_get_file_list_by_folder */
#ifndef TOLUA_DISABLE_tolua_ToScript_c_get_file_list_by_folder00
static int tolua_ToScript_c_get_file_list_by_folder00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* folder = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   const char* tolua_ret = (const char*)  c_get_file_list_by_folder(folder);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_file_list_by_folder'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: c_rand_between */
#ifndef TOLUA_DISABLE_tolua_ToScript_c_rand_between00
static int tolua_ToScript_c_rand_between00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a = ((int)  tolua_tonumber(tolua_S,1,0));
  int b = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   int tolua_ret = (int)  c_rand_between(a,b);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_rand_between'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: c_select_by_tenth */
#ifndef TOLUA_DISABLE_tolua_ToScript_c_select_by_tenth00
static int tolua_ToScript_c_select_by_tenth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int tenth = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  c_select_by_tenth(tenth);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_select_by_tenth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_delete00
static int tolua_ToScript_script_role_t_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_add_hero of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_add_hero00
static int tolua_ToScript_script_role_t_c_add_hero00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int hero_index = ((const int)  tolua_tonumber(tolua_S,2,0));
  const int level = ((const int)  tolua_tonumber(tolua_S,3,0));
  const int source_type = ((const int)  tolua_tonumber(tolua_S,4,0));
  const int source_param = ((const int)  tolua_tonumber(tolua_S,5,0));
  bool is_notify = ((bool)  tolua_toboolean(tolua_S,6,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_add_hero'", NULL);
#endif
  {
   self->c_add_hero(hero_index,level,source_type,source_param,is_notify);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_add_hero'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_del_hero of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_del_hero00
static int tolua_ToScript_script_role_t_c_del_hero00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int hero_index = ((const int)  tolua_tonumber(tolua_S,2,0));
  const int source_type = ((const int)  tolua_tonumber(tolua_S,3,0));
  const int source_param = ((const int)  tolua_tonumber(tolua_S,4,0));
  bool is_notify = ((bool)  tolua_toboolean(tolua_S,5,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_del_hero'", NULL);
#endif
  {
   self->c_del_hero(hero_index,source_type,source_param,is_notify);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_del_hero'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_add_item of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_add_item00
static int tolua_ToScript_script_role_t_c_add_item00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int item_tid = ((const int)  tolua_tonumber(tolua_S,2,0));
  const int add_num = ((const int)  tolua_tonumber(tolua_S,3,0));
  const int source_type = ((const int)  tolua_tonumber(tolua_S,4,0));
  const int source_param = ((const int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_add_item'", NULL);
#endif
  {
   self->c_add_item(item_tid,add_num,source_type,source_param);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_add_item'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_add_equip of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_add_equip00
static int tolua_ToScript_script_role_t_c_add_equip00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int equip_tid = ((const int)  tolua_tonumber(tolua_S,2,0));
  const int source_type = ((const int)  tolua_tonumber(tolua_S,3,0));
  const int source_param = ((const int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_add_equip'", NULL);
#endif
  {
   self->c_add_equip(equip_tid,source_type,source_param);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_add_equip'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_add_money of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_add_money00
static int tolua_ToScript_script_role_t_c_add_money00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int type = ((const int)  tolua_tonumber(tolua_S,2,0));
  const int count = ((const int)  tolua_tonumber(tolua_S,3,0));
  const int source_type = ((const int)  tolua_tonumber(tolua_S,4,0));
  const int source_param = ((const int)  tolua_tonumber(tolua_S,5,0));
  bool is_notify = ((bool)  tolua_toboolean(tolua_S,6,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_add_money'", NULL);
#endif
  {
   self->c_add_money(type,count,source_type,source_param,is_notify);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_add_money'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_init_new_role_guide of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_init_new_role_guide00
static int tolua_ToScript_script_role_t_c_init_new_role_guide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int guide_tid = ((const int)  tolua_tonumber(tolua_S,2,0));
  bool is_waiting_to_finish = ((bool)  tolua_toboolean(tolua_S,3,0));
  const int event_type = ((const int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_init_new_role_guide'", NULL);
#endif
  {
   self->c_init_new_role_guide(guide_tid,is_waiting_to_finish,event_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_init_new_role_guide'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_finish_new_role_guide of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_finish_new_role_guide00
static int tolua_ToScript_script_role_t_c_finish_new_role_guide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int guide_tid = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_finish_new_role_guide'", NULL);
#endif
  {
   self->c_finish_new_role_guide(guide_tid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_finish_new_role_guide'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_open_function of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_open_function00
static int tolua_ToScript_script_role_t_c_open_function00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int func_id = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_open_function'", NULL);
#endif
  {
   self->c_open_function(func_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_open_function'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_close_function of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_close_function00
static int tolua_ToScript_script_role_t_c_close_function00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int func_id = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_close_function'", NULL);
#endif
  {
   self->c_close_function(func_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_close_function'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_is_open_function of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_is_open_function00
static int tolua_ToScript_script_role_t_c_is_open_function00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int func_id = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_is_open_function'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_is_open_function(func_id);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_is_open_function'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_has_task of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_has_task00
static int tolua_ToScript_script_role_t_c_has_task00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int task_id = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_has_task'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_has_task(task_id);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_has_task'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_send_mail of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_send_mail00
static int tolua_ToScript_script_role_t_c_send_mail00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int mail_id = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_send_mail'", NULL);
#endif
  {
   self->c_send_mail(mail_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_send_mail'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_do_jump_point of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_do_jump_point00
static int tolua_ToScript_script_role_t_c_do_jump_point00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int jump_id = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_do_jump_point'", NULL);
#endif
  {
   self->c_do_jump_point(jump_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_do_jump_point'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_set_npc_code of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_set_npc_code00
static int tolua_ToScript_script_role_t_c_set_npc_code00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int func = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_set_npc_code'", NULL);
#endif
  {
   self->c_set_npc_code(func);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_set_npc_code'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_set_collect_code of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_set_collect_code00
static int tolua_ToScript_script_role_t_c_set_collect_code00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int func = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_set_collect_code'", NULL);
#endif
  {
   self->c_set_collect_code(func);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_set_collect_code'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_new_fight of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_new_fight00
static int tolua_ToScript_script_role_t_c_new_fight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
  const int against_id = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_new_fight'", NULL);
#endif
  {
   self->c_new_fight(against_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_new_fight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_cancel_fight of class  script_role_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_role_t_c_cancel_fight00
static int tolua_ToScript_script_role_t_c_cancel_fight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_role_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_role_t* self = (script_role_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_cancel_fight'", NULL);
#endif
  {
   self->c_cancel_fight();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_cancel_fight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_delete00
static int tolua_ToScript_script_fight_hero_t_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_script_id of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_script_id00
static int tolua_ToScript_script_fight_hero_t_c_get_script_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_script_id'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->c_get_script_id();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_script_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_tid of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_tid00
static int tolua_ToScript_script_fight_hero_t_c_get_tid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_tid'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->c_get_tid();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_tid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_uid of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_uid00
static int tolua_ToScript_script_fight_hero_t_c_get_uid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_uid'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->c_get_uid();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_uid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_camp of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_camp00
static int tolua_ToScript_script_fight_hero_t_c_get_camp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_camp'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->c_get_camp();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_camp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_sex of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_sex00
static int tolua_ToScript_script_fight_hero_t_c_get_sex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_sex'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->c_get_sex();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_sex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_is_dead of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_is_dead00
static int tolua_ToScript_script_fight_hero_t_c_is_dead00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_is_dead'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_is_dead();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_is_dead'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_attr of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_attr00
static int tolua_ToScript_script_fight_hero_t_c_get_attr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  unsigned int type = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_attr'", NULL);
#endif
  {
   int tolua_ret = (int)  self->c_get_attr(type);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_attr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_set_attr of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_set_attr00
static int tolua_ToScript_script_fight_hero_t_c_set_attr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  unsigned int type = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  int value = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_set_attr'", NULL);
#endif
  {
   self->c_set_attr(type,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_set_attr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_sub_hp of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_sub_hp00
static int tolua_ToScript_script_fight_hero_t_c_sub_hp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  int value = ((int)  tolua_tonumber(tolua_S,2,0));
  script_fight_hero_t* attaker = ((script_fight_hero_t*)  tolua_tousertype(tolua_S,3,0));
  unsigned int skill = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_sub_hp'", NULL);
#endif
  {
   self->c_sub_hp(value,attaker,skill);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_sub_hp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_add_hp of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_add_hp00
static int tolua_ToScript_script_fight_hero_t_c_add_hp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  int value = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_add_hp'", NULL);
#endif
  {
   self->c_add_hp(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_add_hp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_add_buff of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_add_buff00
static int tolua_ToScript_script_fight_hero_t_c_add_buff00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  script_fight_hero_t* target = ((script_fight_hero_t*)  tolua_tousertype(tolua_S,2,0));
  unsigned int id = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned int skill = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
  unsigned int layer = ((unsigned int)  tolua_tonumber(tolua_S,5,1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_add_buff'", NULL);
#endif
  {
   self->c_add_buff(target,id,skill,layer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_add_buff'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_remove_buff of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_remove_buff00
static int tolua_ToScript_script_fight_hero_t_c_remove_buff00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  unsigned int tid = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int attacker_id = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_remove_buff'", NULL);
#endif
  {
   self->c_remove_buff(tid,attacker_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_remove_buff'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_dispel_buff of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_dispel_buff00
static int tolua_ToScript_script_fight_hero_t_c_dispel_buff00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  unsigned int type = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int num = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned int attacker_id = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_dispel_buff'", NULL);
#endif
  {
   self->c_dispel_buff(type,num,attacker_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_dispel_buff'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_has_buff of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_has_buff00
static int tolua_ToScript_script_fight_hero_t_c_has_buff00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  unsigned int tid = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_has_buff'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_has_buff(tid);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_has_buff'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_sub_buff_layer of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_sub_buff_layer00
static int tolua_ToScript_script_fight_hero_t_c_sub_buff_layer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  unsigned int tid = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int attacker_id = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_sub_buff_layer'", NULL);
#endif
  {
   self->c_sub_buff_layer(tid,attacker_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_sub_buff_layer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_buff_layer of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_buff_layer00
static int tolua_ToScript_script_fight_hero_t_c_get_buff_layer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  unsigned int tid = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int attacker_id = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_buff_layer'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->c_get_buff_layer(tid,attacker_id);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_buff_layer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_add_target of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_add_target00
static int tolua_ToScript_script_fight_hero_t_c_add_target00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  script_fight_hero_t* target = ((script_fight_hero_t*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_add_target'", NULL);
#endif
  {
   self->c_add_target(target);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_add_target'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_fight_hero of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_fight_hero00
static int tolua_ToScript_script_fight_hero_t_c_get_fight_hero00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  const char* uid = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_fight_hero'", NULL);
#endif
  {
   script_fight_hero_t* tolua_ret = (script_fight_hero_t*)  self->c_get_fight_hero(uid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"script_fight_hero_t");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_fight_hero'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_all_enemy of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_all_enemy00
static int tolua_ToScript_script_fight_hero_t_c_get_all_enemy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_all_enemy'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->c_get_all_enemy();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_all_enemy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_all_teammate of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_all_teammate00
static int tolua_ToScript_script_fight_hero_t_c_get_all_teammate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_all_teammate'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->c_get_all_teammate();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_all_teammate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_skill_damage of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_skill_damage00
static int tolua_ToScript_script_fight_hero_t_c_skill_damage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  script_fight_hero_t* target = ((script_fight_hero_t*)  tolua_tousertype(tolua_S,2,0));
  int damage = ((int)  tolua_tonumber(tolua_S,3,0));
  unsigned int skill = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_skill_damage'", NULL);
#endif
  {
   self->c_skill_damage(target,damage,skill);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_skill_damage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_skill_ext_damage of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_skill_ext_damage00
static int tolua_ToScript_script_fight_hero_t_c_skill_ext_damage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  script_fight_hero_t* target = ((script_fight_hero_t*)  tolua_tousertype(tolua_S,2,0));
  int damage = ((int)  tolua_tonumber(tolua_S,3,0));
  unsigned int skill = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_skill_ext_damage'", NULL);
#endif
  {
   self->c_skill_ext_damage(target,damage,skill);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_skill_ext_damage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_default_skill of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_default_skill00
static int tolua_ToScript_script_fight_hero_t_c_get_default_skill00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_default_skill'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->c_get_default_skill();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_default_skill'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_counter_skill of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_counter_skill00
static int tolua_ToScript_script_fight_hero_t_c_get_counter_skill00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_counter_skill'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->c_get_counter_skill();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_counter_skill'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_use_skill of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_use_skill00
static int tolua_ToScript_script_fight_hero_t_c_use_skill00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isstring(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  unsigned int skill = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int type = ((unsigned int)  tolua_tonumber(tolua_S,3,1));
  const char* targets = ((const char*)  tolua_tostring(tolua_S,4,NULL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_use_skill'", NULL);
#endif
  {
   self->c_use_skill(skill,type,targets);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_use_skill'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_share_damage_to of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_share_damage_to00
static int tolua_ToScript_script_fight_hero_t_c_share_damage_to00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  script_fight_hero_t* teammate = ((script_fight_hero_t*)  tolua_tousertype(tolua_S,2,0));
  script_fight_hero_t* attacker = ((script_fight_hero_t*)  tolua_tousertype(tolua_S,3,0));
  unsigned int skill = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
  int damage = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_share_damage_to'", NULL);
#endif
  {
   self->c_share_damage_to(teammate,attacker,skill,damage);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_share_damage_to'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_set_var of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_set_var00
static int tolua_ToScript_script_fight_hero_t_c_set_var00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  const char* var = ((const char*)  tolua_tostring(tolua_S,2,0));
  int value = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_set_var'", NULL);
#endif
  {
   self->c_set_var(var,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_set_var'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_var of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_var00
static int tolua_ToScript_script_fight_hero_t_c_get_var00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  const char* var = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_var'", NULL);
#endif
  {
   int tolua_ret = (int)  self->c_get_var(var);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_var'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_skill_by_index of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_skill_by_index00
static int tolua_ToScript_script_fight_hero_t_c_get_skill_by_index00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  const int index = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_skill_by_index'", NULL);
#endif
  {
   int tolua_ret = (int)  self->c_get_skill_by_index(index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_skill_by_index'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_skill_size of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_skill_size00
static int tolua_ToScript_script_fight_hero_t_c_get_skill_size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_skill_size'", NULL);
#endif
  {
   int tolua_ret = (int)  self->c_get_skill_size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_skill_size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_add_combat_act of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_add_combat_act00
static int tolua_ToScript_script_fight_hero_t_c_add_combat_act00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  unsigned int type = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int value = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  script_fight_hero_t* target = ((script_fight_hero_t*)  tolua_tousertype(tolua_S,4,0));
  script_fight_hero_t* attacker = ((script_fight_hero_t*)  tolua_tousertype(tolua_S,5,0));
  unsigned int remain_value = ((unsigned int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_add_combat_act'", NULL);
#endif
  {
   self->c_add_combat_act(type,value,target,attacker,remain_value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_add_combat_act'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_add_combat_skill_act of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_add_combat_skill_act00
static int tolua_ToScript_script_fight_hero_t_c_add_combat_skill_act00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  unsigned int type = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int value = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  const char* attacker = ((const char*)  tolua_tostring(tolua_S,4,0));
  const char* targets = ((const char*)  tolua_tostring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_add_combat_skill_act'", NULL);
#endif
  {
   self->c_add_combat_skill_act(type,value,attacker,targets);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_add_combat_skill_act'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_set_cur_select of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_set_cur_select00
static int tolua_ToScript_script_fight_hero_t_c_set_cur_select00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  const char* target = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_set_cur_select'", NULL);
#endif
  {
   self->c_set_cur_select(target);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_set_cur_select'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_max_atk_target of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_max_atk_target00
static int tolua_ToScript_script_fight_hero_t_c_get_max_atk_target00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  unsigned int skill = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_max_atk_target'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->c_get_max_atk_target(skill);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_max_atk_target'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_weak_target of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_weak_target00
static int tolua_ToScript_script_fight_hero_t_c_get_weak_target00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  unsigned int skill = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_weak_target'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->c_get_weak_target(skill);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_weak_target'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_weak_hp_target of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_weak_hp_target00
static int tolua_ToScript_script_fight_hero_t_c_get_weak_hp_target00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
  unsigned int skill = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_weak_hp_target'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->c_get_weak_hp_target(skill);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_weak_hp_target'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_fight_calc_result of class  script_fight_hero_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_fight_hero_t_c_get_fight_calc_result00
static int tolua_ToScript_script_fight_hero_t_c_get_fight_calc_result00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_fight_hero_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_fight_hero_t* self = (script_fight_hero_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_fight_calc_result'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->c_get_fight_calc_result();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_fight_calc_result'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  script_dungeon_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_dungeon_t_delete00
static int tolua_ToScript_script_dungeon_t_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_dungeon_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_dungeon_t* self = (script_dungeon_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_summon_monster of class  script_dungeon_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_dungeon_t_c_summon_monster00
static int tolua_ToScript_script_dungeon_t_c_summon_monster00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_dungeon_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_dungeon_t* self = (script_dungeon_t*)  tolua_tousertype(tolua_S,1,0);
  const int monster_tid = ((const int)  tolua_tonumber(tolua_S,2,0));
  const int num = ((const int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_summon_monster'", NULL);
#endif
  {
   self->c_summon_monster(monster_tid,num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_summon_monster'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_notify_dungeon_schedule of class  script_dungeon_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_dungeon_t_c_notify_dungeon_schedule00
static int tolua_ToScript_script_dungeon_t_c_notify_dungeon_schedule00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_dungeon_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_dungeon_t* self = (script_dungeon_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_notify_dungeon_schedule'", NULL);
#endif
  {
   self->c_notify_dungeon_schedule();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_notify_dungeon_schedule'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_death_count of class  script_dungeon_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_dungeon_t_c_get_death_count00
static int tolua_ToScript_script_dungeon_t_c_get_death_count00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_dungeon_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_dungeon_t* self = (script_dungeon_t*)  tolua_tousertype(tolua_S,1,0);
  const int monster_tid = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_death_count'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->c_get_death_count(monster_tid);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_death_count'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_is_event_done of class  script_dungeon_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_dungeon_t_c_is_event_done00
static int tolua_ToScript_script_dungeon_t_c_is_event_done00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_dungeon_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_dungeon_t* self = (script_dungeon_t*)  tolua_tousertype(tolua_S,1,0);
  const int id = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_is_event_done'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_is_event_done(id);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_is_event_done'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_add_event_done of class  script_dungeon_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_dungeon_t_c_add_event_done00
static int tolua_ToScript_script_dungeon_t_c_add_event_done00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_dungeon_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_dungeon_t* self = (script_dungeon_t*)  tolua_tousertype(tolua_S,1,0);
  const int id = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_add_event_done'", NULL);
#endif
  {
   self->c_add_event_done(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_add_event_done'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_add_schedule_count of class  script_dungeon_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_dungeon_t_c_add_schedule_count00
static int tolua_ToScript_script_dungeon_t_c_add_schedule_count00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_dungeon_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_dungeon_t* self = (script_dungeon_t*)  tolua_tousertype(tolua_S,1,0);
  const int id = ((const int)  tolua_tonumber(tolua_S,2,0));
  bool is_del = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_add_schedule_count'", NULL);
#endif
  {
   self->c_add_schedule_count(id,is_del);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_add_schedule_count'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_ToScript_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"script",0);
  tolua_beginmodule(tolua_S,"script");
   tolua_constant(tolua_S,"combat_act_type_skill",script::combat_act_type_skill);
   tolua_constant(tolua_S,"combat_act_type_buff",script::combat_act_type_buff);
   tolua_constant(tolua_S,"combat_act_type_buff_remove",script::combat_act_type_buff_remove);
   tolua_constant(tolua_S,"combat_act_type_tactic_on",script::combat_act_type_tactic_on);
   tolua_constant(tolua_S,"combat_act_type_tactic_off",script::combat_act_type_tactic_off);
   tolua_constant(tolua_S,"combat_act_type_dodge",script::combat_act_type_dodge);
   tolua_constant(tolua_S,"combat_act_type_heal",script::combat_act_type_heal);
   tolua_constant(tolua_S,"combat_act_type_heal_crit",script::combat_act_type_heal_crit);
   tolua_constant(tolua_S,"combat_act_type_damage",script::combat_act_type_damage);
   tolua_constant(tolua_S,"combat_act_type_ext",script::combat_act_type_ext);
   tolua_constant(tolua_S,"combat_act_type_crit",script::combat_act_type_crit);
   tolua_constant(tolua_S,"combat_act_type_absorb",script::combat_act_type_absorb);
   tolua_constant(tolua_S,"combat_act_type_reflect",script::combat_act_type_reflect);
   tolua_constant(tolua_S,"combat_act_type_leech",script::combat_act_type_leech);
   tolua_constant(tolua_S,"combat_act_type_share_damage",script::combat_act_type_share_damage);
   tolua_constant(tolua_S,"combat_act_type_relive",script::combat_act_type_relive);
   tolua_constant(tolua_S,"combat_act_type_block",script::combat_act_type_block);
   tolua_constant(tolua_S,"combat_act_type_counter",script::combat_act_type_counter);
   tolua_constant(tolua_S,"combat_act_type_combo",script::combat_act_type_combo);
   tolua_constant(tolua_S,"combat_act_type_follow",script::combat_act_type_follow);
   tolua_constant(tolua_S,"combat_act_type_pincer_attack",script::combat_act_type_pincer_attack);
   tolua_constant(tolua_S,"combat_act_type_att_speed_inc",script::combat_act_type_att_speed_inc);
   tolua_constant(tolua_S,"combat_act_type_att_speed_dec",script::combat_act_type_att_speed_dec);
   tolua_constant(tolua_S,"combat_act_type_kill_leech",script::combat_act_type_kill_leech);
   tolua_constant(tolua_S,"combat_act_type_recover",script::combat_act_type_recover);
   tolua_constant(tolua_S,"combat_act_type_syn_cd",script::combat_act_type_syn_cd);
   tolua_constant(tolua_S,"combat_act_type_syn_hp",script::combat_act_type_syn_hp);
   tolua_constant(tolua_S,"combat_act_type_syn_wave",script::combat_act_type_syn_wave);
   tolua_constant(tolua_S,"combat_act_type_beheaded",script::combat_act_type_beheaded);
   tolua_constant(tolua_S,"combat_act_type_syn_atk_count",script::combat_act_type_syn_atk_count);
   tolua_constant(tolua_S,"combat_act_type_buff_damage",script::combat_act_type_buff_damage);
   tolua_constant(tolua_S,"combat_act_type_buff_heal",script::combat_act_type_buff_heal);
   tolua_constant(tolua_S,"combat_act_type_buff_dot",script::combat_act_type_buff_dot);
   tolua_constant(tolua_S,"combat_act_type_accept_damage",script::combat_act_type_accept_damage);
   tolua_constant(tolua_S,"combat_act_type_be_share_damage",script::combat_act_type_be_share_damage);
   tolua_constant(tolua_S,"combat_act_type_special_skill",script::combat_act_type_special_skill);
   tolua_constant(tolua_S,"combat_act_type_immuno",script::combat_act_type_immuno);
   tolua_constant(tolua_S,"combat_act_type_trigger_skill",script::combat_act_type_trigger_skill);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"c_debug",tolua_ToScript_c_debug00);
  tolua_function(tolua_S,"c_info",tolua_ToScript_c_info00);
  tolua_function(tolua_S,"c_error",tolua_ToScript_c_error00);
  tolua_function(tolua_S,"c_assert_fail",tolua_ToScript_c_assert_fail00);
  tolua_function(tolua_S,"c_get_root_dir",tolua_ToScript_c_get_root_dir00);
  tolua_function(tolua_S,"c_do_file",tolua_ToScript_c_do_file00);
  tolua_function(tolua_S,"c_get_file_list_by_folder",tolua_ToScript_c_get_file_list_by_folder00);
  tolua_function(tolua_S,"c_rand_between",tolua_ToScript_c_rand_between00);
  tolua_function(tolua_S,"c_select_by_tenth",tolua_ToScript_c_select_by_tenth00);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"script_role_t","script_role_t","ScriptProxy<role_t>",tolua_collect_script_role_t);
  #else
  tolua_cclass(tolua_S,"script_role_t","script_role_t","ScriptProxy<role_t>",NULL);
  #endif
  tolua_beginmodule(tolua_S,"script_role_t");
   tolua_function(tolua_S,"delete",tolua_ToScript_script_role_t_delete00);
   tolua_function(tolua_S,"c_add_hero",tolua_ToScript_script_role_t_c_add_hero00);
   tolua_function(tolua_S,"c_del_hero",tolua_ToScript_script_role_t_c_del_hero00);
   tolua_function(tolua_S,"c_add_item",tolua_ToScript_script_role_t_c_add_item00);
   tolua_function(tolua_S,"c_add_equip",tolua_ToScript_script_role_t_c_add_equip00);
   tolua_function(tolua_S,"c_add_money",tolua_ToScript_script_role_t_c_add_money00);
   tolua_function(tolua_S,"c_init_new_role_guide",tolua_ToScript_script_role_t_c_init_new_role_guide00);
   tolua_function(tolua_S,"c_finish_new_role_guide",tolua_ToScript_script_role_t_c_finish_new_role_guide00);
   tolua_function(tolua_S,"c_open_function",tolua_ToScript_script_role_t_c_open_function00);
   tolua_function(tolua_S,"c_close_function",tolua_ToScript_script_role_t_c_close_function00);
   tolua_function(tolua_S,"c_is_open_function",tolua_ToScript_script_role_t_c_is_open_function00);
   tolua_function(tolua_S,"c_has_task",tolua_ToScript_script_role_t_c_has_task00);
   tolua_function(tolua_S,"c_send_mail",tolua_ToScript_script_role_t_c_send_mail00);
   tolua_function(tolua_S,"c_do_jump_point",tolua_ToScript_script_role_t_c_do_jump_point00);
   tolua_function(tolua_S,"c_set_npc_code",tolua_ToScript_script_role_t_c_set_npc_code00);
   tolua_function(tolua_S,"c_set_collect_code",tolua_ToScript_script_role_t_c_set_collect_code00);
   tolua_function(tolua_S,"c_new_fight",tolua_ToScript_script_role_t_c_new_fight00);
   tolua_function(tolua_S,"c_cancel_fight",tolua_ToScript_script_role_t_c_cancel_fight00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"script_fight_hero_t","script_fight_hero_t","ScriptProxy<fight_hero_t>",tolua_collect_script_fight_hero_t);
  #else
  tolua_cclass(tolua_S,"script_fight_hero_t","script_fight_hero_t","ScriptProxy<fight_hero_t>",NULL);
  #endif
  tolua_beginmodule(tolua_S,"script_fight_hero_t");
   tolua_function(tolua_S,"delete",tolua_ToScript_script_fight_hero_t_delete00);
   tolua_function(tolua_S,"c_get_script_id",tolua_ToScript_script_fight_hero_t_c_get_script_id00);
   tolua_function(tolua_S,"c_get_tid",tolua_ToScript_script_fight_hero_t_c_get_tid00);
   tolua_function(tolua_S,"c_get_uid",tolua_ToScript_script_fight_hero_t_c_get_uid00);
   tolua_function(tolua_S,"c_get_camp",tolua_ToScript_script_fight_hero_t_c_get_camp00);
   tolua_function(tolua_S,"c_get_sex",tolua_ToScript_script_fight_hero_t_c_get_sex00);
   tolua_function(tolua_S,"c_is_dead",tolua_ToScript_script_fight_hero_t_c_is_dead00);
   tolua_function(tolua_S,"c_get_attr",tolua_ToScript_script_fight_hero_t_c_get_attr00);
   tolua_function(tolua_S,"c_set_attr",tolua_ToScript_script_fight_hero_t_c_set_attr00);
   tolua_function(tolua_S,"c_sub_hp",tolua_ToScript_script_fight_hero_t_c_sub_hp00);
   tolua_function(tolua_S,"c_add_hp",tolua_ToScript_script_fight_hero_t_c_add_hp00);
   tolua_function(tolua_S,"c_add_buff",tolua_ToScript_script_fight_hero_t_c_add_buff00);
   tolua_function(tolua_S,"c_remove_buff",tolua_ToScript_script_fight_hero_t_c_remove_buff00);
   tolua_function(tolua_S,"c_dispel_buff",tolua_ToScript_script_fight_hero_t_c_dispel_buff00);
   tolua_function(tolua_S,"c_has_buff",tolua_ToScript_script_fight_hero_t_c_has_buff00);
   tolua_function(tolua_S,"c_sub_buff_layer",tolua_ToScript_script_fight_hero_t_c_sub_buff_layer00);
   tolua_function(tolua_S,"c_get_buff_layer",tolua_ToScript_script_fight_hero_t_c_get_buff_layer00);
   tolua_function(tolua_S,"c_add_target",tolua_ToScript_script_fight_hero_t_c_add_target00);
   tolua_function(tolua_S,"c_get_fight_hero",tolua_ToScript_script_fight_hero_t_c_get_fight_hero00);
   tolua_function(tolua_S,"c_get_all_enemy",tolua_ToScript_script_fight_hero_t_c_get_all_enemy00);
   tolua_function(tolua_S,"c_get_all_teammate",tolua_ToScript_script_fight_hero_t_c_get_all_teammate00);
   tolua_function(tolua_S,"c_skill_damage",tolua_ToScript_script_fight_hero_t_c_skill_damage00);
   tolua_function(tolua_S,"c_skill_ext_damage",tolua_ToScript_script_fight_hero_t_c_skill_ext_damage00);
   tolua_function(tolua_S,"c_get_default_skill",tolua_ToScript_script_fight_hero_t_c_get_default_skill00);
   tolua_function(tolua_S,"c_get_counter_skill",tolua_ToScript_script_fight_hero_t_c_get_counter_skill00);
   tolua_function(tolua_S,"c_use_skill",tolua_ToScript_script_fight_hero_t_c_use_skill00);
   tolua_function(tolua_S,"c_share_damage_to",tolua_ToScript_script_fight_hero_t_c_share_damage_to00);
   tolua_function(tolua_S,"c_set_var",tolua_ToScript_script_fight_hero_t_c_set_var00);
   tolua_function(tolua_S,"c_get_var",tolua_ToScript_script_fight_hero_t_c_get_var00);
   tolua_function(tolua_S,"c_get_skill_by_index",tolua_ToScript_script_fight_hero_t_c_get_skill_by_index00);
   tolua_function(tolua_S,"c_get_skill_size",tolua_ToScript_script_fight_hero_t_c_get_skill_size00);
   tolua_function(tolua_S,"c_add_combat_act",tolua_ToScript_script_fight_hero_t_c_add_combat_act00);
   tolua_function(tolua_S,"c_add_combat_skill_act",tolua_ToScript_script_fight_hero_t_c_add_combat_skill_act00);
   tolua_function(tolua_S,"c_set_cur_select",tolua_ToScript_script_fight_hero_t_c_set_cur_select00);
   tolua_function(tolua_S,"c_get_max_atk_target",tolua_ToScript_script_fight_hero_t_c_get_max_atk_target00);
   tolua_function(tolua_S,"c_get_weak_target",tolua_ToScript_script_fight_hero_t_c_get_weak_target00);
   tolua_function(tolua_S,"c_get_weak_hp_target",tolua_ToScript_script_fight_hero_t_c_get_weak_hp_target00);
   tolua_function(tolua_S,"c_get_fight_calc_result",tolua_ToScript_script_fight_hero_t_c_get_fight_calc_result00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"script_dungeon_t","script_dungeon_t","ScriptProxy<dungeon_scene_t>",tolua_collect_script_dungeon_t);
  #else
  tolua_cclass(tolua_S,"script_dungeon_t","script_dungeon_t","ScriptProxy<dungeon_scene_t>",NULL);
  #endif
  tolua_beginmodule(tolua_S,"script_dungeon_t");
   tolua_function(tolua_S,"delete",tolua_ToScript_script_dungeon_t_delete00);
   tolua_function(tolua_S,"c_summon_monster",tolua_ToScript_script_dungeon_t_c_summon_monster00);
   tolua_function(tolua_S,"c_notify_dungeon_schedule",tolua_ToScript_script_dungeon_t_c_notify_dungeon_schedule00);
   tolua_function(tolua_S,"c_get_death_count",tolua_ToScript_script_dungeon_t_c_get_death_count00);
   tolua_function(tolua_S,"c_is_event_done",tolua_ToScript_script_dungeon_t_c_is_event_done00);
   tolua_function(tolua_S,"c_add_event_done",tolua_ToScript_script_dungeon_t_c_add_event_done00);
   tolua_function(tolua_S,"c_add_schedule_count",tolua_ToScript_script_dungeon_t_c_add_schedule_count00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"attrtype_enum",0);
  tolua_beginmodule(tolua_S,"attrtype_enum");
   tolua_constant(tolua_S,"strength",attrtype_enum::strength);
   tolua_constant(tolua_S,"intelligent",attrtype_enum::intelligent);
   tolua_constant(tolua_S,"vitality",attrtype_enum::vitality);
   tolua_constant(tolua_S,"spirit",attrtype_enum::spirit);
   tolua_constant(tolua_S,"additive_damage",attrtype_enum::additive_damage);
   tolua_constant(tolua_S,"fight_constant",attrtype_enum::fight_constant);
   tolua_constant(tolua_S,"atk",attrtype_enum::atk);
   tolua_constant(tolua_S,"atk_range",attrtype_enum::atk_range);
   tolua_constant(tolua_S,"atk_max",attrtype_enum::atk_max);
   tolua_constant(tolua_S,"def_physical",attrtype_enum::def_physical);
   tolua_constant(tolua_S,"attack_no_wakeup_rate",attrtype_enum::attack_no_wakeup_rate);
   tolua_constant(tolua_S,"hp",attrtype_enum::hp);
   tolua_constant(tolua_S,"max_hp",attrtype_enum::max_hp);
   tolua_constant(tolua_S,"max_hp_in_combat",attrtype_enum::max_hp_in_combat);
   tolua_constant(tolua_S,"max_hp_bonus",attrtype_enum::max_hp_bonus);
   tolua_constant(tolua_S,"atk_growth",attrtype_enum::atk_growth);
   tolua_constant(tolua_S,"def_physical_growth",attrtype_enum::def_physical_growth);
   tolua_constant(tolua_S,"share_damage",attrtype_enum::share_damage);
   tolua_constant(tolua_S,"max_hp_growth",attrtype_enum::max_hp_growth);
   tolua_constant(tolua_S,"sp",attrtype_enum::sp);
   tolua_constant(tolua_S,"max_sp",attrtype_enum::max_sp);
   tolua_constant(tolua_S,"ranged_atk",attrtype_enum::ranged_atk);
   tolua_constant(tolua_S,"action_bar",attrtype_enum::action_bar);
   tolua_constant(tolua_S,"hp_present_ratio",attrtype_enum::hp_present_ratio);
   tolua_constant(tolua_S,"hp_loss_ratio",attrtype_enum::hp_loss_ratio);
   tolua_constant(tolua_S,"crit_rate",attrtype_enum::crit_rate);
   tolua_constant(tolua_S,"crit_damage",attrtype_enum::crit_damage);
   tolua_constant(tolua_S,"crit_damage_bonus",attrtype_enum::crit_damage_bonus);
   tolua_constant(tolua_S,"crit_damage_reduce",attrtype_enum::crit_damage_reduce);
   tolua_constant(tolua_S,"block_rate",attrtype_enum::block_rate);
   tolua_constant(tolua_S,"block_effect",attrtype_enum::block_effect);
   tolua_constant(tolua_S,"block_effect_bonus",attrtype_enum::block_effect_bonus);
   tolua_constant(tolua_S,"block_effect_reduce",attrtype_enum::block_effect_reduce);
   tolua_constant(tolua_S,"accurate",attrtype_enum::accurate);
   tolua_constant(tolua_S,"speed",attrtype_enum::speed);
   tolua_constant(tolua_S,"effect_hit_rate",attrtype_enum::effect_hit_rate);
   tolua_constant(tolua_S,"effect_dodge_rate",attrtype_enum::effect_dodge_rate);
   tolua_constant(tolua_S,"def_physical_reduce",attrtype_enum::def_physical_reduce);
   tolua_constant(tolua_S,"damage_bonus",attrtype_enum::damage_bonus);
   tolua_constant(tolua_S,"damage_reduce",attrtype_enum::damage_reduce);
   tolua_constant(tolua_S,"get_damage_bonus",attrtype_enum::get_damage_bonus);
   tolua_constant(tolua_S,"get_damage_reduce",attrtype_enum::get_damage_reduce);
   tolua_constant(tolua_S,"heal_bonus",attrtype_enum::heal_bonus);
   tolua_constant(tolua_S,"heal_reduce",attrtype_enum::heal_reduce);
   tolua_constant(tolua_S,"get_heal_bonus",attrtype_enum::get_heal_bonus);
   tolua_constant(tolua_S,"get_heal_reduce",attrtype_enum::get_heal_reduce);
   tolua_constant(tolua_S,"leech",attrtype_enum::leech);
   tolua_constant(tolua_S,"reflect",attrtype_enum::reflect);
   tolua_constant(tolua_S,"shield",attrtype_enum::shield);
   tolua_constant(tolua_S,"shield_bonus",attrtype_enum::shield_bonus);
   tolua_constant(tolua_S,"shield_points",attrtype_enum::shield_points);
   tolua_constant(tolua_S,"luck_points",attrtype_enum::luck_points);
   tolua_constant(tolua_S,"luck_shield_points",attrtype_enum::luck_shield_points);
   tolua_constant(tolua_S,"xiangyu_shield_points",attrtype_enum::xiangyu_shield_points);
   tolua_constant(tolua_S,"dodge",attrtype_enum::dodge);
   tolua_constant(tolua_S,"combo_rate",attrtype_enum::combo_rate);
   tolua_constant(tolua_S,"combo_max",attrtype_enum::combo_max);
   tolua_constant(tolua_S,"counter_rate",attrtype_enum::counter_rate);
   tolua_constant(tolua_S,"counter_max",attrtype_enum::counter_max);
   tolua_constant(tolua_S,"speed_second",attrtype_enum::speed_second);
   tolua_constant(tolua_S,"skill_damage",attrtype_enum::skill_damage);
   tolua_constant(tolua_S,"skill_damage_extra",attrtype_enum::skill_damage_extra);
   tolua_constant(tolua_S,"skill_heal",attrtype_enum::skill_heal);
   tolua_constant(tolua_S,"skill_heal_extra",attrtype_enum::skill_heal_extra);
   tolua_constant(tolua_S,"skill_shield",attrtype_enum::skill_shield);
   tolua_constant(tolua_S,"skill_shield_basic",attrtype_enum::skill_shield_basic);
   tolua_constant(tolua_S,"skill_leech_basic",attrtype_enum::skill_leech_basic);
   tolua_constant(tolua_S,"skill_leech",attrtype_enum::skill_leech);
   tolua_constant(tolua_S,"skill_reflect_basic",attrtype_enum::skill_reflect_basic);
   tolua_constant(tolua_S,"spurting",attrtype_enum::spurting);
   tolua_constant(tolua_S,"true_damage",attrtype_enum::true_damage);
   tolua_constant(tolua_S,"true_def",attrtype_enum::true_def);
   tolua_constant(tolua_S,"pvp_arena_damage",attrtype_enum::pvp_arena_damage);
   tolua_constant(tolua_S,"pvp_arena_def",attrtype_enum::pvp_arena_def);
   tolua_constant(tolua_S,"pvp_no_arena_damage",attrtype_enum::pvp_no_arena_damage);
   tolua_constant(tolua_S,"pvp_no_arena_def",attrtype_enum::pvp_no_arena_def);
   tolua_constant(tolua_S,"true_cure",attrtype_enum::true_cure);
   tolua_constant(tolua_S,"phase_rate",attrtype_enum::phase_rate);
   tolua_constant(tolua_S,"phase_max",attrtype_enum::phase_max);
   tolua_constant(tolua_S,"stage_atk_bonus",attrtype_enum::stage_atk_bonus);
   tolua_constant(tolua_S,"stage_def_physical_bonus",attrtype_enum::stage_def_physical_bonus);
   tolua_constant(tolua_S,"stage_def_spell_bonus",attrtype_enum::stage_def_spell_bonus);
   tolua_constant(tolua_S,"stage_max_hp_bonus",attrtype_enum::stage_max_hp_bonus);
   tolua_constant(tolua_S,"add_speed",attrtype_enum::add_speed);
   tolua_constant(tolua_S,"all_team_damage_turn",attrtype_enum::all_team_damage_turn);
   tolua_constant(tolua_S,"atk_add_per",attrtype_enum::atk_add_per);
   tolua_constant(tolua_S,"atk_cut_per",attrtype_enum::atk_cut_per);
   tolua_constant(tolua_S,"def_add_per",attrtype_enum::def_add_per);
   tolua_constant(tolua_S,"def_cut_per",attrtype_enum::def_cut_per);
   tolua_constant(tolua_S,"crit_damage_cut_per",attrtype_enum::crit_damage_cut_per);
   tolua_constant(tolua_S,"hp_cut_per",attrtype_enum::hp_cut_per);
   tolua_constant(tolua_S,"speed_add_per",attrtype_enum::speed_add_per);
   tolua_constant(tolua_S,"speed_cut_per",attrtype_enum::speed_cut_per);
   tolua_constant(tolua_S,"def_ignore_rate",attrtype_enum::def_ignore_rate);
   tolua_constant(tolua_S,"dp_convert_hurt",attrtype_enum::dp_convert_hurt);
   tolua_constant(tolua_S,"dp_convert_reflect_rate",attrtype_enum::dp_convert_reflect_rate);
   tolua_constant(tolua_S,"target_debuff_attack_addition",attrtype_enum::target_debuff_attack_addition);
   tolua_constant(tolua_S,"target_status_attack_id",attrtype_enum::target_status_attack_id);
   tolua_constant(tolua_S,"target_status_attack_addition",attrtype_enum::target_status_attack_addition);
   tolua_constant(tolua_S,"debuff_damage_strengthen",attrtype_enum::debuff_damage_strengthen);
   tolua_constant(tolua_S,"thunder_cloud_buff_id",attrtype_enum::thunder_cloud_buff_id);
   tolua_constant(tolua_S,"thunder_clond_hurt_ratio",attrtype_enum::thunder_clond_hurt_ratio);
   tolua_constant(tolua_S,"damnation_buff_id",attrtype_enum::damnation_buff_id);
   tolua_constant(tolua_S,"damnation_hurt_ratio",attrtype_enum::damnation_hurt_ratio);
   tolua_constant(tolua_S,"lose_blood_volume_to_attack",attrtype_enum::lose_blood_volume_to_attack);
   tolua_constant(tolua_S,"blood_volume_to_attack_limit_weapon",attrtype_enum::blood_volume_to_attack_limit_weapon);
   tolua_constant(tolua_S,"blood_volume_to_attack_ratio_weapon",attrtype_enum::blood_volume_to_attack_ratio_weapon);
   tolua_constant(tolua_S,"blood_volume_to_attack_limit_skill",attrtype_enum::blood_volume_to_attack_limit_skill);
   tolua_constant(tolua_S,"blood_volume_to_attack_ratio_skill",attrtype_enum::blood_volume_to_attack_ratio_skill);
   tolua_constant(tolua_S,"self_attack_addition",attrtype_enum::self_attack_addition);
   tolua_constant(tolua_S,"counter_attack_addition",attrtype_enum::counter_attack_addition);
   tolua_constant(tolua_S,"def_ignore_value",attrtype_enum::def_ignore_value);
   tolua_constant(tolua_S,"speed_effect_ratio",attrtype_enum::speed_effect_ratio);
   tolua_constant(tolua_S,"stratagem_buff_id",attrtype_enum::stratagem_buff_id);
   tolua_constant(tolua_S,"stratagem_buff_ratio",attrtype_enum::stratagem_buff_ratio);
   tolua_constant(tolua_S,"lose_blood_volume_to_defend",attrtype_enum::lose_blood_volume_to_defend);
   tolua_constant(tolua_S,"once_attack_lose_max_hp_ratio",attrtype_enum::once_attack_lose_max_hp_ratio);
   tolua_constant(tolua_S,"reflect_by_attack",attrtype_enum::reflect_by_attack);
   tolua_constant(tolua_S,"reflect_by_damage",attrtype_enum::reflect_by_damage);
   tolua_constant(tolua_S,"reflect_limit",attrtype_enum::reflect_limit);
   tolua_constant(tolua_S,"once_trigger_skill_limit",attrtype_enum::once_trigger_skill_limit);
   tolua_constant(tolua_S,"one_round_all_damage_to_injury",attrtype_enum::one_round_all_damage_to_injury);
   tolua_constant(tolua_S,"dodge_single_target_damage",attrtype_enum::dodge_single_target_damage);
   tolua_constant(tolua_S,"combo_buffer_id",attrtype_enum::combo_buffer_id);
   tolua_constant(tolua_S,"single_attack_behurt_ratio",attrtype_enum::single_attack_behurt_ratio);
   tolua_constant(tolua_S,"range_attack_behurt_ratio",attrtype_enum::range_attack_behurt_ratio);
   tolua_constant(tolua_S,"skill_cd_change",attrtype_enum::skill_cd_change);
   tolua_constant(tolua_S,"anti_dodge",attrtype_enum::anti_dodge);
   tolua_constant(tolua_S,"in_control_attack_addition",attrtype_enum::in_control_attack_addition);
   tolua_constant(tolua_S,"other_action_addition_hurt",attrtype_enum::other_action_addition_hurt);
   tolua_constant(tolua_S,"lose_blood_volume_to_crit_effect",attrtype_enum::lose_blood_volume_to_crit_effect);
   tolua_constant(tolua_S,"all_team_damage_to_injury",attrtype_enum::all_team_damage_to_injury);
   tolua_constant(tolua_S,"extra_atk",attrtype_enum::extra_atk);
   tolua_constant(tolua_S,"extra_def",attrtype_enum::extra_def);
   tolua_constant(tolua_S,"extra_hp",attrtype_enum::extra_hp);
   tolua_constant(tolua_S,"be_combo_buffer_id",attrtype_enum::be_combo_buffer_id);
   tolua_constant(tolua_S,"temp_terry",attrtype_enum::temp_terry);
   tolua_constant(tolua_S,"extra_skill_ratio",attrtype_enum::extra_skill_ratio);
   tolua_constant(tolua_S,"crit_add_phase_rate",attrtype_enum::crit_add_phase_rate);
   tolua_constant(tolua_S,"shared_hurt_per",attrtype_enum::shared_hurt_per);
   tolua_constant(tolua_S,"reduce_shared_hurt_per",attrtype_enum::reduce_shared_hurt_per);
   tolua_constant(tolua_S,"shared_buff_id",attrtype_enum::shared_buff_id);
   tolua_constant(tolua_S,"buff_round",attrtype_enum::buff_round);
   tolua_constant(tolua_S,"debuff_round",attrtype_enum::debuff_round);
   tolua_constant(tolua_S,"phase_buffer_id",attrtype_enum::phase_buffer_id);
   tolua_constant(tolua_S,"taunt_damage_reduce",attrtype_enum::taunt_damage_reduce);
   tolua_constant(tolua_S,"dodge_group_target_damage",attrtype_enum::dodge_group_target_damage);
   tolua_constant(tolua_S,"speed_effect_reduce_ratio",attrtype_enum::speed_effect_reduce_ratio);
   tolua_constant(tolua_S,"execute_boss_extra_damage",attrtype_enum::execute_boss_extra_damage);
   tolua_constant(tolua_S,"stun",attrtype_enum::stun);
   tolua_constant(tolua_S,"taunt",attrtype_enum::taunt);
   tolua_constant(tolua_S,"bleeding",attrtype_enum::bleeding);
   tolua_constant(tolua_S,"confused",attrtype_enum::confused);
   tolua_constant(tolua_S,"drunk",attrtype_enum::drunk);
   tolua_constant(tolua_S,"exempt_cd",attrtype_enum::exempt_cd);
   tolua_constant(tolua_S,"immuno",attrtype_enum::immuno);
   tolua_constant(tolua_S,"hostile_type_stamp",attrtype_enum::hostile_type_stamp);
   tolua_constant(tolua_S,"beheaded_rate",attrtype_enum::beheaded_rate);
   tolua_constant(tolua_S,"beheaded_damage_times",attrtype_enum::beheaded_damage_times);
   tolua_constant(tolua_S,"soul_condition",attrtype_enum::soul_condition);
   tolua_constant(tolua_S,"no_revive_condition",attrtype_enum::no_revive_condition);
   tolua_constant(tolua_S,"soul_guard",attrtype_enum::soul_guard);
   tolua_constant(tolua_S,"athanasia",attrtype_enum::athanasia);
   tolua_constant(tolua_S,"invincibility",attrtype_enum::invincibility);
   tolua_constant(tolua_S,"guide_team",attrtype_enum::guide_team);
   tolua_constant(tolua_S,"frozen",attrtype_enum::frozen);
   tolua_constant(tolua_S,"sleep",attrtype_enum::sleep);
   tolua_constant(tolua_S,"forget",attrtype_enum::forget);
   tolua_constant(tolua_S,"forbid_strengthen",attrtype_enum::forbid_strengthen);
   tolua_constant(tolua_S,"silent",attrtype_enum::silent);
   tolua_constant(tolua_S,"def_break",attrtype_enum::def_break);
   tolua_constant(tolua_S,"immuno_single_attack",attrtype_enum::immuno_single_attack);
   tolua_constant(tolua_S,"damage_absorb_and_boom",attrtype_enum::damage_absorb_and_boom);
   tolua_constant(tolua_S,"fairyland",attrtype_enum::fairyland);
   tolua_constant(tolua_S,"water_immune",attrtype_enum::water_immune);
   tolua_constant(tolua_S,"fire_immune",attrtype_enum::fire_immune);
   tolua_constant(tolua_S,"wind_immune",attrtype_enum::wind_immune);
   tolua_constant(tolua_S,"yin_immune",attrtype_enum::yin_immune);
   tolua_constant(tolua_S,"yang_immune",attrtype_enum::yang_immune);
   tolua_constant(tolua_S,"water_hurt_reduce",attrtype_enum::water_hurt_reduce);
   tolua_constant(tolua_S,"fire_hurt_reduce",attrtype_enum::fire_hurt_reduce);
   tolua_constant(tolua_S,"wind_hurt_reduce",attrtype_enum::wind_hurt_reduce);
   tolua_constant(tolua_S,"yin_hurt_reduce",attrtype_enum::yin_hurt_reduce);
   tolua_constant(tolua_S,"yang_hurt_reduce",attrtype_enum::yang_hurt_reduce);
   tolua_constant(tolua_S,"water_hurt_increase",attrtype_enum::water_hurt_increase);
   tolua_constant(tolua_S,"fire_hurt_increase",attrtype_enum::fire_hurt_increase);
   tolua_constant(tolua_S,"wind_hurt_increase",attrtype_enum::wind_hurt_increase);
   tolua_constant(tolua_S,"yin_hurt_increase",attrtype_enum::yin_hurt_increase);
   tolua_constant(tolua_S,"yang_hurt_increase",attrtype_enum::yang_hurt_increase);
   tolua_constant(tolua_S,"buff_attacker_be_hurt_count",attrtype_enum::buff_attacker_be_hurt_count);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_ToScript (lua_State* tolua_S) {
 return tolua_ToScript_open(tolua_S);
};
#endif

