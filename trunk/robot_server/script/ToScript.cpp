/*
** Lua binding: ToScript
** Generated automatically by tolua++-1.0.92 on Tue Jul 24 22:57:07 2018.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_ToScript_open (lua_State* tolua_S);

#include "script/game_script_func.h"
#include "script/script_robot.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_script_robot_t (lua_State* tolua_S)
{
 script_robot_t* self = (script_robot_t*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"script_robot_t");
 tolua_usertype(tolua_S,"ScriptProxy<robot_t>");
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

/* method: delete of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_delete00
static int tolua_ToScript_script_robot_t_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
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

/* method: c_test_robot of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_test_robot00
static int tolua_ToScript_script_robot_t_c_test_robot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  const int index = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_test_robot'", NULL);
#endif
  {
   self->c_test_robot(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_test_robot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_move_to of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_move_to00
static int tolua_ToScript_script_robot_t_c_move_to00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float z = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_move_to'", NULL);
#endif
  {
   self->c_move_to(x,z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_move_to'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_fast_move of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_fast_move00
static int tolua_ToScript_script_robot_t_c_fast_move00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float z = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_fast_move'", NULL);
#endif
  {
   self->c_fast_move(x,z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_fast_move'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_move_arrive of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_move_arrive00
static int tolua_ToScript_script_robot_t_c_move_arrive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float z = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_move_arrive'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_move_arrive(x,z);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_move_arrive'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_move_around of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_move_around00
static int tolua_ToScript_script_robot_t_c_move_around00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float z = ((float)  tolua_tonumber(tolua_S,3,0));
  float dist = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_move_around'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_move_around(x,z,dist);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_move_around'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_scene of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_get_scene00
static int tolua_ToScript_script_robot_t_c_get_scene00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_scene'", NULL);
#endif
  {
   int tolua_ret = (int)  self->c_get_scene();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_scene'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_robot_done of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_robot_done00
static int tolua_ToScript_script_robot_t_c_robot_done00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_robot_done'", NULL);
#endif
  {
   self->c_robot_done();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_robot_done'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_currX of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_get_currX00
static int tolua_ToScript_script_robot_t_c_get_currX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_currX'", NULL);
#endif
  {
   float tolua_ret = (float)  self->c_get_currX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_currX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_currY of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_get_currY00
static int tolua_ToScript_script_robot_t_c_get_currY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_currY'", NULL);
#endif
  {
   float tolua_ret = (float)  self->c_get_currY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_currY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_talk_about of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_talk_about00
static int tolua_ToScript_script_robot_t_c_talk_about00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* str = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_talk_about'", NULL);
#endif
  {
   self->c_talk_about(index,str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_talk_about'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_jump_scene of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_jump_scene00
static int tolua_ToScript_script_robot_t_c_jump_scene00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_jump_scene'", NULL);
#endif
  {
   self->c_jump_scene(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_jump_scene'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_jump_id of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_jump_id00
static int tolua_ToScript_script_robot_t_c_jump_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_jump_id'", NULL);
#endif
  {
   self->c_jump_id(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_jump_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_gm_command of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_gm_command00
static int tolua_ToScript_script_robot_t_c_gm_command00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_gm_command'", NULL);
#endif
  {
   self->c_gm_command(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_gm_command'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_toggle_mount of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_toggle_mount00
static int tolua_ToScript_script_robot_t_c_toggle_mount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  int state = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_toggle_mount'", NULL);
#endif
  {
   self->c_toggle_mount(state);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_toggle_mount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_mail_list of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_get_mail_list00
static int tolua_ToScript_script_robot_t_c_get_mail_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_mail_list'", NULL);
#endif
  {
   self->c_get_mail_list();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_mail_list'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_have_new_mail of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_have_new_mail00
static int tolua_ToScript_script_robot_t_c_have_new_mail00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_have_new_mail'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_have_new_mail();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_have_new_mail'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_open_one_mail of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_open_one_mail00
static int tolua_ToScript_script_robot_t_c_open_one_mail00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_open_one_mail'", NULL);
#endif
  {
   self->c_open_one_mail();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_open_one_mail'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_random_mail of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_random_mail00
static int tolua_ToScript_script_robot_t_c_random_mail00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  int mail_num = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_random_mail'", NULL);
#endif
  {
   self->c_random_mail(mail_num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_random_mail'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_mail_num of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_get_mail_num00
static int tolua_ToScript_script_robot_t_c_get_mail_num00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_mail_num'", NULL);
#endif
  {
   int tolua_ret = (int)  self->c_get_mail_num();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_mail_num'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_del_one_mail of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_del_one_mail00
static int tolua_ToScript_script_robot_t_c_del_one_mail00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_del_one_mail'", NULL);
#endif
  {
   self->c_del_one_mail();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_del_one_mail'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_addenda_mail_index of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_get_addenda_mail_index00
static int tolua_ToScript_script_robot_t_c_get_addenda_mail_index00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_addenda_mail_index'", NULL);
#endif
  {
   int tolua_ret = (int)  self->c_get_addenda_mail_index();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_addenda_mail_index'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_mail_addenda of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_get_mail_addenda00
static int tolua_ToScript_script_robot_t_c_get_mail_addenda00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_mail_addenda'", NULL);
#endif
  {
   self->c_get_mail_addenda(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_mail_addenda'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_one_key_del_all of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_one_key_del_all00
static int tolua_ToScript_script_robot_t_c_one_key_del_all00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_one_key_del_all'", NULL);
#endif
  {
   self->c_one_key_del_all();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_one_key_del_all'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_try_open_dungeon of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_try_open_dungeon00
static int tolua_ToScript_script_robot_t_c_try_open_dungeon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  int dungeon_id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_try_open_dungeon'", NULL);
#endif
  {
   self->c_try_open_dungeon(dungeon_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_try_open_dungeon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_is_in_dungeon of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_is_in_dungeon00
static int tolua_ToScript_script_robot_t_c_is_in_dungeon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  int dungeon_id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_is_in_dungeon'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_is_in_dungeon(dungeon_id);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_is_in_dungeon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_leave_dungeon of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_leave_dungeon00
static int tolua_ToScript_script_robot_t_c_leave_dungeon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_leave_dungeon'", NULL);
#endif
  {
   self->c_leave_dungeon();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_leave_dungeon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_is_all_killed of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_is_all_killed00
static int tolua_ToScript_script_robot_t_c_is_all_killed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_is_all_killed'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_is_all_killed();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_is_all_killed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_fight_index of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_get_fight_index00
static int tolua_ToScript_script_robot_t_c_get_fight_index00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_fight_index'", NULL);
#endif
  {
   int tolua_ret = (int)  self->c_get_fight_index();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_fight_index'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_dungeon_move_to of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_dungeon_move_to00
static int tolua_ToScript_script_robot_t_c_dungeon_move_to00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_dungeon_move_to'", NULL);
#endif
  {
   self->c_dungeon_move_to(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_dungeon_move_to'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_is_near_dungeon_target of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_is_near_dungeon_target00
static int tolua_ToScript_script_robot_t_c_is_near_dungeon_target00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_is_near_dungeon_target'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_is_near_dungeon_target(index);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_is_near_dungeon_target'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_try_enter_fight of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_try_enter_fight00
static int tolua_ToScript_script_robot_t_c_try_enter_fight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_try_enter_fight'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_try_enter_fight();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_try_enter_fight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_is_fighting of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_is_fighting00
static int tolua_ToScript_script_robot_t_c_is_fighting00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_is_fighting'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_is_fighting();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_is_fighting'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_try_auto_fight of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_try_auto_fight00
static int tolua_ToScript_script_robot_t_c_try_auto_fight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_try_auto_fight'", NULL);
#endif
  {
   self->c_try_auto_fight();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_try_auto_fight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_request_boss_info of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_request_boss_info00
static int tolua_ToScript_script_robot_t_c_request_boss_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_request_boss_info'", NULL);
#endif
  {
   self->c_request_boss_info(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_request_boss_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_boss_monster_id of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_get_boss_monster_id00
static int tolua_ToScript_script_robot_t_c_get_boss_monster_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_boss_monster_id'", NULL);
#endif
  {
   int tolua_ret = (int)  self->c_get_boss_monster_id();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_boss_monster_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_boss_map_id of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_get_boss_map_id00
static int tolua_ToScript_script_robot_t_c_get_boss_map_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_boss_map_id'", NULL);
#endif
  {
   int tolua_ret = (int)  self->c_get_boss_map_id();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_boss_map_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_boss_pos_x of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_get_boss_pos_x00
static int tolua_ToScript_script_robot_t_c_get_boss_pos_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_boss_pos_x'", NULL);
#endif
  {
   int tolua_ret = (int)  self->c_get_boss_pos_x();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_boss_pos_x'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_boss_pos_y of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_get_boss_pos_y00
static int tolua_ToScript_script_robot_t_c_get_boss_pos_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_boss_pos_y'", NULL);
#endif
  {
   int tolua_ret = (int)  self->c_get_boss_pos_y();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_boss_pos_y'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_try_attack_field_boss of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_try_attack_field_boss00
static int tolua_ToScript_script_robot_t_c_try_attack_field_boss00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  int monster_id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_try_attack_field_boss'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_try_attack_field_boss(monster_id);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_try_attack_field_boss'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_get_field_boss_exist_flag of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_get_field_boss_exist_flag00
static int tolua_ToScript_script_robot_t_c_get_field_boss_exist_flag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_get_field_boss_exist_flag'", NULL);
#endif
  {
   char tolua_ret = (char)  self->c_get_field_boss_exist_flag();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_get_field_boss_exist_flag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_clear_field_boss_exist_flag of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_clear_field_boss_exist_flag00
static int tolua_ToScript_script_robot_t_c_clear_field_boss_exist_flag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_clear_field_boss_exist_flag'", NULL);
#endif
  {
   self->c_clear_field_boss_exist_flag();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_clear_field_boss_exist_flag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_random_boss_id of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_random_boss_id00
static int tolua_ToScript_script_robot_t_c_random_boss_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_random_boss_id'", NULL);
#endif
  {
   int tolua_ret = (int)  self->c_random_boss_id();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_random_boss_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_is_join_arena of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_is_join_arena00
static int tolua_ToScript_script_robot_t_c_is_join_arena00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_is_join_arena'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_is_join_arena();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_is_join_arena'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_join_arena of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_join_arena00
static int tolua_ToScript_script_robot_t_c_join_arena00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_join_arena'", NULL);
#endif
  {
   self->c_join_arena();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_join_arena'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_arena_match of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_arena_match00
static int tolua_ToScript_script_robot_t_c_arena_match00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_arena_match'", NULL);
#endif
  {
   self->c_arena_match();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_arena_match'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_is_arena_matched of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_is_arena_matched00
static int tolua_ToScript_script_robot_t_c_is_arena_matched00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_is_arena_matched'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->c_is_arena_matched();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_is_arena_matched'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_buy_month_card of class  script_robot_t */
#ifndef TOLUA_DISABLE_tolua_ToScript_script_robot_t_c_buy_month_card00
static int tolua_ToScript_script_robot_t_c_buy_month_card00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"script_robot_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  script_robot_t* self = (script_robot_t*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_buy_month_card'", NULL);
#endif
  {
   self->c_buy_month_card(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_buy_month_card'.",&tolua_err);
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
  tolua_function(tolua_S,"c_debug",tolua_ToScript_c_debug00);
  tolua_function(tolua_S,"c_info",tolua_ToScript_c_info00);
  tolua_function(tolua_S,"c_error",tolua_ToScript_c_error00);
  tolua_function(tolua_S,"c_assert_fail",tolua_ToScript_c_assert_fail00);
  tolua_function(tolua_S,"c_get_root_dir",tolua_ToScript_c_get_root_dir00);
  tolua_function(tolua_S,"c_do_file",tolua_ToScript_c_do_file00);
  tolua_function(tolua_S,"c_get_file_list_by_folder",tolua_ToScript_c_get_file_list_by_folder00);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"script_robot_t","script_robot_t","ScriptProxy<robot_t>",tolua_collect_script_robot_t);
  #else
  tolua_cclass(tolua_S,"script_robot_t","script_robot_t","ScriptProxy<robot_t>",NULL);
  #endif
  tolua_beginmodule(tolua_S,"script_robot_t");
   tolua_function(tolua_S,"delete",tolua_ToScript_script_robot_t_delete00);
   tolua_function(tolua_S,"c_test_robot",tolua_ToScript_script_robot_t_c_test_robot00);
   tolua_function(tolua_S,"c_move_to",tolua_ToScript_script_robot_t_c_move_to00);
   tolua_function(tolua_S,"c_fast_move",tolua_ToScript_script_robot_t_c_fast_move00);
   tolua_function(tolua_S,"c_move_arrive",tolua_ToScript_script_robot_t_c_move_arrive00);
   tolua_function(tolua_S,"c_move_around",tolua_ToScript_script_robot_t_c_move_around00);
   tolua_function(tolua_S,"c_get_scene",tolua_ToScript_script_robot_t_c_get_scene00);
   tolua_function(tolua_S,"c_robot_done",tolua_ToScript_script_robot_t_c_robot_done00);
   tolua_function(tolua_S,"c_get_currX",tolua_ToScript_script_robot_t_c_get_currX00);
   tolua_function(tolua_S,"c_get_currY",tolua_ToScript_script_robot_t_c_get_currY00);
   tolua_function(tolua_S,"c_talk_about",tolua_ToScript_script_robot_t_c_talk_about00);
   tolua_function(tolua_S,"c_jump_scene",tolua_ToScript_script_robot_t_c_jump_scene00);
   tolua_function(tolua_S,"c_jump_id",tolua_ToScript_script_robot_t_c_jump_id00);
   tolua_function(tolua_S,"c_gm_command",tolua_ToScript_script_robot_t_c_gm_command00);
   tolua_function(tolua_S,"c_toggle_mount",tolua_ToScript_script_robot_t_c_toggle_mount00);
   tolua_function(tolua_S,"c_get_mail_list",tolua_ToScript_script_robot_t_c_get_mail_list00);
   tolua_function(tolua_S,"c_have_new_mail",tolua_ToScript_script_robot_t_c_have_new_mail00);
   tolua_function(tolua_S,"c_open_one_mail",tolua_ToScript_script_robot_t_c_open_one_mail00);
   tolua_function(tolua_S,"c_random_mail",tolua_ToScript_script_robot_t_c_random_mail00);
   tolua_function(tolua_S,"c_get_mail_num",tolua_ToScript_script_robot_t_c_get_mail_num00);
   tolua_function(tolua_S,"c_del_one_mail",tolua_ToScript_script_robot_t_c_del_one_mail00);
   tolua_function(tolua_S,"c_get_addenda_mail_index",tolua_ToScript_script_robot_t_c_get_addenda_mail_index00);
   tolua_function(tolua_S,"c_get_mail_addenda",tolua_ToScript_script_robot_t_c_get_mail_addenda00);
   tolua_function(tolua_S,"c_one_key_del_all",tolua_ToScript_script_robot_t_c_one_key_del_all00);
   tolua_function(tolua_S,"c_try_open_dungeon",tolua_ToScript_script_robot_t_c_try_open_dungeon00);
   tolua_function(tolua_S,"c_is_in_dungeon",tolua_ToScript_script_robot_t_c_is_in_dungeon00);
   tolua_function(tolua_S,"c_leave_dungeon",tolua_ToScript_script_robot_t_c_leave_dungeon00);
   tolua_function(tolua_S,"c_is_all_killed",tolua_ToScript_script_robot_t_c_is_all_killed00);
   tolua_function(tolua_S,"c_get_fight_index",tolua_ToScript_script_robot_t_c_get_fight_index00);
   tolua_function(tolua_S,"c_dungeon_move_to",tolua_ToScript_script_robot_t_c_dungeon_move_to00);
   tolua_function(tolua_S,"c_is_near_dungeon_target",tolua_ToScript_script_robot_t_c_is_near_dungeon_target00);
   tolua_function(tolua_S,"c_try_enter_fight",tolua_ToScript_script_robot_t_c_try_enter_fight00);
   tolua_function(tolua_S,"c_is_fighting",tolua_ToScript_script_robot_t_c_is_fighting00);
   tolua_function(tolua_S,"c_try_auto_fight",tolua_ToScript_script_robot_t_c_try_auto_fight00);
   tolua_function(tolua_S,"c_request_boss_info",tolua_ToScript_script_robot_t_c_request_boss_info00);
   tolua_function(tolua_S,"c_get_boss_monster_id",tolua_ToScript_script_robot_t_c_get_boss_monster_id00);
   tolua_function(tolua_S,"c_get_boss_map_id",tolua_ToScript_script_robot_t_c_get_boss_map_id00);
   tolua_function(tolua_S,"c_get_boss_pos_x",tolua_ToScript_script_robot_t_c_get_boss_pos_x00);
   tolua_function(tolua_S,"c_get_boss_pos_y",tolua_ToScript_script_robot_t_c_get_boss_pos_y00);
   tolua_function(tolua_S,"c_try_attack_field_boss",tolua_ToScript_script_robot_t_c_try_attack_field_boss00);
   tolua_function(tolua_S,"c_get_field_boss_exist_flag",tolua_ToScript_script_robot_t_c_get_field_boss_exist_flag00);
   tolua_function(tolua_S,"c_clear_field_boss_exist_flag",tolua_ToScript_script_robot_t_c_clear_field_boss_exist_flag00);
   tolua_function(tolua_S,"c_random_boss_id",tolua_ToScript_script_robot_t_c_random_boss_id00);
   tolua_function(tolua_S,"c_is_join_arena",tolua_ToScript_script_robot_t_c_is_join_arena00);
   tolua_function(tolua_S,"c_join_arena",tolua_ToScript_script_robot_t_c_join_arena00);
   tolua_function(tolua_S,"c_arena_match",tolua_ToScript_script_robot_t_c_arena_match00);
   tolua_function(tolua_S,"c_is_arena_matched",tolua_ToScript_script_robot_t_c_is_arena_matched00);
   tolua_function(tolua_S,"c_buy_month_card",tolua_ToScript_script_robot_t_c_buy_month_card00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_ToScript (lua_State* tolua_S) {
 return tolua_ToScript_open(tolua_S);
};
#endif

