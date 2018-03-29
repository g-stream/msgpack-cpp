#ifndef TESTHARNESS_H_
#define TESTHARNESS_H_

#include <cstdio>
#include <iostream>
namespace msgpack{
    namespace test{
#define TEST_WITH_OP(test, opname, op) \
template<typename X, typename Y> \
void test##opname (const char* file, int line, X x, Y y){  \
    if(!(x op y))\
        printf("crashed at file: %s  line: %d\n", file, line); \
}

TEST_WITH_OP(test, Ge, >=);
TEST_WITH_OP(test, Gt, >);
TEST_WITH_OP(test, Eq, ==);
TEST_WITH_OP(test, Lt, <);
TEST_WITH_OP(test, Le, <=);
TEST_WITH_OP(test, Ne, !=);
#undef TEST_WITH_OP
template<typename Q>
void testOK(const char* file, int line, Q q){ \
    if(!(q)){
        printf("crashed at file: %s  line: %d\n", file, line); \
    }\
}

#define ASSERT_GE(x, y) ::msgpack::test::testGe(__FILE__, __LINE__, (x), (y))
#define ASSERT_GT(x, y) ::msgpack::test::testGt(__FILE__, __LINE__, (x), (y))
#define ASSERT_EQ(x, y) ::msgpack::test::testEq(__FILE__, __LINE__, (x), (y))
#define ASSERT_LT(x, y) ::msgpack::test::testLt(__FILE__, __LINE__, (x), (y))
#define ASSERT_LE(x, y) ::msgpack::test::testLe(__FILE__, __LINE__, (x), (y))
#define ASSERT_NE(x, y) ::msgpack::test::testNe(__FILE__, __LINE__, (x), (y))

#define ASSERT_OK(q) ::msgpack::test::testOK(__FILE__, __LINE__, (q))
}
}
#endif 