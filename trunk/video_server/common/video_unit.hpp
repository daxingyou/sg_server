#pragma once

/**
* \brief 录像服务处理单元
*/

#include "redis_unit.hpp"
#include "network/message.hpp"

class video_unit_t;
typedef boost::shared_ptr<video_unit_t> video_unit_ptr;

class video_unit_t : public redis_unit_t, public boost::enable_shared_from_this<video_unit_t>
{
public:
	explicit video_unit_t(boost::asio::io_service& ios);
	virtual ~video_unit_t();

	// 设置结束处理状态
	void set_stop();
	// 处理新的录像
	void post_new_video(uint64_t id, const network::msg_buf_ptr& msg_buf);

protected:
	// 处理新的录像请求
	void do_process_video(const video_unit_ptr& self);
	// 保存处理
	virtual void do_save_video(uint64_t id, const network::msg_buf_ptr& msg_buf) = 0;

protected:
	typedef std::deque<std::pair<uint64_t, network::msg_buf_ptr>> video_msg_deque;
	video_msg_deque m_video_msg_deque;

	bool m_is_stop = false;
	bool m_in_process = false;
};