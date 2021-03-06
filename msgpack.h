#pragma once
#ifndef MSGPACK_H_
#define MSGPACK_H_
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <limits>
#include <cassert>
#include <cstdint>
#include <memory>
#include <algorithm>
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
using std::make_shared;
struct MpkNil
{
    bool operator<(const MpkNil& other){
        return false;
    }
    bool operator==(const MpkNil& other){
        return true;
    }
};
enum class TYPE
{
    NIL,
    BOOL,
    INT8,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    FLOAT32,
    FLOAT64,
    STRING,
    BIN,
    ARRAY,
    MAP,
    EXTENSION
};
static const auto mpk_nil = TYPE::NIL;
static const auto mpk_bool = TYPE::BOOL;
static const auto mpk_int8 = TYPE::INT8;
static const auto mpk_int16 = TYPE::INT16;
static const auto mpk_int32 = TYPE::INT32;
static const auto mpk_int64 = TYPE::INT64;
static const auto mpk_uint8 = TYPE::UINT8;
static const auto mpk_uint16 = TYPE::UINT16;
static const auto mpk_uint32 = TYPE::UINT32;
static const auto mpk_uint64 = TYPE::UINT64;
static const auto mpk_float32 = TYPE::FLOAT32;
static const auto mpk_float64 = TYPE::FLOAT64;
static const auto mpk_string = TYPE::STRING;
static const auto mpk_bin = TYPE::BIN;
static const auto mpk_array = TYPE::ARRAY;
static const auto mpk_map = TYPE::MAP;
static const auto mpk_extension = TYPE::EXTENSION;

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
    mpk(const string&);
    mpk(const char*);
    mpk(const vector<uint8_t>&);
    mpk(char* str, size_t n);
    mpk(const vector<mpk>&);
    mpk(const map<mpk, mpk>&);
    mpk(vector<mpk>::iterator, vector<mpk>::iterator);
    msgpack::TYPE type() const;
    bool less(const mpk & other) const ;
    bool equals(const mpk & other) const ;
    void encode(vector<uint8_t>&) const;
private:
    std::shared_ptr<MpkValue> m_ptr;
};

