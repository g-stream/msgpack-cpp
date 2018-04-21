#ifndef PARSER_H_
#define PARSER_H_
#include "config.h"
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <functional>
#include <memory>
namespace msgpack
{
bool is_ext(unsigned char);
bool is_fix(unsigned char);

class Parser{
public:
    Parser(std::string source):source_(source){
        if(Parser::parser_functions == nullptr){
            init_parser_functions();
        }
        (*Parser::parser_functions)[0]();
    }

private:
    static void init_parser_functions();
    static std::shared_ptr<std::array<std::function<void()>, 256>> parser_functions;
    std::string source_;
};


}//namespace msgpack
#endif
