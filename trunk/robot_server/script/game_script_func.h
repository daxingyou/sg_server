#pragma once

//tolua_begin

/// @brief ���������־
void c_debug( const char* msg );

/// @brief ���������־
void c_info(const char* msg);

/// @brief ���������־
void c_error(const char* msg);

/// @brief ����ʧ��
void c_assert_fail( const char* msg );

/// @brief ��ýű���Ŀ¼
const char* c_get_root_dir();

/// @brief ����һ���ű��ļ�
bool c_do_file( const char* filename );

/// @brief ����ļ����µ������ļ���
const char* c_get_file_list_by_folder( const char* folder );


//tolua_end