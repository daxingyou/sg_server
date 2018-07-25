#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:SysChannelBase
/// @remark   ��¼��С:28 X32
///           ��¼��С:28 X64
///           �ֶ�����:7
///           �����ļ�:ϵͳƵ����.xlsx Sheet1
///           ��������:����,��ز���
#pragma pack(1)
struct SysChannelBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x010DD939; ///< II|IIIII
#else
	static const int          FormatCheckCode = 0x010DD939; ///< II|IIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return common::hash_util_t::hash_to_uint32(type, paramId); }; // TODO: ���� ����,��ز��� ����Ψһ��

	uint32_t                  type;                         ///< ����
	uint32_t                  is_notify;                    ///< �Ƿ�㲥

	uint32_t                  paramId;                      ///< ��ز���
	uint32_t                  count;                        ///< �㲥����
	uint32_t                  notify_id;                    ///< ϵͳ֪ͨID
	uint32_t                  language_id;                  ///< ���Ա�ID
	uint32_t                  label;                        ///< ��ǩ����
};
#pragma pack()

struct SysChannel : public Entry
{
	uint32_t                  type()                        { return t_type; }
	uint32_t                  is_notify()                   { return t_is_notify; }

	uint32_t                  paramId()                     { return t_paramId; }
	uint32_t                  count()                       { return t_count; }
	uint32_t                  notify_id()                   { return t_notify_id; }
	uint32_t                  language_id()                 { return t_language_id; }
	uint32_t                  label()                       { return t_label; }

private:
	uint32_t                  t_type;                       ///< ����
	uint32_t                  t_is_notify;                  ///< �Ƿ�㲥

	uint32_t                  t_paramId;                    ///< ��ز���
	uint32_t                  t_count;                      ///< �㲥����
	uint32_t                  t_notify_id;                  ///< ϵͳ֪ͨID
	uint32_t                  t_language_id;                ///< ���Ա�ID
	uint32_t                  t_label;                      ///< ��ǩ����

public:
	void reset();
	void fill( const SysChannelBase& base );
	const char* getClassName() const { return "ϵͳƵ����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SysChannel, SysChannelBase > SysChannelManager;

inline void SysChannel::reset()
{
	this->t_type = 0;
	this->t_is_notify = 0;
	this->t_paramId = 0;
	this->t_count = 0;
	this->t_notify_id = 0;
	this->t_language_id = 0;
	this->t_label = 0;
};

inline void SysChannel::fill( const SysChannelBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_type = base.type;
	this->t_is_notify = base.is_notify;
	this->t_paramId = base.paramId;
	this->t_count = base.count;
	this->t_notify_id = base.notify_id;
	this->t_language_id = base.language_id;
	this->t_label = base.label;
};

inline const std::string SysChannel::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SysChannel.tbls64";
#else
	return "SysChannel.tbls32";
#endif
};
