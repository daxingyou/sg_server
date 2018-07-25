#ifndef __ROBOT_SERVER_FWD_HPP__
#define __ROBOT_SERVER_FWD_HPP__

#include <boost/asio.hpp>


class robot_server_t;
struct robot_config_s;
struct env
{
    static robot_server_t *server;
    static boost::asio::signal_set *signals;
    static struct robot_config_s *robot_config; 
};




#endif  //__ROBOT_SERVER_FWD_HPP__
