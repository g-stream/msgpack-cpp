#ifndef CONFIG_H_
#define CONFIG_H_
#include <map>
#include <cstdint>
namespace msgpack
{
    union EndianTest{
        uint16_t data;
        uint8_t brother[2];
    };
    static const EndianTest endian_test{0x0001};
    const bool BigEndian = endian_test.brother[0] == 0;
    
    enum class TYPE {
        Integer,
        Nil,
        Boolean,
        Float,
        Raw,
        Array,
        Map,
        Extension
    };

    enum TYPE_PREFIX {
        FIX_UNINT        =  0x0,
        FIX_NGINT        =	0b111,
        FIX_MAP          =  0b1000,
        FIX_ARRAY        =  0b1001,

        FIX_STR          =  0b101,
        
        NIL              =  0xc0,
        RESERVERD        =  0xc1,
        FALSE            =  0xC2,
        TRUE             =  0xc3,
        BIN8             =  0xc4,
        BIN16            =  0xc5,
        BIN32            =  0xc6,
        EXT8             =	0xc7,
        EXT16            =	0xc8,
        EXT32            =	0xc9,
        FLOAT32          =	0xca,
        FLOAT64          =	0xcb,
        UINT8            =	0xcc,
        UINT16           =	0xcd,
        UINT32           =	0xce,
        UINT64           =	0xcf,
        INT8             =	0xd0,
        INT16            =	0xd1,
        INT32            =	0xd2,
        INT64            =	0xd3,
        FIX_EXT1         =	0xd4,
        FIX_EXT2         =	0xd5,
        FIX_EXT4         =	0xd6,
        FIX_EXT8         =	0xd7,
        FIX_EXT16        =	0xd8,
        STR8             =	0xd9,
        STR16            =	0xda,
        STR32            =	0xdb,
        ARRAY16          =	0xdc,
        ARRAY32          =	0xdd,
        MAP16            =	0xde,
        MAP32            =	0xdf
    };
    #define DEFINE_PROPERTY_PAIR(name, value) {name, value},
    const std::map<TYPE_PREFIX, bool> is_fix_map(
        {
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_UNINT, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_NGINT, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_MAP, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_ARRAY, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_STR, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::NIL, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::RESERVERD, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FALSE, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::TRUE, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::BIN8, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::BIN16, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::BIN32, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::EXT8, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::EXT16, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::EXT32, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FLOAT32, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FLOAT64, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::UINT8, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::UINT16, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::UINT32, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::UINT64, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::INT8, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::INT16, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::INT32, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::INT64, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_EXT1, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_EXT2, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_EXT4, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_EXT8, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_EXT16, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::STR8, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::STR16, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::STR32, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::ARRAY16, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::ARRAY32, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::MAP16, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::MAP32, false)
        }
    );
    const std::map<TYPE_PREFIX, bool> is_ext_map(
        {
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_UNINT, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_NGINT, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_MAP, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_ARRAY, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_STR, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::NIL, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::RESERVERD, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FALSE, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::TRUE, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::BIN8, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::BIN16, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::BIN32, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::EXT8, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::EXT16, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::EXT32, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FLOAT32, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FLOAT64, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::UINT8, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::UINT16, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::UINT32, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::UINT64, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::INT8, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::INT16, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::INT32, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::INT64, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_EXT1, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_EXT2, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_EXT4, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_EXT8, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::FIX_EXT16, true)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::STR8, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::STR16, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::STR32, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::ARRAY16, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::ARRAY32, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::MAP16, false)
            DEFINE_PROPERTY_PAIR(TYPE_PREFIX::MAP32, false)
        }
    );
    #undef DEFINE_PROPERTY_PAIR
}




#endif
