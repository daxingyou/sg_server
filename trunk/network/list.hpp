#ifndef _COMMON_LIST_HPP_
#define _COMMON_LIST_HPP_

#include <list>
#include "tcp_socket.hpp"
NS_NETWORK_BEGIN
template<typename Elem, typename Ax = std::allocator<Elem> >
struct list_t
{
    typedef std::list<Elem, Ax> type;
};

struct ITER_T
{
	ITER_T(list_t<tcp_socket_wptr>::type::iterator _i)
		: i(_i) {}

	list_t<tcp_socket_wptr>::type::iterator i;
};
NS_NETWORK_END
#endif /*_COMMON_LIST_HPP_*/
