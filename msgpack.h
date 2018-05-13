#pragma once
#ifndef MSGPACK_H_
#define MSGPACK_H_
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <cstdint>
#include <memory>
#include <type_traits>
namespace msgpack
{

namespace {
union endian_check_t {
    uint16_t data;
    uint8_t check[2];
};
const endian_check_t endian_check_data {
    0xffef
};
const bool IsBigEndian = endian_check_data.check[0] == 0xff;
const bool IsLittleEndian = !IsBigEndian;
}

using std::map;
using std::string;
using std::vector;
using std::is_same;
struct MpkNil
{
};
enum class TYPE
{
    NIL,
    BOOL,
    INT,
    FLOAT,
    STRING,
    BIN,
    ARRAY,
    MAP,
    EXTENSION
};
auto mpk_nil = TYPE::NIL;
auto mpk_bool = TYPE::BOOL;
auto mpk_int = TYPE::INT;
auto mpk_float = TYPE::FLOAT;
auto mpk_string = TYPE::STRING;
auto mpk_bin = TYPE::BIN;
auto mpk_array = TYPE::ARRAY;
auto mpk_map = TYPE::MAP;
auto mpk_extension = TYPE::EXTENSION;

static const uint8_t prefix_nil = 0xc0;
static const uint8_t prefix_false = 0xc2;
static const uint8_t prefix_true = 0xc3;
static const uint8_t prefix_bin8 = 0xc4;
static const uint8_t prefix_bin16 = 0xc5;
static const uint8_t prefix_bin32 = 0xc6;
static const uint8_t prefix_ext8 = 0xc7;
static const uint8_t prefix_ext16 = 0xc8;
static const uint8_t prefix_ext32 = 0xc9;
static const uint8_t prefix_float32 = 0xca;
static const uint8_t prefix_float64 = 0xcb;
static const uint8_t prefix_uint8 = 0xcc;
static const uint8_t prefix_uint16 = 0xcd;
static const uint8_t prefix_uint32 = 0xce;
static const uint8_t prefix_uint64 = 0xcf;
static const uint8_t prefix_int8 = 0xd0;
static const uint8_t prefix_int16 = 0xd1;
static const uint8_t prefix_int32 = 0xd2;
static const uint8_t prefix_int64 = 0xd3;
static const uint8_t prefix_fixext1 = 0xd4;
static const uint8_t prefix_fixext2 = 0xd5;
static const uint8_t prefix_fixext4 = 0xd6;
static const uint8_t prefix_fixext8 = 0xd7;
static const uint8_t prefix_fixext16 = 0xd8;
static const uint8_t prefix_str8 = 0xd9;
static const uint8_t prefix_str16 = 0xda;
static const uint8_t prefix_str32 = 0xdb;
static const uint8_t prefix_array16 = 0xdc;
static const uint8_t prefix_array32 = 0xdd;
static const uint8_t prefix_map16 = 0xde;
static const uint8_t prefix_map32 = 0xdf;

static void* enabler = nullptr;
class MpkValue;
class mpk
{
public:
    mpk();//return a mpk_nil
    mpk(bool);
    mpk(uint8_t);
    mpk(uint16_t);
    mpk(uint32_t);
    mpk(uint64_t);
    mpk(int8_t);
    mpk(int16_t);
    mpk(int32_t);
    mpk(int64_t);
    mpk(float);
    mpk(double);
    mpk(string);
    mpk(string, msgpack::TYPE);
    mpk(char* str, size_t n);
    mpk(vector<mpk>);
    mpk(map<string, mpk>);
    mpk(vector<mpk>::iterator, vector<mpk>::iterator);
    const msgpack::TYPE type;
    bool operator<(const mpk &);
private:
    std::shared_ptr<MpkValue> m_ptr;
};

class MpkValue {
public:
    virtual void encode(vector<uint8_t>&);
    virtual msgpack::TYPE type() const = 0;
    virtual bool operator<(const MpkValue&) const = 0;
    virtual bool operator==(const MpkValue&) const = 0;
    bool operator>(const MpkValue& other) const {
        return other < *this;
    }
    bool operator<=(const MpkValue& other) const {
        return ! (other<*this);
    }
    bool operator>=(const MpkValue& other) const {
        return !(*this < other);
    }
    bool operator!=(const MpkValue& other) const {
        return !(*this == other);
    }
};

template<msgpack::TYPE type_tag, typename T>
class Value:public MpkValue {
public:
    explicit Value(const T& v):m_value(v){}
    msgpack::TYPE type() const {
        return type_tag;
    }
    bool operator<(const MpkValue& other) const override{
        if(type() != other.type())
            return type() < other.type();
        return m_value < other.m_value;
    }
    
private:
    T m_value;
};
mpk_bool_t:public Value<mpk_bool, bool>{
}
mpk_nil_t:public Value<mpk_nil, MpkNil>{}

mpk_int8_t:public Value<mpk_int, int8_t>{
    
}
mpk_int16_t:public Value<mpk_int, int16_t>{}
mpk_int32_t:public Value<mpk_int, int32_t>{}
mpk_int64_t:public Value<mpk_int, int64_t>{}
mpk_uint8_t:public Value<mpk_int, uint8_t>{}
mpk_uint16_t:public Value<mpk_int, uint16_t>{}
mpk_uint32_t:public Value<mpk_int, uint32_t>{}
mpk_uint64_t:public Value<mpk_int, uint64_t>{}


mpk_float32_t:public Value<mpk_float, float>{}
mpk_float64_t:public Value<mpk_float, double>{}

mpk_bin_t:public Value<mpk_bin, vector<uint8_t>>{}
mpk_string_t:public Value<mpk_string, string>>{}


mpk_array_t:public Value<mpk_array, vector<mpk>>{}
mpk_map_t:public Value<mpk_map, map<mpk, mpk>>{}

namespace {

template<typename T>
class encode_value {
public:
    union U {
        uint8_t data_to_encode[sizeof(T)];
        T data;
    };
    encode_value(T d) {
        d_.data = d;
    }
    void operator()(vector<uint8_t>& out) {
        if(IsBigEndian) {
            for(int i = 0; i < sizeof(T); i++) {
                out.push_back(d_.data_to_encode[i]);
            }
        } else {
            for(int i = 0; i < sizeof(T); i++) {
                out.push_back(d_.data_to_encode[sizeof(T) -1 -i]);
            }
        }
    }
private:
    U d_;
};
}
//encode implement
void encode(MpkNil value, vector<uint8_t>& out) {
    out.push_back(prefix_nil);
}
void encode(bool value, vector<uint8_t>& out) {
    if(value)
        out.push_back(prefix_true);
    else
        out.push_back(prefix_false);
}
void encode(int8_t value, vector<uint8_t>& out) {
    if(((1u<<7)&value) == 0)
        out.push_back(*reinterpret_cast<uint8_t*>(&value));
    else if((~((1u<<5)-1) & value) == (~((1u<<5)-1)))
        out.push_back(*reinterpret_cast<uint8_t*>(&value));
    else {
        out.push_back(prefix_int8);
        out.push_back(*reinterpret_cast<uint8_t*>(&value));
    }
}
void encode(int16_t value, vector<uint8_t>& out) {
    if(value <= std::numeric_limits<int8_t>::max() && value >= std::numeric_limits<int8_t>::min())
        encode(static_cast<int8_t>(value), out);
    else {
        out.push_back(prefix_int16);
        encode_value<int16_t> encoder(value);
        encoder(out);
    }
}
void encode(int32_t value, vector<uint8_t>& out) {
    if(value <= std::numeric_limits<int16_t>::max() && value >= std::numeric_limits<int16_t>::min())
        encode(static_cast<int16_t>(value), out);
    else {
        out.push_back(prefix_int32);
        encode_value<int32_t> encoder(value);
        encoder(out);
    }
}
void encode(int64_t value, vector<uint8_t>& out) {
    std::cout << "int int64_t encode " << value << std::endl;
    if(value <= std::numeric_limits<int32_t>::max() && value >= std::numeric_limits<int32_t>::min())
        encode(static_cast<int32_t>(value), out);
    else {
        out.push_back(prefix_int64);
        encode_value<int64_t> encoder(value);
        encoder(out);
    }
}
void encode(uint8_t value, vector<uint8_t>& out) {
    if(((1u<<7)&value)==0)
        out.push_back(value);
    else
        out.push_back(*reinterpret_cast<uint8_t*>(&value));
}
void encode(uint16_t value, vector<uint8_t>& out) {
    if(value <= std::numeric_limits<uint8_t>::max())
        encode(static_cast<uint8_t>(value), out);
    else {
        out.push_back(prefix_uint16);
        encode_value<uint16_t> encoder(value);
        encoder(out);
    }
}
void encode(uint32_t value, vector<uint8_t>& out) {
    if(value <= std::numeric_limits<uint16_t>::max())
        encode(static_cast<uint16_t>(value), out);
    else {
        out.push_back(prefix_uint32);
        encode_value<uint32_t> encoder(value);
        encoder(out);
    }
}
void encode(uint64_t value, vector<uint8_t>& out) {
    if(value <= std::numeric_limits<uint32_t>::max())
        encode(static_cast<uint32_t>(value), out);
    else {
        out.push_back(prefix_uint64);
        encode_value<uint64_t> encoder(value);
        encoder(out);
    }
}
void encode(float value, vector<uint8_t>& out) {
    out.push_back(prefix_float32);
    encode_value<float> encoder(value);
    encoder(out);
}
void encode(double value, vector<uint8_t>& out) {
    out.push_back(prefix_float64);
    encode_value<double> encoder(value);
    encoder(out);
}

void encode(const string& value, vector<uint8_t>& out){
    using namespace std;;
    if(value.size()<32){
        out.push_back((((1<<8)-1) & value.size()) | 0xA0);
        
    } else if( value.size() <= numeric_limits<uint8_t>::max()){
        out.push_back(prefix_str8);
        encode_value<uint8_t> encoder(value.size());
        encoder(out);
    } else if( value.size() <= numeric_limits<uint16_t>::max()){
        out.push_back(prefix_str16);
        encode_value<uint16_t> encoder(value.size());
        encoder(out);
    } else if (value.size() <= numeric_limits<uint32_t>::max()){
        out.push_back(prefix_str32);
        encode_value<uint32_t> encoder(value.size());
        encoder(out);
    } else 
        throw "carsh because a str has size large than 2^32";
    copy(value.begin(), value.end(), back_inserter(out));
}
void encode(const vector<uint8_t>& value, vector<uint8_t>& out){
    using namespace std;;
    if( value.size() <= numeric_limits<uint8_t>::max()){
        out.push_back(prefix_bin8);
        encode_value<uint8_t> encoder(value.size());
        encoder(out);
    } else if( value.size() <= numeric_limits<uint16_t>::max()){
        out.push_back(prefix_bin16);
        encode_value<uint16_t> encoder(value.size());
        encoder(out);
    } else if (value.size() <= numeric_limits<uint32_t>::max()){
        out.push_back(prefix_bin32);
        encode_value<uint32_t> encoder(value.size());
        encoder(out);
    } else 
        throw "carsh because a bin has size large than 2^32";
    copy(value.begin(), value.end(), back_inserter(out));
}
void encode(vector<mpk> value, vector<uint8_t>& out);
void encode(map<mpk, mpk> value, vector<uint8_t>& out);





}
#endif
