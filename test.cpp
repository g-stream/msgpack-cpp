#include <iostream>
#include <cstdio>
#include <cstdio>
#include <vector>

#include <map>
#include "msgpack.h"
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
    v.clear();
}
int main()
{
    using namespace msgpack;
    using namespace std;

    Test test;

    ASSERT_OK("test", 1 == 1);
    msgpack::mpk_bool_t vtrue(true);
    msgpack::mpk_bool_t vfalse(false);
    ASSERT_OK("mpkvalue compare", vfalse < vtrue);
    ASSERT_OK("compare int", mpk(8) < mpk(16));
    ASSERT_OK("compare different type", mpk() < mpk(0.1));
    auto a = mpk(0xffffffffff);
    vector<uint8_t> v;
    auto b = mpk(string("00000000000000000"));
    auto c = mpk(vector<mpk>{a,b});
    auto d = mpk(map<mpk,mpk>{{a,b}});
    
    c.encode(v);
    for(auto i: v)
        printf("%02x ", i);
    v.clear();
    d.encode(v);
    cout << endl;
    for(auto i: v)
        printf("%02x ", i);
    
    auto dd = mpk(map<mpk,mpk>{{d,d},{a,c}});
    v.clear();
    dd.encode(v);
    cout << endl;
    for(auto i: v)
        printf("%02x ", i);
    
    
}
