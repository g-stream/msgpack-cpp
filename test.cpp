#include <iostream>
#include <cstdio>
#include <cstdio>
#include <map>
//#include "msgpack.h"
struct Test{
    static void assert_ok(const char* name, bool ok, const char* file, int line){
        cnt++;
        if(ok){
            passed_cnt++;
        } else {
            std::cout << name << " : failed in file: " << file  << " line: " << line << std::endl;
        }
    }
    ~Test(){
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
int main()
{
  //using namespace msgpack;
  Test test;
  ASSERT_OK("test", 1 == 1);


}
