#ifndef __COMMON_RANDOM_UUID_HPP__ 
#define __COMMON_RANDOM_UUID_HPP__ 

#include "macros.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <random>
#include <string>


NS_COMMON_BEGIN

class uuid_t
{
public:
    uuid_t() : m_gen(&m_ran)
    {
        std::random_device dev;
        m_ran.seed(dev()); // one should likely seed in a better way
    }

    boost::uuids::uuid gen() 
    { 
        return m_gen();
    }

    static std::string random_uuid();

private:
    boost::mt19937 m_ran;
    boost::uuids::basic_random_generator<boost::mt19937> m_gen;
};

NS_COMMON_END
#endif
