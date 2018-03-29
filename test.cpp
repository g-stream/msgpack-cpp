#include <iostream>
#include <cstdio>
#include "testharness.h"
#include "config.h"
#include "parser.h"
#include <cstdio>
#include <map>
int main(){
    ASSERT_EQ(0,msgpack::TYPE_PREFIX::FIX_UNINT);
    ASSERT_OK(msgpack::is_fix(0x0));
    ASSERT_OK(msgpack::is_fix(0b1000));
    ASSERT_OK(msgpack::is_fix(0b111));
    ASSERT_OK(msgpack::is_fix(0b1001));
    ASSERT_OK(msgpack::is_fix(0b101));
    for(int i = 0xc0; i <= 0xd3; i++){
        ASSERT_OK(!msgpack::is_fix(i));
    }
    for(int i = 0xd9; i <= 0xdf; i++){
        ASSERT_OK(!msgpack::is_fix(i));
    }
    for(int i = 0xc7; i <= 0xc9; i++){
        ASSERT_OK(msgpack::is_ext(i));
    }
    for(int i = 0xd4; i <= 0xd7; i++){
        ASSERT_OK(msgpack::is_ext(i));
    }
    

}