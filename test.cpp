#include <iostream>
#include <cstdio>
#include <cstdio>
#include <vector>

#include <map>
#include "msgpack.h"

using namespace msgpack;
using namespace std;

using byte_stream = vector<uint8_t>;
struct Test {
    static void assert_ok(const char* name, bool ok, const char* file, int line) {
        cnt++;
        if(ok) {
            passed_cnt++;
        } else {
            std::cout << name << " : failed in file: " << file  << " line: " << line << std::endl;
        }
    }
    ~Test() {
        std::cout << "tested " << cnt <<  " cases and " ;
        std::cout << passed_cnt  << " passed, " << passed_cnt*100.0/cnt << "%" <<std::endl;
    }
private:
    static int passed_cnt;
    static int cnt;

};

int Test::passed_cnt = 0;
int Test::cnt = 0;

#define ASSERT_OK(name, p) test.assert_ok((name), (p),__FILE__, __LINE__)

using namespace std;
void print_str(string str, std::vector<uint8_t> &v) {
    cout << str << endl;
    for(auto i: v)
        printf("%02x ",i);
    cout << endl;
}

bool test_encode_and_decode(mpk& value) {
    byte_stream bin;
    value.encode(bin);
    //print_str("one", bin);
    auto tmp = decode(bin);
    byte_stream bin2;
    tmp.encode(bin2);
    //print_str("two", bin2);
    return bin == bin2;
}
int main()
{
    Test test;

    ASSERT_OK("test", 1 == 1);
    msgpack::mpk_bool_t vtrue(true);
    msgpack::mpk_bool_t vfalse(false);
    ASSERT_OK("mpkvalue compare", vfalse < vtrue);
    ASSERT_OK("compare int", mpk(8) < mpk(16));
    ASSERT_OK("compare different type", mpk() < mpk(0.1));

    auto test_mpk_string_v = mpk("aaaa");
    auto test_mpk_uint8_max_v = mpk(numeric_limits<uint8_t>::max());
    auto test_mpk_uint16_max_v = mpk(numeric_limits<uint16_t>::max());
    auto test_mpk_uint32_max_v = mpk(numeric_limits<uint32_t>::max());
    auto test_mpk_uint64_max_v = mpk(numeric_limits<uint64_t>::max());
    auto test_mpk_int8_min_v = mpk(numeric_limits<int8_t>::min());
    auto test_mpk_int8_max_v = mpk(numeric_limits<int8_t>::max());
    auto test_mpk_int16_min_v = mpk(numeric_limits<int16_t>::min());
    auto test_mpk_int16_max_v = mpk(numeric_limits<int16_t>::max());
    auto test_mpk_int32_min_v = mpk(numeric_limits<int32_t>::min());
    auto test_mpk_int32_max_v = mpk(numeric_limits<int32_t>::max());
    auto test_mpk_int64_min_v = mpk(numeric_limits<int64_t>::min());
    auto test_mpk_int64_max_v = mpk(numeric_limits<int64_t>::max());

    auto max_min_array = mpk(vector<mpk> {test_mpk_uint8_max_v, test_mpk_uint16_max_v, test_mpk_uint32_max_v , test_mpk_uint64_max_v,test_mpk_int8_min_v , test_mpk_int8_max_v ,test_mpk_int16_min_v ,test_mpk_int16_max_v ,test_mpk_int32_min_v ,test_mpk_int64_min_v , test_mpk_int64_max_v});

    
    ASSERT_OK("max_min_array", test_encode_and_decode(max_min_array));
    
    ASSERT_OK("fixstr", test_encode_and_decode(test_mpk_string_v));
    string str8 = string("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    string str16;
    for(int i = 0; i < 0xffff; i++){
        str16.push_back('x');
    }
    string str32;
    for(int i = 0; i < 0xffff; i++){
        str32.push_back('x');
        str32.push_back('f');
    }
    auto test_string8_v = mpk(str8);
    auto test_string16_v = mpk(str16);
    auto test_string32_v = mpk(str32);
    ASSERT_OK("test str8", test_encode_and_decode(test_string8_v));
    ASSERT_OK("test str16", test_encode_and_decode(test_string16_v));
    ASSERT_OK("test str32", test_encode_and_decode(test_string32_v));
    
    auto test_false_v = mpk(false);
    ASSERT_OK("test false", test_encode_and_decode(test_false_v));
    auto test_true_v = mpk(true);
    ASSERT_OK("test true", test_encode_and_decode(test_true_v));
    auto test_nil_v = mpk();
    ASSERT_OK("test nil", test_encode_and_decode(test_nil_v));
    auto test_bin8_v = mpk(byte_stream{0xff,0xef, 0xac, 0x00});
    ASSERT_OK("test bin8", test_encode_and_decode(test_bin8_v));
    byte_stream test_bin16_byte_stream;
    for(int i = 0; i < 300; i++){
        test_bin16_byte_stream.push_back(0xef);
        test_bin16_byte_stream.push_back(0xff);
        test_bin16_byte_stream.push_back(0x00);
    }
    byte_stream test_bin32_byte_stream;
    for(int i = 0; i < 65535; i++){
        test_bin32_byte_stream.push_back(0xef);
        test_bin32_byte_stream.push_back(0xff);
        test_bin32_byte_stream.push_back(0x00);
    }
    auto test_bin16_v = mpk(test_bin16_byte_stream);
    ASSERT_OK("test bin16", test_encode_and_decode(test_bin16_v));
    auto test_bin32_v = mpk(test_bin32_byte_stream);
    ASSERT_OK("test bin32", test_encode_and_decode(test_bin32_v));
    
    auto test_mix_array_v = mpk(vector<mpk>{test_mpk_string_v, test_string8_v, test_string16_v, test_string32_v, test_mpk_int16_max_v, max_min_array, test_false_v, test_true_v, test_nil_v, test_bin16_v, test_bin32_v, test_bin8_v});
    ASSERT_OK("test mix array", test_encode_and_decode(test_mix_array_v));
    
    auto test_mix_map_v = mpk(map<mpk,mpk>{{test_mpk_string_v,test_string8_v}, {test_false_v, test_true_v}, {test_string16_v, test_string32_v}, {test_mpk_int16_max_v, max_min_array}, {test_nil_v, test_bin16_v}, {test_bin32_v, test_mix_array_v}});
   // ASSERT_OK("test mix map", test_encode_and_decode(test_mix_map_v));
    
    auto test_int_to_int_map_v = mpk(map<mpk,mpk>{{test_mpk_int16_max_v, test_mpk_int16_min_v}, {test_mpk_int32_max_v, test_mpk_int32_min_v}});
    ASSERT_OK("test int to int map", test_encode_and_decode(test_int_to_int_map_v));
    auto test_int_to_int_string_to_string_map_v = mpk(map<mpk,mpk>{{test_mpk_int16_max_v, test_mpk_int16_min_v}, {test_mpk_int32_max_v, test_mpk_int32_min_v}, {test_string16_v, test_string32_v}});
    ASSERT_OK("test int to int map add string to string map", test_encode_and_decode(test_int_to_int_string_to_string_map_v));
    
}