class MpkValue {
public:
    virtual void encode(vector<uint8_t>& out) = 0;
    virtual msgpack::TYPE type() const = 0;
    virtual decltype(typeid(int)) specific_type() const = 0;
    virtual ~MpkValue(){};
};

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
            for(size_t i = 0; i < sizeof(T); i++) {
                out.push_back(d_.data_to_encode[i]);
            }
        } else {
            for(size_t i = 0; i < sizeof(T); i++) {
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
    else{
        out.push_back(prefix_uint8);
        out.push_back(*reinterpret_cast<uint8_t*>(&value));
    }
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



void encode(const std::string& value, vector<uint8_t>& out) {
    if(value.length()<32) {
        out.push_back((((1<<5)-1) & value.length()) | 0xA0);
    } else if( value.size() <= std::numeric_limits<uint8_t>::max()) {
        out.push_back(prefix_str8);
        encode_value<uint8_t> encoder(value.size());
        encoder(out);
    } else if( value.size() <= std::numeric_limits<uint16_t>::max()) {
        out.push_back(prefix_str16);
        encode_value<uint16_t> encoder(value.size());
        encoder(out);
    } else if (value.size() <= std::numeric_limits<uint32_t>::max()) {
        out.push_back(prefix_str32);
        encode_value<uint32_t> encoder(value.size());
        encoder(out);
    } else
        throw "carsh because a str has size large than 2^32";
    copy(value.begin(), value.end(), back_inserter(out));
}
void encode(const vector<uint8_t>& value, vector<uint8_t>& out) {
    using namespace std;;
    if( value.size() <= std::numeric_limits<uint8_t>::max()) {
        out.push_back(prefix_bin8);
        encode_value<uint8_t> encoder(value.size());
        encoder(out);
    } else if( value.size() <= std::numeric_limits<uint16_t>::max()) {
        out.push_back(prefix_bin16);
        encode_value<uint16_t> encoder(value.size());
        encoder(out);
    } else if (value.size() <= std::numeric_limits<uint32_t>::max()) {
        out.push_back(prefix_bin32);
        encode_value<uint32_t> encoder(value.size());
        encoder(out);
    } else
        throw "carsh because a bin has size large than 2^32";
    copy(value.begin(), value.end(), back_inserter(out));
}
void encode(vector<mpk> value, vector<uint8_t>& out){
    if(value.size()<=15)
        out.push_back((((1<<4)-1)&value.size())|0x90);
    else if(value.size() <= std::numeric_limits<uint16_t>::max()){
        out.push_back(prefix_array16);
        encode_value<uint16_t> encoder(value.size());
        encoder(out);        
    } else if(value.size() <= std::numeric_limits<uint32_t>::max()){
        out.push_back(prefix_array32);
        encode_value<uint32_t> encoder(value.size());
        encoder(out);
    } else {
        throw "crash because array's size out of max value of uint32_t";
    }
    for(auto item: value){
        item.encode(out);
    }
};
void encode(map<mpk, mpk> value, vector<uint8_t>& out){
    if(value.size()<=15)
        out.push_back((((1<<4)-1)&value.size())|0x80);
    else if(value.size() <= std::numeric_limits<uint16_t>::max()){
        out.push_back(prefix_map16);
        encode_value<uint16_t> encoder(value.size());
        encoder(out);        
    } else if(value.size() <= std::numeric_limits<uint32_t>::max()){
        out.push_back(prefix_map32);
        encode_value<uint32_t> encoder(value.size());
        encoder(out);
    } else {
        throw "crash because map's size out of max value of uint32_t";
    }
    for(auto item: value){
        item.first.encode(out);
        item.second.encode(out);
    }
};
template<msgpack::TYPE type_tag, typename T>
class Value:public MpkValue {
public:
    explicit Value(const T& v):m_value(v) {}
    virtual void encode(vector<uint8_t>& out) override {
        msgpack::encode(m_value, out);
    }
    virtual msgpack::TYPE type() const override {
        return type_tag;
    }
    virtual decltype(typeid(int)) specific_type() const override {
        return typeid(m_value);
    }
    T get() const {
        return m_value;
    }
    bool operator<(const MpkValue& other){
        if(type()!=other.type()){
            return type() < other.type();
        }
        return m_value < dynamic_cast<const Value<type_tag, T>*>(&other)->get();
    }
private:
    const T m_value;
};

class mpk_bool_t:public Value<mpk_bool, bool> {
public:
    mpk_bool_t(bool v):Value<mpk_bool, bool>(v){}
};
class mpk_nil_t:public Value<mpk_nil, MpkNil> {
public:
    mpk_nil_t():Value<mpk_nil, MpkNil>(MpkNil{}){}
};

class mpk_int8_t:public Value<mpk_int8, int8_t> {
public:
    mpk_int8_t(int8_t v):Value<mpk_int8, int8_t>(v){}
};
class mpk_int16_t:public Value<mpk_int16, int16_t> {
public:
    mpk_int16_t(int16_t v):Value<mpk_int16, int16_t>(v){}
};
class mpk_int32_t:public Value<mpk_int32, int32_t> {
public:
    mpk_int32_t(int32_t v):Value<mpk_int32, int32_t>(v){}
};
class mpk_int64_t:public Value<mpk_int64, int64_t> {
public:
    mpk_int64_t(int64_t v):Value<mpk_int64, int64_t>(v){}
};
class mpk_uint8_t:public Value<mpk_uint8, uint8_t> {
public:
    mpk_uint8_t(uint8_t v):Value<mpk_uint8, uint8_t>(v){}
};
class mpk_uint16_t:public Value<mpk_uint16, uint16_t> {
public:
    mpk_uint16_t(uint16_t v):Value<mpk_uint16, uint16_t>(v){}
};
class mpk_uint32_t:public Value<mpk_uint32, uint32_t> {
public:
    mpk_uint32_t(uint32_t v):Value<mpk_uint32, uint32_t>(v){}
};
class mpk_uint64_t:public Value<mpk_uint64, uint64_t> {
public:
    mpk_uint64_t(uint64_t v):Value<mpk_uint64, uint64_t>(v){}
};

class mpk_float32_t:public Value<mpk_float32, float> {
public:
    mpk_float32_t(float v):Value<mpk_float32, float>(v){}
};
class mpk_float64_t:public Value<mpk_float64, double> {
public:
    mpk_float64_t(double v):Value<mpk_float64, double>(v){}
};

class mpk_bin_t:public Value<mpk_bin, vector<uint8_t>> {
public:
    mpk_bin_t(const vector<uint8_t>& v):Value<mpk_bin, vector<uint8_t>>(v){}
};
class mpk_string_t:public Value<mpk_string, string> {
public:
    mpk_string_t(const string& v):Value<mpk_string, string>(v){}
};

class mpk_array_t:public Value<mpk_array, vector<mpk>> {
public:
    mpk_array_t(const vector<mpk>& v):Value<mpk_array, vector<mpk>>(v){}
};
class mpk_map_t:public Value<mpk_map, map<mpk, mpk>> {
public:
    mpk_map_t(const map<mpk, mpk>& v):Value<mpk_map, map<mpk, mpk>>(v){}
};

msgpack::TYPE msgpack::mpk::type() const
{
        return m_ptr->type();
}

msgpack::mpk::mpk():m_ptr(make_shared<mpk_nil_t>()){}
msgpack::mpk::mpk(bool v):m_ptr(make_shared<mpk_bool_t>(v)){}

msgpack::mpk::mpk(int8_t v):m_ptr(make_shared<mpk_int8_t>(v)){}
msgpack::mpk::mpk(int16_t v):m_ptr(make_shared<mpk_int16_t>(v)){}
msgpack::mpk::mpk(int32_t v):m_ptr(make_shared<mpk_int32_t>(v)){}
msgpack::mpk::mpk(int64_t v):m_ptr(make_shared<mpk_int64_t>(v)){}
msgpack::mpk::mpk(uint8_t v):m_ptr(make_shared<mpk_uint8_t>(v)){}
msgpack::mpk::mpk(uint16_t v):m_ptr(make_shared<mpk_uint16_t>(v)){}
msgpack::mpk::mpk(uint32_t v):m_ptr(make_shared<mpk_uint32_t>(v)){}
msgpack::mpk::mpk(uint64_t v):m_ptr(make_shared<mpk_uint64_t>(v)){}
msgpack::mpk::mpk(float v):m_ptr(make_shared<mpk_float32_t>(v)){}
msgpack::mpk::mpk(double v):m_ptr(make_shared<mpk_float64_t>(v)){}

msgpack::mpk::mpk(const string& v):m_ptr(make_shared<mpk_string_t>(v)){}
msgpack::mpk::mpk(const char* v):mpk(string(v)){};
msgpack::mpk::mpk(const vector<uint8_t>& v):m_ptr(make_shared<mpk_bin_t>(v)){}

msgpack::mpk::mpk(const vector<mpk>& v):m_ptr(make_shared<mpk_array_t>(v)){}
msgpack::mpk::mpk(const map<mpk, mpk>& v):m_ptr(make_shared<mpk_map_t>(v)){}


bool msgpack::mpk::less(const mpk& other) const {
    return m_ptr.get() < other.m_ptr.get();
}
bool msgpack::mpk::equals(const mpk& other) const {
    return m_ptr.get() == other.m_ptr.get();
}

bool operator<(const mpk& lhs, const mpk& rhs){
    return lhs.less(rhs);
}
bool operator==(const mpk& lhs, const mpk& rhs){
    return lhs.equals(rhs);
}
//todo extention

void msgpack::mpk::encode(vector<uint8_t>& v) const {
    m_ptr->encode(v);
}



//parser

mpk decode_core(vector<uint8_t>& in, size_t& cur);

template<typename T>
class decode_value{
public:
    union Data{
        T data;
        uint8_t data_array[sizeof(T)];
    };
    decode_value(vector<uint8_t>& in, size_t cur){
        assert(cur + sizeof(T) <= in.size());
        for(size_t i = 0; i < sizeof(T); i++){
            d_.data_array[i] = in[cur+i];
        }
    }
    T operator()(){
        if(IsBigEndian)
            return d_.data;
        else{
            std::reverse(d_.data_array, d_.data_array + sizeof(T));
            return d_.data;
        }
    }
private:
    Data d_;
};






mpk decode_nil(vector<uint8_t>& in, size_t& cur){
    assert(cur < in.size());
    cur++;
    return mpk();
}
mpk decode_false(vector<uint8_t>& in, size_t& cur){
    assert(cur < in.size());
    cur++;
    return mpk(false);
}
mpk decode_true(vector<uint8_t>& in, size_t& cur){
    assert(cur < in.size());
    cur++;
    return mpk(true);
}
mpk decode_bin_n(vector<uint8_t>& in, size_t& cur, size_t n){
    assert(cur + n <= in.size());
    vector<uint8_t> tmp;
    for(size_t i = 0; i < n; i++){
        tmp.push_back(in[cur+i]);
    }
    return mpk(tmp);
}
mpk decode_bin8(vector<uint8_t>& in, size_t& cur){
    cur++;
    decode_value<uint8_t> decoder(in, cur);
    size_t len = decoder();
    cur++;
    size_t curr = cur;
    cur += len;
    return decode_bin_n(in, curr, len);
}

mpk decode_bin16(vector<uint8_t>& in, size_t& cur){
    cur++;
    decode_value<uint16_t> decoder(in, cur);
    size_t len = decoder();
    cur += 2;
    size_t curr = cur;
    cur += len;
    return decode_bin_n(in, curr, len);
}
mpk decode_bin32(vector<uint8_t>& in, size_t& cur){
    cur++;
    decode_value<uint32_t> decoder(in, cur);
    size_t len = decoder();
    cur += 4;
    size_t curr = cur;
    cur += len;
    return decode_bin_n(in, curr, len);
}
mpk decode_float32(vector<uint8_t>& in, size_t& cur){
    assert(cur + 5 <= in.size());
    cur++;
    decode_value<float> decoder(in, cur);
    cur += 4;
    return mpk(decoder());
}
mpk decode_float64(vector<uint8_t>& in, size_t& cur){
    assert(cur + 9 <= in.size());
    cur++;
    decode_value<double> decoder(in, cur);
    cur += 8;
    return mpk(decoder());
}
mpk decode_uint8(vector<uint8_t>& in, size_t& cur){
    assert(cur + 2 <= in.size());
    cur++;
    decode_value<uint8_t> decoder(in, cur);
    cur++;
    return mpk(decoder());
}
mpk decode_uint16(vector<uint8_t>& in, size_t& cur){
    assert(cur + 3 <= in.size());
    cur++;
    decode_value<uint16_t> decoder(in, cur);
    cur += 2;
    return mpk(decoder());
}
mpk decode_uint32(vector<uint8_t>& in, size_t& cur){
    assert(cur + 5 <= in.size());
    cur++;
    decode_value<uint32_t> decoder(in, cur);
    cur += 4;
    return mpk(decoder());
}
mpk decode_uint64(vector<uint8_t>& in, size_t& cur){
    assert(cur + 9 <= in.size());
    cur++;
    decode_value<uint64_t> decoder(in, cur);
    cur += 8;
    return mpk(decoder());
}
mpk decode_int8(vector<uint8_t>& in, size_t& cur){
    assert(cur + 2 <= in.size());
    cur++;
    decode_value<int8_t> decoder(in, cur);
    cur++;
    return mpk(decoder());
}
mpk decode_int16(vector<uint8_t>& in, size_t& cur){
    assert(cur + 3 <= in.size());
    cur++;
    decode_value<int16_t> decoder(in, cur);
    cur += 2;
    return mpk(decoder());
}
mpk decode_int32(vector<uint8_t>& in, size_t& cur){
    assert(cur + 5 <= in.size());
    cur++;
    decode_value<int32_t> decoder(in, cur);
    cur += 4;
    return mpk(decoder());
}
mpk decode_int64(vector<uint8_t>& in, size_t& cur){
    assert(cur + 9 <= in.size());
    cur++;
    decode_value<int64_t> decoder(in, cur);
    cur += 8;
    return mpk(decoder());
}
mpk decode_str_n(vector<uint8_t>& in, size_t& cur, size_t n){
    assert(cur + n <= in.size());
    string tmp;
    for(size_t i = 0; i < n; i++){
        tmp.push_back(in[cur+i]);
    }
    return mpk(tmp);
}
mpk decode_str8(vector<uint8_t>& in, size_t& cur){
    cur++;
    decode_value<uint8_t> decoder(in, cur);
    size_t len = decoder();
    cur++;
    size_t curr = cur;
    cur += len;
    return decode_str_n(in, curr, len);
}
mpk decode_str16(vector<uint8_t>& in, size_t& cur){
    cur++;
    decode_value<uint16_t> decoder(in, cur);
    size_t len = decoder();
    cur += 2;
    size_t curr = cur;
    cur += len;
    return decode_str_n(in, curr, len);
}
mpk decode_str32(vector<uint8_t>& in, size_t& cur){
    cur++;
    decode_value<uint32_t> decoder(in, cur);
    size_t len = decoder();
    cur += 4;
    size_t curr = cur;
    cur += len;
    return decode_str_n(in, curr, len);   
}
mpk decode_fixstr(vector<uint8_t>& in, size_t& cur){
    size_t len = (0x1f & in[cur]);
    cur++;
    size_t curr = cur;
    cur += len;
    return decode_str_n(in, curr, len);   
}


//////////////
mpk decode_array_n(vector<uint8_t>& in, size_t& cur, size_t n){
    vector<mpk> tmp;
    for(size_t i = 0; i < n; i++){
        tmp.push_back(decode_core(in, cur));
    }
    return tmp;
}
mpk decode_array16(vector<uint8_t>& in, size_t& cur){
    cur++;
    decode_value<uint8_t> decoder(in, cur);
    size_t num = decoder();
    cur += 2;
    return decode_array_n(in, cur, num);
}
mpk decode_array32(vector<uint8_t>& in, size_t& cur){
    cur++;
    decode_value<uint32_t> decoder(in, cur);
    size_t num = decoder();
    cur += 4;
    return decode_array_n(in, cur, num);
}
mpk decode_fixarray(vector<uint8_t>& in, size_t& cur){
    size_t num = (0x0f&in[cur]);
    cur++;
    return decode_array_n(in, cur, num);
}

mpk decode_map_n(vector<uint8_t>& in, size_t& cur, size_t n){
    map<mpk, mpk> tmp;
    for(size_t i = 0; i < n; i++){
        auto key = decode_core(in, cur);
        auto value = decode_core(in, cur);
        tmp[key] = value;
    }
    return tmp;
}

mpk decode_map16(vector<uint8_t>& in, size_t& cur){
    cur++;
    decode_value<uint16_t> decoder(in, cur);
    size_t num = decoder();
    cur += 2;
    return decode_map_n(in, cur, num);
}
mpk decode_map32(vector<uint8_t>& in, size_t& cur){
    cur++;
    decode_value<uint32_t> decoder(in, cur);
    size_t num = decoder();
    cur += 4;
    return decode_map_n(in, cur, num);
}
mpk decode_fixmap(vector<uint8_t>& in, size_t& cur){
    size_t num = (0x0f&in[cur]);
    cur++;
    return decode_map_n(in, cur, num);
}




typedef mpk (*decode_function_type)(vector<uint8_t>&, size_t&);
decode_function_type decode_functions[256] = {
    [prefix_nil]      =  decode_nil,
    [prefix_false]    =  decode_false,
    [prefix_true]     =  decode_true,
    [prefix_bin8]     =  decode_bin8,
    [prefix_bin16]    =  decode_bin16,
    [prefix_bin32]    =  decode_bin32,
    [prefix_float32]  =  decode_float32,
    [prefix_float64]  =  decode_float64,
    [prefix_uint8]    =  decode_uint8,
    [prefix_uint16]   =  decode_uint16,
    [prefix_uint32]   =  decode_uint32,
    [prefix_uint64]   =  decode_uint64,
    [prefix_int8]     =  decode_int8,
    [prefix_int16]    =  decode_int16,
    [prefix_int32]    =  decode_int32,
    [prefix_int64]    =  decode_int64,
    [prefix_str8]     =  decode_str8,
    [prefix_str16]    =  decode_str16,
    [prefix_str32]    =  decode_str32,
    [prefix_array16]  =  decode_array16,
    [prefix_array32]  =  decode_array32,
    [prefix_map16]    =  decode_map16,
    [prefix_map32]    =  decode_map32
};

//LL(1) parser
mpk decode_core(vector<uint8_t>& in, size_t& cur){
    assert(cur < in.size());
    if((1<<7 & in[cur]) == 0){//positive little int
        return mpk(static_cast<int8_t>(in[cur++]));
    }else if((0xe0 & in[cur]) == 0xe0){ //negtive little int
        return mpk(static_cast<int8_t>(in[cur++]));
    }else if((0xf0 & in[cur]) == 0x80){
        return decode_fixmap(in, cur);
    }else if((0xf0 & in[cur]) == 0x90){
        return decode_fixarray(in, cur);
    }else if((0xe0 & in[cur]) == 0xa0){
        return decode_fixstr(in, cur);
    }else 
        return decode_functions[in[cur]](in, cur);
}

mpk decode(vector<uint8_t>& in){
    size_t index = 0;
    auto tmp = decode_core(in, index);
    //assert(index == in.size());
    return tmp;
}

}
#endif
