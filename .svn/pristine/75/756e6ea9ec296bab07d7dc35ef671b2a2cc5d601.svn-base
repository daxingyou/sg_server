#include "video_unit.hpp"

video_unit_t::video_unit_t(boost::asio::io_service& ios)
	: redis_unit_t(ios)
	, m_is_stop(false)
	, m_in_process(false)
{

}

video_unit_t::~video_unit_t()
{

}

void video_unit_t::set_stop()
{
	auto self(shared_from_this());
	m_strand.post([this, self]()
	{
		m_is_stop = true;
	});
}

void video_unit_t::post_new_video(uint64_t id, const network::msg_buf_ptr& msg_buf)
{
	auto self(shared_from_this());
	m_strand.post([this, self, id, msg_buf]()
	{
		if (m_is_stop) {
			return;
		}

		m_video_msg_deque.push_back(std::make_pair(id, msg_buf));
		do_process_video(self);
	});
}

void video_unit_t::do_process_video(const video_unit_ptr& self)
{
	if (!m_in_process) {
		if (!m_video_msg_deque.empty()) {
			m_in_process = true;
			std::pair<uint64_t, network::msg_buf_ptr> pair = m_video_msg_deque.front();
			m_video_msg_deque.pop_front();
			m_strand.post([this, self, pair]()
			{
				do_save_video(pair.first, pair.second);
				m_in_process = false;
				do_process_video(self);
			});
		}
	}
}

