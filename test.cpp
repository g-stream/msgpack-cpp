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
    
    vector<uint8_t> v{0xff,0xff,0x00,0x00};
    decode_value decoder(v,0,4);
    cout << decoder();
}
