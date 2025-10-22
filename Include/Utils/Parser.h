#ifndef PARSER_HPP 
#define PARSER_HPP
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <optional>   
#include <unordered_map>
#include <vector>
#include "./Models/DataObjects.h"

using json = nlohmann::json;

class Parser
{
public:
    Parser();
    ~Parser();
    std::unordered_map<std::string, MenuData> menuMap;
};


#endif