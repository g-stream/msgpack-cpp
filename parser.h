#ifndef PARSER_H_
#define PARSER_H_ 
#include "config.h"
#include <cstdint>
namespace msgpack{
    //to define function is_ext and is_fix
#define CHECK_PROPERTIES(name) \
bool is_##name(uint8_t byte){                                                        \
    if(!(byte>>7)){                                                                    \
        return msgpack::is_##name##_map.at(msgpack::TYPE_PREFIX::FIX_UNINT);            \
    } else if (byte >> 5 == static_cast<uint8_t>(msgpack::TYPE_PREFIX::FIX_NGINT)){  \
        return msgpack::is_##name##_map.at(msgpack::TYPE_PREFIX::FIX_NGINT);            \
    } else if (byte >> 4 == static_cast<uint8_t>(msgpack::TYPE_PREFIX::FIX_MAP)){    \
        return msgpack::is_##name##_map.at(msgpack::TYPE_PREFIX::FIX_MAP);              \
    } else if (byte >> 4 == static_cast<uint8_t>(msgpack::TYPE_PREFIX::FIX_ARRAY)){  \
        return msgpack::is_##name##_map.at(msgpack::TYPE_PREFIX::FIX_ARRAY);            \
    } else if (byte >> 5 == static_cast<uint8_t>(msgpack::TYPE_PREFIX::FIX_STR)){    \
        return msgpack::is_##name##_map.at(msgpack::TYPE_PREFIX::FIX_STR);              \
    }                                                                                \
    return msgpack::is_##name##_map.at(static_cast<msgpack::TYPE_PREFIX>(byte));        \
}
CHECK_PROPERTIES(ext);
CHECK_PROPERTIES(fix);
#undef CHECK_PROPERTIES
}


#endif