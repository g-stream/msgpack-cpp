#pragma once
#ifndef MSGPACK_H_
#define MSGPACK_H_
#include <string>
#include <vector>
#include <map>
#include <cstdint>
namespace msgpack
{

using std::map;
using std::string;
using std::vector;
struct MPK_NIL
{
};
enum class mpk_type
{
    NIL,
    BOOL,
    INT,
    FLOAT,
    STRING,
    BIN,
    ARRAY,
    MAP
};
auto mpk_nil = mpk_type::NIL;
auto mpk_bool = mpk_type::BOOL;
auto mpk_int = mpk_type::INT;
auto mpk_float = mpk_type::FLOAT;
auto mpk_string = mpk_type::STRING;
auto mpk_bin = mpk_type::BIN;
auto mpk_array = mpk_type::ARRAY;
auto mpk_map = mpk_type::MAP;

bool mpk::operator<(const mpk &rhs)
{
    if (type != rhs.type)
    {
        return type < rhs.type;
    }
    else
    {
        return get() < rhs.get();
    }
}
bool operator<(const mpk &v1, const mpk &v2)
{
    return v1.compare(v2)
}
class mpk
{
  public:
    mpk();
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
    mpk(double); //double 一定是4byte ?
    mpk(string);
    mpk(string, mpk_type);
    mpk(vector<mpk>);
    mpk(map<string, mpk>);
    const mpk_type type;
    bool operator<(const mpk &);

  private:
    union mpk_value {
        MPK_NIL nil_v;
        bool bool_v;
        uint8_t uint8_v;
        uint16_t uint16_v;
        uint32_t uint32_v;
        uint64_t uint64_v;
        int8_t int8_v;
        int16_t int16_v;
        int32_t int32_v;
        int64_t int64_v;
        string string_v;
        float float_v;
        double double_v;
        vector<mpk> array_v;
        map<mpk, mpk> map_v;
    };
    mpk_value value_;
};

bool operator<(mpk, mpk);
}
#endif
