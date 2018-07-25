#include "random_uuid.hpp"


NS_COMMON_BEGIN

std::string uuid_t::random_uuid()
{
    static uuid_t gen;
    return to_string(gen.gen());
}

NS_COMMON_END