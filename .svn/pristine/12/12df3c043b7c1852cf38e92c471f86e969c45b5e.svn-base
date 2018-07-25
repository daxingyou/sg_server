#include "tcp_socket.hpp"
#include "log.hpp"
#include <boost/bind.hpp>


NS_NETWORK_BEGIN
USING_NS_COMMON;

tcp_socket_t::tcp_socket_t(boost::asio::io_service& ios, io_handler_t* handler)
    : m_io_service(ios)
    , m_inner_socket(ios)
    , m_strand(ios)
    , m_write_in_pending(false)
    , m_idle_index(0)
    , m_io_handler(handler)
    , m_bind_data(NULL)
{
    m_wrap_buf.clear();
    m_write_buf[0].clear();
    m_write_buf[1].clear();
}

tcp_socket_t::~tcp_socket_t()
{
}

//----------------------------------------------------------------------------------------------
void tcp_socket_t::read()
{
    if (is_closed())
    {
        //log_warn("socket[%d:%d] closed!", m_inner_socket.native(), m_socket_id);
        return;
    }

    m_inner_socket.async_read_some(boost::asio::buffer(m_read_buf),
        m_strand.wrap(boost::bind(&tcp_socket_t::handle_read, 
                shared_from_this(), 
                boost::asio::placeholders::error, 
                boost::asio::placeholders::bytes_transferred)));
}

void tcp_socket_t::handle_read(const boost::system::error_code& ec, size_t bytes)
{
    if (is_closed())
    {
        //log_warn("socket[%d:%d] closed!", m_inner_socket.native(), m_socket_id);
        return;
    }

    if (NULL == m_io_handler)
    {
        log_error("NULL == m_io_handler");
        return;
    }

    if (ec)
    {
        if (ec == boost::asio::error::operation_aborted)
        {
            return;
        }

        m_io_handler->on_read_error(shared_from_this(), ec);

        return;
    }

    if (m_io_handler->on_read(shared_from_this(), m_read_buf.begin(), bytes))
    {
        return;
    }

    try
    {
        if (m_msg_parser.parse(m_read_buf.begin(), bytes))
        {
            std::vector<message_t>& msg_vec = m_msg_parser.packs();
            size_t vec_size = msg_vec.size();
            for (size_t i = 0; i < vec_size; ++i)
            {
                if (m_io_handler)
                {
                    m_io_handler->on_new_msg(shared_from_this(), msg_vec[i]);
                }
                else
                {
                    log_error("m_io_handler is null");
                }
            }
            msg_vec.clear();
        }
    }
    catch(const std::exception& e)
    {
        log_error("ec:%s", e.what());
        close();
        return;
    }

    read();
}

//----------------------------------------------------------------------------------------------
void tcp_socket_t::write(const msg_buf_ptr& buf)
{
    if (is_closed())
    {
        //log_warn("socket[%d:%d] closed!", m_inner_socket.native(), m_socket_id);
        return;
    }

    m_strand.dispatch(boost::bind(&tcp_socket_t::do_write, shared_from_this(), buf));
}

void tcp_socket_t::do_write(const msg_buf_ptr& buf)
{
    if (is_closed())
    {
        //log_warn("socket[%d:%d] closed!", m_inner_socket.native(), m_socket_id);
        return;
    }

    m_write_buf[m_idle_index].push_back(buf);
    if (!m_write_in_pending)
    {
        if (!m_write_buf[m_idle_index].empty())
        {
            wrap_idle_buffers();
            m_idle_index = another_index(m_idle_index);
            boost::asio::async_write(m_inner_socket,
                m_wrap_buf,
                m_strand.wrap(boost::bind(&tcp_socket_t::handle_write,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred)));
            m_write_in_pending = true;
        }
    }
}

void tcp_socket_t::handle_write(const boost::system::error_code& ec, size_t bytes)
{
    if (is_closed())
    {
        //log_warn("socket[%d:%d] closed!", m_inner_socket.native(), m_socket_id);
        return;
    }
    
    if (NULL == m_io_handler)
    {
        log_error("NULL == m_io_handler");
        return;
    }

    if (ec)
    {
        if (ec == boost::asio::error::operation_aborted)
        {
            return;
        }

        m_io_handler->on_write_error(shared_from_this(), ec);
        return;
    }

    if (rewrap_idle_buffers(bytes))
    {
        return;
    }

    // swap and send idle buffers
    m_write_in_pending  = false;
    size_t pending_index = another_index(m_idle_index);
    m_write_buf[pending_index].clear();
    m_wrap_buf.clear();
    if (!m_write_buf[m_idle_index].empty())
    {
        wrap_idle_buffers();
        m_idle_index = pending_index;
        boost::asio::async_write(m_inner_socket, 
            m_wrap_buf, 
            m_strand.wrap(boost::bind(&tcp_socket_t::handle_write, 
            shared_from_this(), 
            boost::asio::placeholders::error, 
            boost::asio::placeholders::bytes_transferred)));
        m_write_in_pending = true;
    }
}

void tcp_socket_t::wrap_idle_buffers()
{
    if (is_closed())
    {
        //log_warn("socket[%d:%d] closed!", m_inner_socket.native(), m_socket_id);
        return;
    }

    m_wrap_buf.clear();
    m_wrap_buf.reserve(m_write_buf[m_idle_index].size());
    std::deque<msg_buf_ptr>::iterator itr = m_write_buf[m_idle_index].begin();
    for (; itr != m_write_buf[m_idle_index].end(); ++itr)
    {
        m_wrap_buf.push_back(boost::asio::buffer((*itr)->data(), (*itr)->size()));
    }
}

bool tcp_socket_t::rewrap_idle_buffers(size_t bytes)
{
    if (is_closed())
    {
        //log_warn("socket[%d:%d] closed!", m_inner_socket.native(), m_socket_id);
        return false;
    }

    int pending_index = another_index(m_idle_index);
    std::deque<msg_buf_ptr>& bufs = m_write_buf[pending_index];

    for (std::deque<msg_buf_ptr>::iterator iter = bufs.begin(); iter != bufs.end();)
    {
        if (bytes < (*iter)->size())
        {
            break;
        }

        bytes -= (*iter)->size();
        iter = bufs.erase(iter);
    }

    if ((bytes == 0) && bufs.empty())
        return false;

    char* p = bufs.front()->data() + bytes;
    size_t remain = bufs.front()->size() - bytes;

    m_wrap_buf.clear();
    m_wrap_buf.reserve(bufs.size());
    m_wrap_buf.push_back(boost::asio::buffer(p, remain));

    std::deque<msg_buf_ptr>::iterator itr = bufs.begin();
    for (++itr; itr != bufs.end(); ++itr)
    {
        m_wrap_buf.push_back(boost::asio::buffer((*itr)->data(), (*itr)->size()));
    }
    return true;
}

//---------------------------------------------------------
void tcp_socket_t::close()
{
    if (is_closed())
    {
        //log_warn("socket[%d:%d] closed!", m_inner_socket.native(), m_socket_id);
        return;
    }

    m_strand.dispatch(boost::bind(&tcp_socket_t::do_close, shared_from_this()));
}

void tcp_socket_t::do_close()
{
    if (is_closed())
    {
        //log_warn("socket[%d:%d] closed!", m_inner_socket.native(), m_socket_id);
        return;
    }

    m_inner_socket.close();
    m_wrap_buf.clear();
    m_write_buf[0].clear();
    m_write_buf[1].clear();
    m_write_in_pending = false;
    m_idle_index = 0;

    //log_warn("socket[%d:%d] closed!", m_inner_socket.native(), m_socket_id);
}

NS_NETWORK_END
