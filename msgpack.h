#pragma once
#ifndef MSGPACK_H_
#define MSGPACK_H_
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <memory>
namespace msgpack
{

namespace {
    union endian_check_t{
        uint16_t data;
        uint8_t check[2];
    }
    const endian_check_t endian_check_data{0xffef};
    const IsBigEndian = endian_check_data.check[0] == 0xff;
    const IsLittleEndian = !IsBigEndian;
}
    
using std::map;
using std::string;
using std::vector;
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

uint8_t prefix_nil = 0xc0;
uint8_t prefix_false = 0xc2;
uint8_t prefix_true = 0xc3;
uint8_t prefix_bin8 = 0xc4;
uint8_t prefix_bin16 = 0xc5;
uint8_t prefix_bin32 = 0xc6;
uint8_t prefix_ext8 = 0xc7;
uint8_t prefix_ext16 = 0xc8;
uint8_t prefix_ext32 = 0xc9;
uint8_t prefix_float32 = 0xca;
uint8_t prefix_float64 = 0xcb;
uint8_t prefix_uint8 = 0xcc;
uint8_t prefix_uint16 = 0xcd;
uint8_t prefix_uint32 = 0xce;
uint8_t prefix_uint64 = 0xcf;
uint8_t prefix_int8 = 0xd0;
uint8_t prefix_int16 = 0xd1;
uint8_t prefix_int32 = 0xd2;
uint8_t prefix_int64 = 0xd3;
uint8_t prefix_fixext1 = 0xd4;
uint8_t prefix_fixext2 = 0xd5;
uint8_t prefix_fixext4 = 0xd6;
uint8_t prefix_fixext8 = 0xd7;
uint8_t prefix_fixext16 = 0xd8;
uint8_t prefix_str8 = 0xd9;
uint8_t prefix_str16 = 0xda;
uint8_t prefix_str32 = 0xdb;
uint8_t prefix_array16 = 0xdc;
uint8_t prefix_array32 = 0xdd;
uint8_t prefix_map16 = 0xde;
uint8_t prefix_map32 = 0xdf;

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

class MpkValue{
public:
    virtual void encode(vector<uint8_t>&) = 0;
    virtual msgpack::TYPE type() = 0;
    virtual less(const MpkValue&) = 0;
    virtual equal(const MpkValue&) = 0;
    
}

template<MsgType type_tag, typename T>
class Value:public MpkValue{
public:
    
private:
    T m_value;
}



}
#endif
