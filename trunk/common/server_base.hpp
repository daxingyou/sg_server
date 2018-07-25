#ifndef __COMMON_SERVER_BASE_HPP__
#define __COMMON_SERVER_BASE_HPP__

#include "common_fwd.hpp"
#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>

NS_COMMON_BEGIN

class server_base_t
{
public:
    server_base_t();
    virtual ~server_base_t();

    bool run_server();
    bool close_server();

    inline boost::asio::io_service& get_ios() { return m_ios; }

	virtual const char* server_name() { return ""; }
	virtual bool on_init_server() { return false; }
	virtual bool on_close_server() { return false; }
	virtual void on_run_server() {};
	
	// ��������������ź�
	void terminate();
	// �ܷ�������ź�
	virtual bool check_terminate() const { return true; }
	// �������
	bool is_terminate() { return m_terminate; }

	/**
	 * \brief ���ط����ʵ��ָ��
	 * \return �����ʵ��ָ��
	 */
	static server_base_t *serverInstance()
	{
		return m_server;
	}

private:
    bool init_server();

public:
    boost::asio::io_service m_ios;
    boost::scoped_ptr<boost::asio::io_service::work> m_work;

private:
	static server_base_t* m_server;
	volatile bool m_terminate;	/**< ���������� */
};

NS_COMMON_END
#endif
