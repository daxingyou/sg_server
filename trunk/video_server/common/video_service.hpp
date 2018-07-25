#pragma once

/**
* \brief ����¼��������
*/

#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <map>

#include "video_unit.hpp"
#include "video_common.hpp"

class video_service_t : public redis_base_t
{
public:
	// ����
	virtual ~video_service_t();

	// ��ʼ��
	bool init(const char* host, // redis ip
		int32_t port,			// redis port
		const char* passwd,		// redis auth
		int32_t db,				// redis select
		uint32_t process_num,	// ����Ԫ�����������߳�����
		struct ConnectionPool_S* zdb_pool);

	// �ر�
	void close();

	// �ַ�¼����Ϣ
	void recv_video_message(uint64_t id, const network::msg_buf_ptr& msg_buf);

public:
	struct field_data
	{
		field_data(const std::string& name, DB_TYPE type, bool is_group, bool is_layer, uint32_t ctype)
			: field_name(name)
			, field_type(type)
			, need_group(is_group)
			, need_layer(is_layer)
			, class_type(ctype)
		{
		}

		std::string field_name;
		DB_TYPE field_type;
		bool need_group;
		bool need_layer;
		uint32_t class_type;
	};
	typedef std::vector<field_data> field_list;

	const field_list& get_field_list() const { return m_field_list; }

	// �ϰ����ݣ��������ֲ㣨����У���ϸ�����ͣ�
	typedef std::map<uint32_t, std::map<uint32_t, std::map<uint32_t, std::vector<uint64_t>>>> fight_top_data;

protected:
	// ���캯��
	video_service_t(const std::string &name);
	// ������ṹ
	bool init_table_field(struct ConnectionPool_S* zdb_pool);
	// ���ذ�������
	bool load_rank_data(struct ConnectionPool_S* zdb_pool, const char* sql, fight_top_data& fight_uids);
	// ��ʼ������Ԫ
	virtual bool init_unit(const char* host, int32_t port, const char* passwd, int32_t db, struct ConnectionPool_S* zdb_pool, uint32_t num) = 0;
	// ��mysql��������
	virtual bool start_read_from_db(struct ConnectionPool_S* zdb_pool) = 0;

protected:
	// ���redis״̬
	uint32_t get_redis_state();
	// ����redis״̬
	void set_redis_state(uint32_t state);
	// ��ȡ�����Ļص�
	void read_db_done();

protected:
	boost::asio::io_service m_io_service;
	boost::scoped_ptr<boost::asio::io_service::work> m_work;
	boost::thread_group m_thread_group;

	field_list m_field_list;

	typedef std::vector<video_unit_ptr> video_unit_list;
	video_unit_list m_video_unit_list;

	std::string m_service_name;
	std::string m_redis_state;
};